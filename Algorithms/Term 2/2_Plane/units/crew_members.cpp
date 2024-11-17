// units/crew_members.cpp
#include "crew_members.hpp"

CrewMember::CrewMember() : Unit() {}

std::string CrewMember::getType() const { return "CREW_MEMBER"; }

int CrewMember::getTotalHandLuggageWeight() const { return 0; }

int CrewMember::getTotalLuggageWeight() const { return 0; }

Pilot::Pilot() : CrewMember() {}

std::string Pilot::getType() const { return "PILOT"; }

FlightAttendant::FlightAttendant() : CrewMember() {}

std::string FlightAttendant::getType() const { return "FLIGHT_ATTENDANT"; }
