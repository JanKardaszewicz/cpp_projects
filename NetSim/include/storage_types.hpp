//
// Jan Kardaszewicz 407313

#ifndef SIECI_STORAGE_TYPES_HPP
#define SIECI_STORAGE_TYPES_HPP
#include "package.hpp"
#include "types.hpp"
#include <list>
enum class PackageQueueType
{
    FIFO,
    LIFO
};
class IPackageStockpile
{
public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package&& other) = 0;
    virtual bool empty() = 0;
    virtual std::size_t size() = 0;
    virtual const_iterator begin() const  = 0;
    virtual const_iterator cbegin() const= 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;
    virtual ~IPackageStockpile() = default;
};
class IPackageQueue: public IPackageStockpile
{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
};
class PackageQueue: public IPackageQueue
{
    public:
    explicit PackageQueue(PackageQueueType type) : _type(type){};
    void push(Package&& other) override { _queue.emplace_back(std::move(other)); }
    Package pop() override;
    bool empty() override { return _queue.empty(); }
    std::size_t size() override { return _queue.size(); }
    const_iterator begin() const override {return _queue.begin();}
    const_iterator cbegin() const override {return _queue.cbegin();}
    const_iterator end() const override {return _queue.end();}
    const_iterator cend() const override {return _queue.cend();}
    PackageQueueType get_queue_type() override { return _type; }
    private:
    std::list<Package> _queue;
    PackageQueueType _type;
};
#endif //SIECI_STORAGE_TYPES_HPP
