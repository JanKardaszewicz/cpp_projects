//
// Jan Kardaszewicz 407313


#ifndef SIECI_TYPES_HPP
#define SIECI_TYPES_HPP
#include <functional>
using ElementID = unsigned int;
using Time = int;
using TimeOffset = int;
using ProbabilityGenerator = std::function<double()>;
#endif //SIECI_TYPES_HPP
