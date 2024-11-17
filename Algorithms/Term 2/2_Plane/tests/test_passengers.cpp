// tests/passengers.cpp
#include "../units/passengers.hpp"
#include <cassert>
#include <iostream>

void testEconomyPassenger()
{
    EconomyPassenger passenger({ 5 }, { 10 });
    assert(passenger.getType() == "ECONOMY");
    assert(passenger.getTotalHandLuggageWeight() == 5);
    assert(passenger.getTotalLuggageWeight() == 10);
    assert(passenger.getMaxHandLuggagePieces() == 1);
    assert(passenger.getMaxHandLuggageTotalWeight() == 10);
    assert(passenger.getMaxLuggagePieces() == 1);
    assert(passenger.getMaxLuggageTotalWeight() == 24);
    std::cout << "EconomyPassenger tests passed!\n";
}

void testBusinessPassenger()
{
    BusinessPassenger passenger({ 10, 5 }, { 20 });
    assert(passenger.getType() == "BUSINESS");
    assert(passenger.getTotalHandLuggageWeight() == 15);
    assert(passenger.getTotalLuggageWeight() == 20);
    assert(passenger.getMaxHandLuggagePieces() == 2);
    assert(passenger.getMaxHandLuggageTotalWeight() == 24);
    assert(passenger.getMaxLuggagePieces() == 2);
    assert(passenger.getMaxLuggageTotalWeight() == 40);
    std::cout << "BusinessPassenger tests passed!\n";
}

void testFirstClassPassenger()
{
    FirstClassPassenger passenger({ 20, 30 }, { 50 });
    assert(passenger.getType() == "FIRST_CLASS");
    assert(passenger.getTotalHandLuggageWeight() == 50);
    assert(passenger.getTotalLuggageWeight() == 50);
    assert(passenger.getMaxHandLuggagePieces() == 2);
    assert(passenger.getMaxHandLuggageTotalWeight() == 60);
    assert(passenger.getMaxLuggagePieces() == 2);
    assert(passenger.getMaxLuggageTotalWeight() == INT32_MAX);
    std::cout << "FirstClassPassenger tests passed!\n";
}

int main()
{
    testEconomyPassenger();
    testBusinessPassenger();
    testFirstClassPassenger();
    return 0;
}
