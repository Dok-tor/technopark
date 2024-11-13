// units/plane.hpp
#ifndef PLANE_HPP
#define PLANE_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "unit.hpp"
#include "passengers.hpp"
#include "crew_members.hpp"

class Segment {
private:
    std::string name;
    int maxWeight;
    int currentWeight;
    int maxPassengers;
    int currentPassengers;
    std::vector<std::shared_ptr<Unit>> units;

    // Хранение списка весов отдельных единиц багажа
    std::vector<int> checkedBaggageWeights;

public:
    Segment(const std::string& name, int maxWeight, int maxPassengers);

    const std::string& getName() const;

    int getMaxWeight() const;
    int getCurrentWeight() const;
    int getMaxPassengers() const;
    int getCurrentPassengers() const;

    bool addUnit(const std::shared_ptr<Unit>& unit);
    void removeUnit(const std::shared_ptr<Unit>& unit);

    // Методы для управления весом ручной клади и багажа
    void addHandLuggageWeight(int weight);
    void removeHandLuggageWeight(int weight);

    bool canAccommodateLuggage(int luggageWeight) const;

    void addCheckedBaggageWeight(int weight);
    void removeCheckedBaggageWeight(int weight);

    const std::vector<int>& getCheckedBaggageWeights() const;
};

class Plane {
private:
    std::map<std::string, std::shared_ptr<Segment>> segments;
    std::vector<std::shared_ptr<Unit>> crewMembers;

public:
    Plane();

    void addSegment(const std::string& name, int maxWeight, int maxPassengers);

    bool addCrewMember(const std::shared_ptr<Unit>& crewMember);

    bool removeLuggageFromEconomy(const std::shared_ptr<Segment>& economySegment, int requiredWeight);

    bool addPassenger(const std::shared_ptr<Passenger>& passenger);
    // Для тестов
    std::shared_ptr<Segment> getSegment(const std::string& name) const;

    void printLoadInfo() const;
};

#endif // PLANE_HPP
