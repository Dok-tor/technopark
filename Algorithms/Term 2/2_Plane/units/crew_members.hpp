// units/crew_members.hpp
#ifndef CREW_MEMBERS_HPP
#define CREW_MEMBERS_HPP

#include "unit.hpp"

class CrewMember : public Unit {
public:
    CrewMember();
    virtual std::string getType() const override;
    virtual int getTotalHandLuggageWeight() const override;
    virtual int getTotalLuggageWeight() const override;
};

class Pilot : public CrewMember {
public:
    Pilot();
    std::string getType() const override;
};

class FlightAttendant : public CrewMember {
public:
    FlightAttendant();
    std::string getType() const override;
};

#endif // CREW_MEMBERS_HPP
