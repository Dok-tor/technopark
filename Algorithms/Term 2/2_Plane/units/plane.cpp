// units/plane.cpp
#include "plane.hpp"
#include <iostream>
#include <algorithm>

// Определения методов класса Segment
Segment::Segment(const std::string& name, int maxWeight, int maxPassengers)
        : name(name), maxWeight(maxWeight), currentWeight(0), maxPassengers(maxPassengers), currentPassengers(0) {}

const std::string& Segment::getName() const {
    return name;
}

int Segment::getMaxWeight() const {
    return maxWeight;
}

int Segment::getCurrentWeight() const {
    return currentWeight;
}

int Segment::getMaxPassengers() const {
    return maxPassengers;
}

int Segment::getCurrentPassengers() const {
    return currentPassengers;
}

bool Segment::addUnit(const std::shared_ptr<Unit>& unit) {
    if (currentPassengers >= maxPassengers) {
        return false;
    }
    units.push_back(unit);
    currentPassengers++;
    return true;
}

void Segment::removeUnit(const std::shared_ptr<Unit>& unit) {
    auto it = std::find(units.begin(), units.end(), unit);
    if (it != units.end()) {
        units.erase(it);
        currentPassengers--;
    }
}

void Segment::addHandLuggageWeight(int weight) {
    currentWeight += weight;
}

void Segment::removeHandLuggageWeight(int weight) {
    currentWeight -= weight;
}

bool Segment::canAccommodateLuggage(int luggageWeight) const {
    return (currentWeight + luggageWeight <= maxWeight);
}

void Segment::addCheckedBaggageWeight(int weight) {
    checkedBaggageWeights.push_back(weight);
    currentWeight += weight;
}

void Segment::removeCheckedBaggageWeight(int weight) {
    auto it = std::find(checkedBaggageWeights.begin(), checkedBaggageWeights.end(), weight);
    if (it != checkedBaggageWeights.end()) {
        checkedBaggageWeights.erase(it);
        currentWeight -= weight;
    }
}

const std::vector<int>& Segment::getCheckedBaggageWeights() const {
    return checkedBaggageWeights;
}

// Определения методов класса Plane
Plane::Plane() {}

void Plane::addSegment(const std::string& name, int maxWeight, int maxPassengers) {
    segments[name] = std::make_shared<Segment>(name, maxWeight, maxPassengers);
}

bool Plane::addCrewMember(const std::shared_ptr<Unit>& crewMember) {
    crewMembers.push_back(crewMember);
    return true;
}

bool Plane::removeLuggageFromEconomy(const std::shared_ptr<Segment>& economySegment, int requiredWeight) {
    int currentWeight = economySegment->getCurrentWeight();
    int maxWeight = economySegment->getMaxWeight();
    int availableCapacity = maxWeight - currentWeight;

    if (availableCapacity >= requiredWeight) {
        // Достаточно места, удалять багаж не нужно
        return true;
    }


    const auto& baggageWeights = economySegment->getCheckedBaggageWeights();

    // Сортируем багаж по возрастанию веса
    std::vector<int> sortedBaggage = baggageWeights;
    std::sort(sortedBaggage.begin(), sortedBaggage.end());

    int cumulativeRemovedWeight = 0;
    std::vector<int> baggageToRemove;

    for (int weight : sortedBaggage) {
        baggageToRemove.push_back(weight);
        cumulativeRemovedWeight += weight;
        availableCapacity += weight;

        if (availableCapacity >= requiredWeight) {
            break;
        }
    }

    if (availableCapacity >= requiredWeight) {
        // Удаляем выбранный багаж из сегмента
        for (int weight : baggageToRemove) {
            economySegment->removeCheckedBaggageWeight(weight);
        }
        return true;
    } else {
        // Не удалось освободить достаточное место
        return false;
    }
}

