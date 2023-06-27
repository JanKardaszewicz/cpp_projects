//
// Jan Kardaszewicz 407313

#ifndef SIECI_FACTORY_HPP
#define SIECI_FACTORY_HPP
#include <vector>
#include <sstream>
#include "types.hpp"
#include "nodes.hpp"
template <class Node>
class NodeCollection
{
public:
    using list = typename std::list<Node>;
    using iterator = typename list::iterator;
    using const_iterator = typename list::const_iterator;
    void add(Node&& node);
    void remove_by_id(ElementID id);
    iterator find_by_id(ElementID id)
    {
        iterator it = std::find_if(_nodecollection.begin(), _nodecollection.end(),
                     [&id](const auto& elem){return id == elem.get_id();});
        return it;
    }
    const_iterator find_by_id(ElementID id) const
    {
        const_iterator it = std::find_if(_nodecollection.begin(), _nodecollection.end(),
                     [&id](const auto& elem){return id == elem.get_id();});
        return it;
    }
    list get_nodes_list() const { return _nodecollection; }
    const_iterator begin() const {return _nodecollection.begin();}
    const_iterator end() const {return _nodecollection.end();}
    const_iterator cbegin() const {return _nodecollection.cbegin();}
    const_iterator cend() const {return _nodecollection.cend();}
    iterator begin() {return _nodecollection.begin();}
    iterator end() {return _nodecollection.end();}
private:
    list _nodecollection;
};

class Factory
{
public:
    Factory() = default;
    enum NodeColor
    {
        UNVISITED,
        VISITED,
        VERIFIED
    };
    //Ramp
    void add_ramp(Ramp && other) {_ramp.add(std::move(other));}
    void remove_ramp(ElementID id) { remove_receiver(_ramp, id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return _ramp.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return _ramp.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return _ramp.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return _ramp.cend();}
    NodeCollection<Ramp>::const_iterator ramp_end() { return _ramp.end();}
    NodeCollection<Ramp>::const_iterator ramp_begin() { return _ramp.begin();}
    //Worker
    void add_worker(Worker && other) {_worker.add(std::move(other));}
    void remove_worker(ElementID id)  { remove_receiver(_worker, id); }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return _worker.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return _worker.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return _worker.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() const {return _worker.cend();};
    NodeCollection<Worker>::const_iterator worker_end() { return _worker.end();}
    NodeCollection<Worker>::const_iterator worker_begin() { return _worker.begin();}
    //Storehouse
    void add_storehouse(Storehouse && other) {_storehouse.add(std::move(other));}
    void remove_storehouse(ElementID id) {_storehouse.remove_by_id(id);}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return _storehouse.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return _storehouse.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return _storehouse.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return _storehouse.cend();}
    NodeCollection<Storehouse>::const_iterator storehouse_end() { return _storehouse.end();}
    NodeCollection<Storehouse>::const_iterator storehouse_begin() { return _storehouse.begin();}
    //bools
    bool is_consistent();
    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);
private:
    NodeCollection <Worker> _worker;
    NodeCollection <Storehouse> _storehouse;
    NodeCollection <Ramp> _ramp;
    template <class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
};
template <class Node>
void NodeCollection<Node>::add(Node&& node)
{
    if (find_by_id(node.get_id()) == _nodecollection.end())
    {
        _nodecollection.push_back(std::move(node));
    }
}
template <class Node>
void NodeCollection<Node>::remove_by_id(ElementID id)
{
    if (find_by_id(id) != _nodecollection.end())
    {
        _nodecollection.erase(find_by_id(id));
    }
}
template<class NodeType>
void Factory::remove_receiver(NodeCollection<NodeType> &collection, ElementID id)
{
    auto it = collection.find_by_id(id);
    auto ptr = dynamic_cast<IPackageReceiver*>(&(*it));
    std::list<PackageSender> senders;
    for(auto& r : _ramp)
    {
        auto& p = r.receiver_preferences_.get_preferences();
        for(auto&[key, val] : p)
        {
            if(key == ptr)
            {
                r.receiver_preferences_.remove_receiver(ptr);
                break;
            }
        }
    }
    for(auto& w : _worker)
    {
        auto& p = w.receiver_preferences_.get_preferences();
        for(auto&[key, val] : p)
        {
            if(key == ptr)
            {
                w.receiver_preferences_.remove_receiver(ptr);
                break;
            }
        }
    }
    collection.remove_by_id(id);
}

struct ParsedLineData
{
    enum class ElementType { RAMP, WORKER, STOREHOUSE, LINK };
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(std::string &line);
Factory load_factory_structure(std::istream & is);
std::pair<std::string, int> decompose(std::string str);
void save_factory_structure(Factory & factory, std::ostream & os);
#endif //SIECI_FACTORY_HPP
