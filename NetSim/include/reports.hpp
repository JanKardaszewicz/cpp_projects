//
// Jan Kardaszewicz 407313


#ifndef SIECI_REPORTS_HPP
#define SIECI_REPORTS_HPP
#include "factory.hpp"

void generate_structure_report(const Factory & f, std::ostream & os);
void generate_simulation_turn_report(const Factory & f, std::ostream & os, Time t);

class IntervalReportNotifier
{
public:
    explicit IntervalReportNotifier(TimeOffset t) : _delta_t(t) {}
    bool should_generate_report(Time t);
private:
    TimeOffset _delta_t;
};

class SpecificTurnsReportNotifier
{
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns) : _turns(std::move(turns)) {}
    bool should_generate_report(Time t);
private:
    std::set<Time> _turns;
};

#endif //SIECI_REPORTS_HPP
