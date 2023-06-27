//
// Jan Kardaszewicz 407313


#include "../include/nodes.hpp"

#include <utility>

void Storehouse::receive_package(Package&& p) {
    _d->push(std::move(p));
}


void Worker::receive_package(Package&& p) {
    _q->push(std::move(p));
}


ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg) : _pg(pg) {
    _p = preferences_t();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* ptr) {
    auto it = std::find_if(_p.begin(), _p.end(),
                           [&ptr](auto & elem){ return ptr == elem.first; });
    if (it != _p.end()) {
        _p.extract(it->first);
        for (auto & elem : _p) {
            elem.second = double(1)/double(_p.size());
        }
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver* ptr) {
    auto it = std::find_if(_p.begin(), _p.end(),
                           [&ptr](auto &elem) { return elem.first == ptr; });
    if (it == _p.end()) {
        if (_p.size() > 0) {
            _p.emplace(ptr, 1/_p.size());
            for (auto & elem : _p) {
                elem.second = double(1)/double(_p.size());
            }
        }
        else {
            _p.emplace(ptr, 1);
        }

    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    std::vector<double> probability_vals;
    for (auto & elem : _p)
    {
        if (probability_vals.size() > 0)
            probability_vals.push_back(elem.second + probability_vals[probability_vals.size()-1]);
        probability_vals.push_back(0.0);
    }
    double probability = _pg();

    std::size_t i = 0;
    IPackageReceiver* ptr = _p.begin()->first;
    for (auto iter = _p.begin(); iter != _p.end(); ++iter, ++i)
    {
        if (probability < probability_vals[i])
            continue;
        else
        {
            ptr = iter->first;
            break;
        }
    }
    remove_receiver(ptr);
    return ptr;
}

void PackageSender::push_package(Package&& package)
{
    if (_buffer.has_value() == 0)
        _buffer.emplace(std::move(package));
}

void PackageSender::send_package()
{
    if (!receiver_preferences_.get_preferences().empty())
    {
        if (_buffer)
        {
            auto receiver = receiver_preferences_.choose_receiver();
            receiver->receive_package(std::move(_buffer.value()));
            _buffer.reset();
        }
    }
}

void Ramp::deliver_goods(Time t)
{
    _t = t;
    if(t % _di == 1){
        push_package(Package());
    }
}

void Worker::do_work(Time t) {
    if(!_worker_buffer.has_value()){
        if(!_q->empty()){
            _worker_buffer.emplace(_q->pop());
            _t = t;
        }
    }
    if(_worker_buffer.has_value())
    {
        if(t >= _pd - _t + 1)
        {
            PackageSender::get_sending_buffer().emplace(std::move(_worker_buffer.value()));
            _worker_buffer.reset();
            if(!_q->empty()){
                _worker_buffer.emplace(_q->pop());
                _t = t;
            }
        }
    }

}
