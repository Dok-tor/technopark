// units/passengers.cpp
#include "passengers.hpp"
#include <cstdint>

// Определения методов класса Passenger
Passenger::Passenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage)
        : Unit(), handLuggageWeights(handLuggage), luggageWeights(luggage) {}

int Passenger::getTotalHandLuggageWeight() const {
    int total = 0;
    for (int weight : handLuggageWeights)
        total += weight;
    return total;
}

int Passenger::getTotalLuggageWeight() const {
    int total = 0;
    for (int weight : luggageWeights)
        total += weight;
    return total;
}

const std::vector<int>& Passenger::getHandLuggageWeights() const {
    return handLuggageWeights;
}

const std::vector<int>& Passenger::getLuggageWeights() const {
    return luggageWeights;
}

// Определения методов класса EconomyPassenger
EconomyPassenger::EconomyPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage)
        : Passenger(handLuggage, luggage) {}

std::string EconomyPassenger::getType() const {
    return "ECONOMY";
}

int EconomyPassenger::getMaxHandLuggagePieces() const {
    return 1;
}

int EconomyPassenger::getMaxHandLuggageTotalWeight() const {
    return 10;
}

int EconomyPassenger::getMaxLuggagePieces() const {
    return 1;
}

int EconomyPassenger::getMaxLuggageTotalWeight() const {
    return 24;
}

// Определения методов класса BusinessPassenger
BusinessPassenger::BusinessPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage)
        : Passenger(handLuggage, luggage) {}

std::string BusinessPassenger::getType() const {
    return "BUSINESS";
}

int BusinessPassenger::getMaxHandLuggagePieces() const {
    return 2;
}

int BusinessPassenger::getMaxHandLuggageTotalWeight() const {
    return 24;
}

int BusinessPassenger::getMaxLuggagePieces() const {
    return 2;
}

int BusinessPassenger::getMaxLuggageTotalWeight() const {
    return 40;
}

// Определения методов класса FirstClassPassenger
FirstClassPassenger::FirstClassPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage)
        : Passenger(handLuggage, luggage) {}

std::string FirstClassPassenger::getType() const {
    return "FIRST_CLASS";
}

int FirstClassPassenger::getMaxHandLuggagePieces() const {
    return 2;
}

int FirstClassPassenger::getMaxHandLuggageTotalWeight() const {
    return 60;
}

int FirstClassPassenger::getMaxLuggagePieces() const {
    return 2;
}

int FirstClassPassenger::getMaxLuggageTotalWeight() const {
    return INT32_MAX; // Без ограничения по весу
}
