// main.cpp
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "units/plane.hpp"

int main()
{
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
            if (iss >> weight)
                handLuggage.push_back(weight); // Ручная кладь
            if (iss >> weight)
                luggage.push_back(weight); // Багаж
        } else if (type == "BUSINESS") {
            // Для BUSINESS: 2 ручные клади и 1 багаж
            for (int i = 0; i < 2 && iss >> weight; ++i) {
                handLuggage.push_back(weight); // Ручная кладь
            }
            if (iss >> weight)
                luggage.push_back(weight); // Багаж
        } else if (type == "FIRST_CLASS") {
            // Для FIRST_CLASS: 2 ручные клади и 2 багажа
            for (int i = 0; i < 2 && iss >> weight; ++i) {
                handLuggage.push_back(weight); // Ручная кладь
            }
            for (int i = 0; i < 2 && iss >> weight; ++i) {
                luggage.push_back(weight); // Багаж
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
            passenger = std::make_shared<EconomyPassenger>(std::move(handLuggage), std::move(luggage));
        } else if (type == "BUSINESS") {
            passenger = std::make_shared<BusinessPassenger>(std::move(handLuggage), std::move(luggage));
        } else if (type == "FIRST_CLASS") {
            passenger = std::make_shared<FirstClassPassenger>(std::move(handLuggage), std::move(luggage));
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