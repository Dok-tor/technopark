//#include <iostream>
//#include <string>
//#include <vector>
//#include <memory>
//#include <map>
//#include <algorithm>
//#include <sstream>
//
//static int global_id = 0;
//
//class Unit {
//protected:
//    int id;
//public:
//    Unit() : id(++global_id) {}
//    virtual ~Unit() = default;
//
//    int getId() const { return id; }
//
//    virtual std::string getType() const = 0;
//    virtual int getTotalHandLuggageWeight() const = 0;
//    virtual int getTotalLuggageWeight() const = 0;
//};
//
//
//class CrewMember : public Unit {
//public:
//    std::string getType() const override {
//        return "CREW_MEMBER";
//    }
//
//    int getTotalHandLuggageWeight() const override {
//        return 0;
//    }
//
//    int getTotalLuggageWeight() const override {
//        return 0;
//    }
//};
//
//class Pilot : public CrewMember {
//public:
//    std::string getType() const override {
//        return "PILOT";
//    }
//};
//
//class FlightAttendant : public CrewMember {
//public:
//    std::string getType() const override {
//        return "FLIGHT_ATTENDANT";
//    }
//};
//
//
//class Passenger : public Unit {
//protected:
//    std::vector<int> handLuggageWeights;
//    std::vector<int> luggageWeights;
//public:
//    Passenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage)
//            : handLuggageWeights(handLuggage), luggageWeights(luggage) {}
//
//    int getTotalHandLuggageWeight() const override {
//        int total = 0;
//        for (int weight : handLuggageWeights)
//            total += weight;
//        return total;
//    }
//
//    int getTotalLuggageWeight() const override {
//        int total = 0;
//        for (int weight : luggageWeights)
//            total += weight;
//        return total;
//    }
//
//    const std::vector<int>& getHandLuggageWeights() const {
//        return handLuggageWeights;
//    }
//
//    const std::vector<int>& getLuggageWeights() const {
//        return luggageWeights;
//    }
//
//    virtual int getMaxHandLuggagePieces() const = 0;
//    virtual int getMaxHandLuggageTotalWeight() const = 0;
//    virtual int getMaxLuggagePieces() const = 0;
//    virtual int getMaxLuggageTotalWeight() const = 0;
//};
//
//
//class EconomyPassenger : public Passenger {
//public:
//    EconomyPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage)
//            : Passenger(handLuggage, luggage) {}
//
//    std::string getType() const override {
//        return "ECONOMY";
//    }
//
//    int getMaxHandLuggagePieces() const override {
//        return 1;
//    }
//
//    int getMaxHandLuggageTotalWeight() const override {
//        return 10;
//    }
//
//    int getMaxLuggagePieces() const override {
//        return 1;
//    }
//
//    int getMaxLuggageTotalWeight() const override {
//        return 24;
//    }
//};
//
//class BusinessPassenger : public Passenger {
//public:
//    BusinessPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage)
//            : Passenger(handLuggage, luggage) {}
//
//    std::string getType() const override {
//        return "BUSINESS";
//    }
//
//    int getMaxHandLuggagePieces() const override {
//        return 2;
//    }
//
//    int getMaxHandLuggageTotalWeight() const override {
//        return 24;
//    }
//
//    int getMaxLuggagePieces() const override {
//        return 2;
//    }
//
//    int getMaxLuggageTotalWeight() const override {
//        return 40;
//    }
//};
//
//class FirstClassPassenger : public Passenger {
//public:
//    FirstClassPassenger(const std::vector<int>& handLuggage, const std::vector<int>& luggage)
//            : Passenger(handLuggage, luggage) {}
//
//    std::string getType() const override {
//        return "FIRST_CLASS";
//    }
//
//    int getMaxHandLuggagePieces() const override {
//        return 2;
//    }
//
//    int getMaxHandLuggageTotalWeight() const override {
//        return 60;
//    }
//
//    int getMaxLuggagePieces() const override {
//        return 2;
//    }
//
//    int getMaxLuggageTotalWeight() const override {
//        return INT32_MAX; // Без ограничения по весу
//    }
//};
//
//
//class Segment {
//private:
//    std::string name;
//    int maxWeight;
//    int currentWeight;
//    int maxPassengers;
//    int currentPassengers;
//    std::vector<std::shared_ptr<Unit>> units;
//
//    // Хранение списка весов отдельных единиц багажа
//    std::vector<int> checkedBaggageWeights;
//
//public:
//    Segment(const std::string& name, int maxWeight, int maxPassengers)
//            : name(name), maxWeight(maxWeight), currentWeight(0), maxPassengers(maxPassengers), currentPassengers(0) {}
//
//    const std::string& getName() const {
//        return name;
//    }
//
//    int getMaxWeight() const {
//        return maxWeight;
//    }
//
//    int getCurrentWeight() const {
//        return currentWeight;
//    }
//
//    int getMaxPassengers() const {
//        return maxPassengers;
//    }
//
//    int getCurrentPassengers() const {
//        return currentPassengers;
//    }
//
//    bool addUnit(const std::shared_ptr<Unit>& unit) {
//        if (currentPassengers >= maxPassengers) {
//            return false;
//        }
//        units.push_back(unit);
//        currentPassengers++;
//        return true;
//    }
//
//    void removeUnit(const std::shared_ptr<Unit>& unit) {
//        auto it = std::find(units.begin(), units.end(), unit);
//        if (it != units.end()) {
//            units.erase(it);
//            currentPassengers--;
//        }
//    }
//
//    // Методы для управления весом ручной клади и багажа
//    void addHandLuggageWeight(int weight) {
//        currentWeight += weight;
//    }
//
//    void removeHandLuggageWeight(int weight) {
//        currentWeight -= weight;
//    }
//
//    bool canAccommodateLuggage(int luggageWeight) const {
//        return (currentWeight + luggageWeight <= maxWeight);
//    }
//
//    void addCheckedBaggageWeight(int weight) {
//        checkedBaggageWeights.push_back(weight);
//        currentWeight += weight;
//    }
//
//    void removeCheckedBaggageWeight(int weight) {
//        auto it = std::find(checkedBaggageWeights.begin(), checkedBaggageWeights.end(), weight);
//        if (it != checkedBaggageWeights.end()) {
//            checkedBaggageWeights.erase(it);
//            currentWeight -= weight;
//        }
//    }
//
//    const std::vector<int>& getCheckedBaggageWeights() const {
//        return checkedBaggageWeights;
//    }
//};
//
//
//class Plane {
//private:
//    std::map<std::string, std::shared_ptr<Segment>> segments;
//    std::vector<std::shared_ptr<Unit>> crewMembers;
//
//public:
//    Plane() {}
//
//    void addSegment(const std::string& name, int maxWeight, int maxPassengers) {
//        segments[name] = std::make_shared<Segment>(name, maxWeight, maxPassengers);
//    }
//
//    bool addCrewMember(const std::shared_ptr<Unit>& crewMember) {
//        crewMembers.push_back(crewMember);
//        return true;
//    }
//
//    bool removeLuggageFromEconomy(const std::shared_ptr<Segment>& economySegment, int requiredWeight) {
//        int currentWeight = economySegment->getCurrentWeight();
//        int maxWeight = economySegment->getMaxWeight();
//        int availableCapacity = maxWeight - currentWeight;
//
//        if (availableCapacity >= requiredWeight) {
//            // Достаточно места, удалять багаж не нужно
//            return true;
//        }
//
//        int additionalSpaceNeeded = requiredWeight - availableCapacity;
//
//        const auto& baggageWeights = economySegment->getCheckedBaggageWeights();
//
//        // Сортируем багаж по возрастанию веса
//        std::vector<int> sortedBaggage = baggageWeights;
//        std::sort(sortedBaggage.begin(), sortedBaggage.end());
//
//        int cumulativeRemovedWeight = 0;
//        std::vector<int> baggageToRemove;
//
//        for (int weight : sortedBaggage) {
//            baggageToRemove.push_back(weight);
//            cumulativeRemovedWeight += weight;
//            availableCapacity += weight;
//
//            if (availableCapacity >= requiredWeight) {
//                break;
//            }
//        }
//
//        if (availableCapacity >= requiredWeight) {
//            // Удаляем выбранный багаж из сегмента
//            for (int weight : baggageToRemove) {
//                economySegment->removeCheckedBaggageWeight(weight);
//            }
//            return true;
//        } else {
//            // Не удалось освободить достаточное место
//            return false;
//        }
//    }
//
//
//    bool addPassenger(const std::shared_ptr<Passenger>& passenger) {
//        std::string type = passenger->getType();
//        auto segment = segments[type];
//        if (!segment) {
//            std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
//            return false;
//        }
//
//        if (segment->getCurrentPassengers() >= segment->getMaxPassengers()) {
//            std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
//            return false;
//        }
//
//        // Проверка количества ручной клади
//        if (passenger->getHandLuggageWeights().size() > passenger->getMaxHandLuggagePieces()) {
//            std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
//            return false;
//        }
//
//        // Проверка суммарного веса ручной клади
//        int totalHandLuggageWeight = passenger->getTotalHandLuggageWeight();
//        if (totalHandLuggageWeight > passenger->getMaxHandLuggageTotalWeight()) {
//            std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
//            return false;
//        }
//
//        // Проверка количества багажа
//        if (passenger->getLuggageWeights().size() > passenger->getMaxLuggagePieces()) {
//            std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
//            return false;
//        }
//
//        // Проверка суммарного веса багажа
//        int totalLuggageWeight = passenger->getTotalLuggageWeight();
//        if (totalLuggageWeight > passenger->getMaxLuggageTotalWeight()) {
//            std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
//            return false;
//        }
//
//        // Добавляем пассажира в свой сегмент
//        if (!segment->addUnit(passenger)) {
//            std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
//            return false;
//        }
//
//        // Добавляем вес ручной клади к текущему весу сегмента
//        segment->addHandLuggageWeight(totalHandLuggageWeight);
//
//        // Обработка багажа
//        if (totalLuggageWeight == 0) {
//            // Нет багажа, обработка завершена
//            return true;
//        }
//
//        // Попытка разместить багаж в своём сегменте
//        if (segment->canAccommodateLuggage(totalLuggageWeight)) {
//            segment->addCheckedBaggageWeight(totalLuggageWeight);
//        } else if (type == "FIRST_CLASS" || type == "BUSINESS") {
//            // Пытаемся разместить багаж в эконом-классе
//            auto economySegment = segments["ECONOMY"];
//            if (economySegment->canAccommodateLuggage(totalLuggageWeight)) {
//                economySegment->addCheckedBaggageWeight(totalLuggageWeight);
//            } else {
//                // Необходимо снять багаж из эконом-класса
//                if (removeLuggageFromEconomy(economySegment, totalLuggageWeight)) {
//                    economySegment->addCheckedBaggageWeight(totalLuggageWeight);
//                    std::cout << "!!PASSENGER'S LUGGAGE REMOVED FROM FLIGHT, ID = " << passenger->getId() << "!!\n";
//                } else {
//                    // Не удалось освободить место
//                    // Пассажир летит без багажа
//                    std::cout << "!!PASSENGER'S LUGGAGE REMOVED FROM FLIGHT, ID = " << passenger->getId() << "!!\n";
//                }
//            }
//        } else if (type == "ECONOMY") {
//            // Багаж снимается с рейса
//            std::cout << "!!PASSENGER'S LUGGAGE REMOVED FROM FLIGHT, ID = " << passenger->getId() << "!!\n";
//            // Пассажир летит без багажа
//        } else {
//            // Другие типы (на случай расширения)
//            std::cout << "!!CANT REGISTER " << type << " PASSENGER, ID = " << passenger->getId() << "!!\n";
//            return false;
//        }
//
//        return true;
//    }
//
//    void printLoadInfo() const {
//        for (const auto& [name, segment] : segments) {
//            std::cout << "Segment: " << name << ", Current Weight: " << segment->getCurrentWeight() << std::endl;
//            std::cout << "/" << segment->getMaxWeight() << ", Passengers: "
//                      << segment->getCurrentPassengers() << "/" << segment->getMaxPassengers() << "\n";
//        }
//    }
//};