bool Plane::addPassenger(const std::shared_ptr<Passenger>& passenger) {
    std::string type = passenger->getType();
    auto segmentIt = segments.find(type);
    if (segmentIt == segments.end()) {
        std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
        return false;
    }
    auto segment = segmentIt->second;

    if (segment->getCurrentPassengers() >= segment->getMaxPassengers()) {
        std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
        return false;
    }

    // Проверка количества ручной клади
    if (static_cast<int>(passenger->getHandLuggageWeights().size()) > passenger->getMaxHandLuggagePieces()) {
        std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
        return false;
    }

    // Проверка суммарного веса ручной клади
    int totalHandLuggageWeight = passenger->getTotalHandLuggageWeight();
    if (totalHandLuggageWeight > passenger->getMaxHandLuggageTotalWeight()) {
        std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
        return false;
    }

    // Проверка количества багажа
    if (static_cast<int>(passenger->getLuggageWeights().size()) > passenger->getMaxLuggagePieces()) {
        std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
        return false;
    }

    // Проверка суммарного веса багажа
    int totalLuggageWeight = passenger->getTotalLuggageWeight();
    if (totalLuggageWeight > passenger->getMaxLuggageTotalWeight()) {
        std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
        return false;
    }

    // Добавляем пассажира в свой сегмент
    if (!segment->addUnit(passenger)) {
        std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
        return false;
    }

    // Добавляем вес ручной клади к текущему весу сегмента
    segment->addHandLuggageWeight(totalHandLuggageWeight);

    // Обработка багажа
    if (totalLuggageWeight == 0) {
        // Нет багажа, обработка завершена
        return true;
    }

    // Попытка разместить багаж в своём сегменте
    if (segment->canAccommodateLuggage(totalLuggageWeight)) {
        segment->addCheckedBaggageWeight(totalLuggageWeight);
        return true;
    } else if (type == "FIRST_CLASS" || type == "BUSINESS") {
        // Пытаемся разместить багаж в эконом-классе
        auto economySegmentIt = segments.find("ECONOMY");
        if (economySegmentIt == segments.end()) {
            // Сегмент эконом-класса не найден, багаж снимается с рейса
            std::cout << "!!PASSENGER'S LUGGAGE REMOVED FROM FLIGHT, ID = " << passenger->getId() << "!!\n";
            return true; // Пассажир летит без багажа
        }
        auto economySegment = economySegmentIt->second;
        if (economySegment->canAccommodateLuggage(totalLuggageWeight)) {
            economySegment->addCheckedBaggageWeight(totalLuggageWeight);
            return true;
        } else {
            // Необходимо снять багаж из эконом-класса
            if (removeLuggageFromEconomy(economySegment, totalLuggageWeight)) {
                economySegment->addCheckedBaggageWeight(totalLuggageWeight);
                return true;
            } else {
                // Не удалось освободить место
                std::cout << "!!PASSENGER'S LUGGAGE REMOVED FROM FLIGHT, ID = " << passenger->getId() << "!!\n";
                return true; // Пассажир летит без багажа
            }
        }
    } else if (type == "ECONOMY") {
        // Багаж снимается с рейса
        std::cout << "!!PASSENGER'S LUGGAGE REMOVED FROM FLIGHT, ID = " << passenger->getId() << "!!\n";
        return true; // Пассажир летит без багажа
    } else {
        // Другие типы
        std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
        return false;
    }
}

void Plane::printLoadInfo() const {
    for (const auto& pair : segments) {
        const auto& name = pair.first;
        const auto& segment = pair.second;
        std::cout << "Segment: " << name << ", Current Weight: " << segment->getCurrentWeight()
                  << "/" << segment->getMaxWeight() << ", Passengers: "
                  << segment->getCurrentPassengers() << "/" << segment->getMaxPassengers() << "\n";
    }
}

std::shared_ptr<Segment> Plane::getSegment(const std::string& name) const {
    auto it = segments.find(name);
    if (it != segments.end()) {
        return it->second;
    }
    return nullptr;
}
