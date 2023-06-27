//
// Jan Kardaszewicz 407313

#ifndef SIECI_PACKAGE_H
#define SIECI_PACKAGE_H
#include "types.hpp"
#include <utility>
#include <list>
#include <set>
class Package
{
    public:
        Package();
        explicit Package(ElementID id);
//        Package(const Package& other): _id(other._id){};
        Package(Package&& other): _id(std::move(other._id)){};
        Package& operator=(Package&& other);
        ElementID get_id() const { return _id; };
        ~Package();
    private:
        ElementID _id;
        static std::set<ElementID> assigned_IDs;
        static std::set<ElementID> freed_IDs;
};
#endif //SIECI_PACKAGE_H
