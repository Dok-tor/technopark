// units/plane.hpp
#ifndef PLANE_HPP
#define PLANE_HPP

#include "crew_members.hpp"
#include "passengers.hpp"
#include "unit.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

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
  Segment(std::string name, int maxWeight, int maxPassengers);

  const std::string &getName() const;

  int getMaxWeight() const;
  int getCurrentWeight() const;
  int getMaxPassengers() const;
  int getCurrentPassengers() const;

  bool addUnit(std::shared_ptr<Unit> unit);
  void removeUnit(const std::shared_ptr<Unit> &unit);

  void addHandLuggageWeight(int weight);
  void removeHandLuggageWeight(int weight);

  bool canAccommodateLuggage(int luggageWeight) const;

  void addCheckedBaggageWeight(int weight);
  void removeCheckedBaggageWeight(int weight);

  const std::vector<int> &getCheckedBaggageWeights() const;
};

class Plane {
private:
  std::map<std::string, std::shared_ptr<Segment>> segments;
  std::vector<std::shared_ptr<Unit>> crewMembers;

public:
  Plane();

  void addSegment(std::string name, int maxWeight, int maxPassengers);

  bool addCrewMember(const std::shared_ptr<Unit> &crewMember);

  static bool
  removeLuggageFromEconomy(const std::shared_ptr<Segment> &economySegment,
                           int requiredWeight);

  bool addPassenger(const std::shared_ptr<Passenger> &passenger);

  std::shared_ptr<Segment> getSegment(const std::string &name)
      const; // Для тестов (чтобы вытаскивать из private)

  void
  printLoadInfo() const; // Отладочная функция для проверки состояния самолёта
};

#endif // PLANE_HPP
