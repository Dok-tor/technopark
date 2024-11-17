// tests/crew_members.cpp
#include "../units/crew_members.hpp"
#include <cassert>
#include <iostream>

void testCrewMember() {
  CrewMember crew;
  assert(crew.getType() == "CREW_MEMBER");
  assert(crew.getTotalHandLuggageWeight() == 0);
  assert(crew.getTotalLuggageWeight() == 0);
  std::cout << "CrewMember tests passed!\n";
}

void testPilot() {
  Pilot pilot;
  assert(pilot.getType() == "PILOT");
  assert(pilot.getTotalHandLuggageWeight() == 0);
  assert(pilot.getTotalLuggageWeight() == 0);
  std::cout << "Pilot tests passed!\n";
}

void testFlightAttendant() {
  FlightAttendant attendant;
  assert(attendant.getType() == "FLIGHT_ATTENDANT");
  assert(attendant.getTotalHandLuggageWeight() == 0);
  assert(attendant.getTotalLuggageWeight() == 0);
  std::cout << "FlightAttendant tests passed!\n";
}

int main() {
  testCrewMember();
  testPilot();
  testFlightAttendant();
  return 0;
}