//int main() {
//    Plane plane;
//
//    std::string line;
//    // Чтение первых строк для инициализации сегментов и экипажа
//    for (int i = 0; i < 11; ++i) {
//        std::getline(std::cin, line);
//        std::istringstream iss(line);
//        std::string type;
//        iss >> type;
//        if (type == "FIRST_CLASS_SEGMENT" || type == "BUSINESS_CLASS_SEGMENT" || type == "ECONOMY_CLASS_SEGMENT") {
//            int maxWeight;
//            iss >> maxWeight;
//            int maxPassengers = 0;
//            if (type == "FIRST_CLASS_SEGMENT")
//            {
//                maxPassengers = 4;
//                type = "FIRST_CLASS";
//            }
//            else if (type == "BUSINESS_CLASS_SEGMENT")
//            {
//                maxPassengers = 10;
//                type = "BUSINESS";
//            }
//            else if (type == "ECONOMY_CLASS_SEGMENT")
//            {
//                maxPassengers = 200;
//                type = "ECONOMY";
//            }
//            plane.addSegment(type, maxWeight, maxPassengers);
//        } else if (type == "PILOT") {
//            auto pilot = std::make_shared<Pilot>();
//            plane.addCrewMember(pilot);
//        } else if (type == "FLIGHT_ATTENDANT") {
//            auto attendant = std::make_shared<FlightAttendant>();
//            plane.addCrewMember(attendant);
//        }
//    }
//    while (std::getline(std::cin, line)) {
//        std::istringstream iss(line);
//        std::string type;
//        iss >> type;
//
//        std::vector<int> handLuggage;
//        std::vector<int> luggage;
//        int weight;
//
//        if (type == "ECONOMY") {
//            // Для ECONOMY: 1 ручная кладь и 1 багаж
//            if (iss >> weight) handLuggage.push_back(weight); // Ручная кладь
//            if (iss >> weight) luggage.push_back(weight);     // Багаж
//        } else if (type == "BUSINESS") {
//            // Для BUSINESS: 2 ручные клади и 1 багаж
//            for (int i = 0; i < 2 && iss >> weight; ++i) {
//                handLuggage.push_back(weight); // Ручная кладь
//            }
//            if (iss >> weight) luggage.push_back(weight);     // Багаж
//        } else if (type == "FIRST_CLASS") {
//            // Для FIRST_CLASS: 2 ручные клади и 2 багажа
//            for (int i = 0; i < 2 && iss >> weight; ++i) {
//                handLuggage.push_back(weight); // Ручная кладь
//            }
//            for (int i = 0; i < 2 && iss >> weight; ++i) {
//                luggage.push_back(weight);     // Багаж
//            }
//        } else if (type == "STAT"){
//            plane.printLoadInfo();
//            // Вывести статистику
//            continue;
//        } else {
//            // Неизвестный тип пассажира, пропускаем
//            continue;
//        }
//
//        // Создаём пассажира на основе типа
//        std::shared_ptr<Passenger> passenger;
//        if (type == "ECONOMY") {
//            passenger = std::make_shared<EconomyPassenger>(handLuggage, luggage);
//        } else if (type == "BUSINESS") {
//            passenger = std::make_shared<BusinessPassenger>(handLuggage, luggage);
//        } else if (type == "FIRST_CLASS") {
//            passenger = std::make_shared<FirstClassPassenger>(handLuggage, luggage);
//        }
//
//        // Добавляем пассажира в самолёт
//        if (passenger) {
//            plane.addPassenger(passenger);
//            plane.printLoadInfo();
//        }
//    }
//
//
//    // Вывод информации о загрузке
//    plane.printLoadInfo();
//
//    return 0;
//}



