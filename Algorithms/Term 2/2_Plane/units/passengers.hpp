// units/passengers.hpp
#ifndef PASSENGERS_HPP
#define PASSENGERS_HPP

#include "unit.hpp"
#include <vector>

class Passenger : public Unit {
protected:
    std::vector<int> handLuggageWeights;
    std::vector<int> luggageWeights;
public:
    Passenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage);

    int getTotalHandLuggageWeight() const override;
    int getTotalLuggageWeight() const override;

    const std::vector<int>& getHandLuggageWeights() const;
    const std::vector<int>& getLuggageWeights() const;

    virtual int getMaxHandLuggagePieces() const = 0;
    virtual int getMaxHandLuggageTotalWeight() const = 0;
    virtual int getMaxLuggagePieces() const = 0;
    virtual int getMaxLuggageTotalWeight() const = 0;
};

class EconomyPassenger : public Passenger {
public:
    EconomyPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage);
    std::string getType() const override;

    int getMaxHandLuggagePieces() const override;
    int getMaxHandLuggageTotalWeight() const override;
    int getMaxLuggagePieces() const override;
    int getMaxLuggageTotalWeight() const override;
};

class BusinessPassenger : public Passenger {
public:
    BusinessPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage);
    std::string getType() const override;

    int getMaxHandLuggagePieces() const override;
    int getMaxHandLuggageTotalWeight() const override;
    int getMaxLuggagePieces() const override;
    int getMaxLuggageTotalWeight() const override;
};

class FirstClassPassenger : public Passenger {
public:
    FirstClassPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage);
    std::string getType() const override;

    int getMaxHandLuggagePieces() const override;
    int getMaxHandLuggageTotalWeight() const override;
    int getMaxLuggagePieces() const override;
    int getMaxLuggageTotalWeight() const override;
};

#endif // PASSENGERS_HPP
