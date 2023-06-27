//
// Jan Kardaszewicz 407313


#ifndef SIECI_NODES_HPP
#define SIECI_NODES_HPP
#include "config.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include <memory>
#include <map>
#include <list>
#include <optional>

enum class ReceiverType
{
    WORKER, STOREHOUSE
};


class IPackageReceiver
{
public:
    virtual void receive_package(Package && p) = 0;
    virtual ElementID get_id() const = 0;
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        virtual ReceiverType get_receiver_type() const = 0;
    #endif
    virtual ~IPackageReceiver() = default;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
};


class Storehouse : public IPackageReceiver
{
public:
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d =
    std::unique_ptr<IPackageStockpile>(new PackageQueue(PackageQueueType::FIFO)))
            : _d(std::move(d)), _id(id) {}
    void receive_package(Package && p) override;
    ElementID get_id() const override { return _id; }
    IPackageQueue * get_queue() const { return dynamic_cast<IPackageQueue*>(_d.get()); }
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; }
    #endif
    IPackageStockpile::const_iterator cbegin() const override { return _d->cbegin(); }
    IPackageStockpile::const_iterator cend() const override { return _d->cend(); }
    IPackageStockpile::const_iterator begin() const override { return _d->begin(); }
    IPackageStockpile::const_iterator end() const override { return _d->end(); }
private:
    std::unique_ptr<IPackageStockpile> _d;
    ElementID _id;
};


class ReceiverPreferences
{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator);
    void add_receiver(IPackageReceiver* ptr);
    void remove_receiver(IPackageReceiver * ptr);
    IPackageReceiver * choose_receiver();
    const preferences_t & get_preferences() const { return _p; }
    const_iterator begin() const{return _p.begin();}
    const_iterator cbegin() const {return _p.cbegin();}
    const_iterator end() const {return _p.end();}
    const_iterator cend() const {return _p.cend();}
    preferences_t _p;
private:
    ProbabilityGenerator _pg;
};


class PackageSender
{
public:
    PackageSender() = default;
    PackageSender(PackageSender && package_sender) = default;
    PackageSender& operator=(PackageSender && packageSender) = default;
    virtual ~PackageSender() = default;
    void send_package();
    std::optional<Package> & get_sending_buffer() const { return (std::optional<Package> &) std::move(_buffer); }
    ReceiverPreferences receiver_preferences_;
protected:
    virtual void push_package(Package&& package);
private:
    std::optional<Package> _buffer;
};


class Ramp : public PackageSender
{
public:
    explicit Ramp(ElementID id, TimeOffset di) : _id(id), _di(di) {}
    Ramp & operator=(Ramp && ramp) = default;
    Ramp(Ramp && ramp) = default;
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const { return _di; }
    ElementID get_id() const { return _id; }
private:
    ElementID _id;
    TimeOffset _di;
    Time _t;
};


class Worker : public PackageSender, public IPackageReceiver
{
public:
    explicit Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> ptr) : _id(id), _pd(pd), _t(0), _q(std::move(ptr)) {}
    void do_work(Time t);
    TimeOffset get_processing_duration() const { return _pd; }
    Time get_package_processing_start_time() const { return _t; }
    IPackageQueue * get_queue() const { return _q.get(); }
    void receive_package(Package && p) override;
    ElementID get_id() const override { return _id; }
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }
    #endif
    std::optional<Package> & get_processing_buffer() const { return (std::optional<Package> &) std::move(_worker_buffer); }
    IPackageStockpile::const_iterator cbegin() const override { return _q->cbegin(); }
    IPackageStockpile::const_iterator cend() const override { return _q->cend(); }
    IPackageStockpile::const_iterator begin() const override { return _q->begin(); }
    IPackageStockpile::const_iterator end() const override { return _q->end(); }
private:
    ElementID _id;
    TimeOffset _pd;
    Time _t;
    std::optional<Package> _worker_buffer;
    std::unique_ptr<IPackageQueue> _q;
};

#endif //SIECI_NODES_HPP