// main.cpp
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>

#include "units/plane.hpp"

int main() {
    Plane plane;

    std::string line;
    // Чтение первых строк для инициализации сегментов и экипажа
    for (int i = 0; i < 11; ++i) {
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "FIRST_CLASS_SEGMENT" || type == "BUSINESS_CLASS_SEGMENT" || type == "ECONOMY_CLASS_SEGMENT") {
            int maxWeight;
            iss >> maxWeight;
            int maxPassengers = 0;
            if (type == "FIRST_CLASS_SEGMENT") {
                maxPassengers = 4;
                type = "FIRST_CLASS";
            } else if (type == "BUSINESS_CLASS_SEGMENT") {
                maxPassengers = 10;
                type = "BUSINESS";
            } else if (type == "ECONOMY_CLASS_SEGMENT") {
                maxPassengers = 200;
                type = "ECONOMY";
            }
            plane.addSegment(type, maxWeight, maxPassengers);
        } else if (type == "PILOT") {
            auto pilot = std::make_shared<Pilot>();
            plane.addCrewMember(pilot);
        } else if (type == "FLIGHT_ATTENDANT") {
            auto attendant = std::make_shared<FlightAttendant>();
            plane.addCrewMember(attendant);
        }
    }
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        std::vector<int> handLuggage;
        std::vector<int> luggage;
        int weight;

        if (type == "ECONOMY") {
            // Для ECONOMY: 1 ручная кладь и 1 багаж
            if (iss >> weight) handLuggage.push_back(weight); // Ручная кладь
            if (iss >> weight) luggage.push_back(weight);     // Багаж
        } else if (type == "BUSINESS") {
            // Для BUSINESS: 2 ручные клади и 1 багаж
            for (int i = 0; i < 2 && iss >> weight; ++i) {
                handLuggage.push_back(weight); // Ручная кладь
            }
            if (iss >> weight) luggage.push_back(weight);     // Багаж
        } else if (type == "FIRST_CLASS") {
            // Для FIRST_CLASS: 2 ручные клади и 2 багажа
            for (int i = 0; i < 2 && iss >> weight; ++i) {
                handLuggage.push_back(weight); // Ручная кладь
            }
            for (int i = 0; i < 2 && iss >> weight; ++i) {
                luggage.push_back(weight);     // Багаж
            }
        } else if (type == "STAT") {
            plane.printLoadInfo();
            // Вывести статистику
            continue;
        } else {
            // Неизвестный тип пассажира, пропускаем
            continue;
        }

        // Создаём пассажира на основе типа
        std::shared_ptr<Passenger> passenger;
        if (type == "ECONOMY") {
            passenger = std::make_shared<EconomyPassenger>(handLuggage, luggage);
        } else if (type == "BUSINESS") {
            passenger = std::make_shared<BusinessPassenger>(handLuggage, luggage);
        } else if (type == "FIRST_CLASS") {
            passenger = std::make_shared<FirstClassPassenger>(handLuggage, luggage);
        }

        // Добавляем пассажира в самолёт
        if (passenger) {
            plane.addPassenger(passenger);
            plane.printLoadInfo();
        }
    }

    // Вывод информации о загрузке
    plane.printLoadInfo();

    return 0;
}