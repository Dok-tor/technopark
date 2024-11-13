// units/crew_members.cpp
#include "crew_members.hpp"


// Определения методов класса CrewMember
CrewMember::CrewMember() : Unit() {}

std::string CrewMember::getType() const {
    return "CREW_MEMBER";
}

int CrewMember::getTotalHandLuggageWeight() const {
    return 0;
}

int CrewMember::getTotalLuggageWeight() const {
    return 0;
}

// Определения методов класса Pilot
Pilot::Pilot() : CrewMember() {}

std::string Pilot::getType() const {
    return "PILOT";
}

// Определения методов класса FlightAttendant
FlightAttendant::FlightAttendant() : CrewMember() {}

std::string FlightAttendant::getType() const {
    return "FLIGHT_ATTENDANT";
}
