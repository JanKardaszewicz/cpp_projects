//
// Jan Kardaszewicz 407313

#include "../include/package.hpp"
std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};
Package::Package()
{
    if(!freed_IDs.empty())
    {
        _id = *(freed_IDs.begin());
        freed_IDs.erase(freed_IDs.begin());
        assigned_IDs.emplace(_id);
    }
    else
    {
        if(!assigned_IDs.empty())
        {
            _id = *(assigned_IDs.end())+1;
            assigned_IDs.emplace(_id);
        }
        else
        {
            _id = 1U;
            assigned_IDs.emplace(_id);
        }
    }
}
Package::Package(ElementID id)
{
    _id = id;
    freed_IDs.erase(_id);
    assigned_IDs.emplace(_id);
}
Package::~Package()
{
    freed_IDs.insert(_id);
    assigned_IDs.erase(_id);
    if (assigned_IDs.empty())
    {
        freed_IDs.clear();
    }
}
Package& Package::operator=(Package&& other)
{
    if(this == &other)
    {
        return *this;
    }
    freed_IDs.emplace(this -> _id);
    this -> _id = std::move(other._id);
    return *this;
}
