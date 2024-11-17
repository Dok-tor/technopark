// tests/test_plane.cpp
#include "../units/crew_members.hpp"
#include "../units/passengers.hpp"
#include "../units/plane.hpp"
#include <cassert>
#include <iostream>
#include <memory>

// Класс для тестирования пассажира с неизвестным типом
class UnknownPassenger : public Passenger {
public:
    UnknownPassenger(const std::vector<int>& handLuggageWeights, const std::vector<int>& luggageWeights)
            : Passenger(handLuggageWeights, luggageWeights)
    {
    }

    std::string getType() const override
    {
        return "UNKNOWN";
    }

    int getMaxHandLuggagePieces() const override { return 1; }
    int getMaxHandLuggageTotalWeight() const override { return 10; }
    int getMaxLuggagePieces() const override { return 1; }
    int getMaxLuggageTotalWeight() const override { return 20; }
};

void testSegmentMethods()
{
    // Создаём сегмент
    Segment segment("TEST_SEGMENT", 100, 2);

    // Тестируем getName()
    assert(segment.getName() == "TEST_SEGMENT");

    // Тестируем addUnit() и removeUnit()
    auto passenger = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(segment.addUnit(passenger));
    assert(segment.getCurrentPassengers() == 1);
    segment.removeUnit(passenger);
    assert(segment.getCurrentPassengers() == 0);

    // Тестируем addHandLuggageWeight() и removeHandLuggageWeight()
    segment.addHandLuggageWeight(5);
    assert(segment.getCurrentWeight() == 5);
    segment.removeHandLuggageWeight(5);
    assert(segment.getCurrentWeight() == 0);

    // Тестируем addCheckedBaggageWeight() и removeCheckedBaggageWeight()
    segment.addCheckedBaggageWeight(10);
    assert(segment.getCurrentWeight() == 10);
    segment.removeCheckedBaggageWeight(10);
    assert(segment.getCurrentWeight() == 0);

    // Тестируем getCheckedBaggageWeights()
    segment.addCheckedBaggageWeight(15);
    const auto& weights = segment.getCheckedBaggageWeights();
    assert(weights.size() == 1 && weights[0] == 15);

    std::cout << "Segment methods tests passed!\n";
}

void testSegmentOverflow()
{
    // Создаём сегмент с максимум 1 пассажиром
    Segment segment("TEST_SEGMENT", 100, 1);

    // Добавляем первого пассажира
    auto passenger1 = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(segment.addUnit(passenger1));

    // Попытка добавить второго пассажира должна вернуть false
    auto passenger2 = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(!segment.addUnit(passenger2));

    std::cout << "Segment overflow test passed!\n";
}

void testRemoveLuggageFromEconomy()
{
    Plane plane;

    // Добавляем сегмент эконом-класса с ограниченным весом
    plane.addSegment("ECONOMY", 50, 5);

    // Заполняем эконом-класс багажом
    auto economySegment = plane.getSegment("ECONOMY");
    economySegment->addCheckedBaggageWeight(45);

    // Добавляем сегмент бизнес-класса с меньшим максимальным весом
    plane.addSegment("BUSINESS", 10, 2); // Изменено с 20 на 10

    // Добавляем пассажира бизнес-класса с багажом, который не помещается
    auto passenger = std::make_shared<BusinessPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });

    assert(plane.addPassenger(passenger));

    // Проверяем, что багаж пассажира был размещён после удаления багажа из эконом-класса
    assert(economySegment->getCurrentWeight() == 10); // Вес багажа пассажира

    std::cout << "Remove luggage from economy success test passed!\n";
}

void testAddPassengerFailures()
{
    Plane plane;

    // Добавляем сегмент эконом-класса с 1 местом
    plane.addSegment("ECONOMY", 100, 1);

    // Добавляем первого пассажира успешно
    auto passenger1 = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(plane.addPassenger(passenger1));

    // Попытаемся добавить второго пассажира в переполненный сегмент
    auto passenger2 = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(!plane.addPassenger(passenger2));

    // Попытаемся добавить пассажира в несуществующий сегмент
    auto passenger3 = std::make_shared<FirstClassPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(!plane.addPassenger(passenger3));

    // Попытаемся добавить пассажира с превышением количества ручной клади
    auto passenger4 = std::make_shared<EconomyPassenger>(std::vector<int> { 5, 5 }, std::vector<int> { 10 });
    assert(!plane.addPassenger(passenger4));

    // Попытаемся добавить пассажира с превышением веса ручной клади
    auto passenger5 = std::make_shared<EconomyPassenger>(std::vector<int> { 15 }, std::vector<int> { 10 });
    assert(!plane.addPassenger(passenger5));

    // Попытаемся добавить пассажира с превышением количества багажа
    auto passenger6 = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 10, 10 });
    assert(!plane.addPassenger(passenger6));

    // Попытаемся добавить пассажира с превышением веса багажа
    auto passenger7 = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 30 });
    assert(!plane.addPassenger(passenger7));

    std::cout << "Add passenger failure tests passed!\n";
}

void testPassengerWithoutLuggage()
{
    Plane plane;
    plane.addSegment("ECONOMY", 100, 2);

    auto passenger = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> {});
    assert(plane.addPassenger(passenger));

    auto economySegment = plane.getSegment("ECONOMY");
    assert(economySegment->getCurrentPassengers() == 1);
    assert(economySegment->getCurrentWeight() == 5); // Только вес ручной клади

    std::cout << "Passenger without luggage test passed!\n";
}

void testBaggageNotAccommodated()
{
    Plane plane;

    // Добавляем сегменты с ограниченным весом
    plane.addSegment("BUSINESS", 20, 2);
    plane.addSegment("ECONOMY", 25, 5);

    // Добавляем пассажира бизнес-класса с очень большим багажом
    auto passenger = std::make_shared<BusinessPassenger>(std::vector<int> { 10 }, std::vector<int> { 30 });

    // Сегмент бизнес-класса не сможет вместить багаж
    assert(plane.addPassenger(passenger));

    // Проверяем, что багаж был снят с рейса
    auto businessSegment = plane.getSegment("BUSINESS");
    auto economySegment = plane.getSegment("ECONOMY");

    // Вес в бизнес-сегменте должен быть равен весу ручной клади
    assert(businessSegment->getCurrentWeight() == passenger->getTotalHandLuggageWeight());
    // Вес в эконом-сегменте должен быть 0, так как багаж не помещается
    assert(economySegment->getCurrentWeight() == 0);

    std::cout << "Baggage not accommodated tests passed!\n";
}

void testRemoveLuggageFromEconomySuccess()
{
    Plane plane;

    // Добавляем сегмент эконом-класса с ограниченным весом
    plane.addSegment("ECONOMY", 50, 5);

    // Заполняем эконом-класс багажом
    auto economySegment = plane.getSegment("ECONOMY");
    economySegment->addCheckedBaggageWeight(45);

    // Добавляем пассажира бизнес-класса с багажом, который не помещается
    plane.addSegment("BUSINESS", 10, 2);
    auto passenger = std::make_shared<BusinessPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });

    assert(plane.addPassenger(passenger));

    // Проверяем, что багаж пассажира был размещён после удаления багажа из эконом-класса
    assert(economySegment->getCurrentWeight() == 10); // Вес багажа пассажира

    std::cout << "Remove luggage from economy success test passed!\n";
}

void testEconomyPassengerBaggageRemoved()
{
    Plane plane;
    plane.addSegment("ECONOMY", 15, 2);

    auto passenger = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 15 });
    assert(plane.addPassenger(passenger));

    auto economySegment = plane.getSegment("ECONOMY");
    // Вес должен быть только от ручной клади
    assert(economySegment->getCurrentWeight() == 5);

    std::cout << "Economy passenger baggage removed test passed!\n";
}

void testUnknownPassenger()
{
    Plane plane;
    plane.addSegment("ECONOMY", 100, 2);

    auto passenger = std::make_shared<UnknownPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(!plane.addPassenger(passenger));

    std::cout << "Unknown passenger test passed!\n";
}

void testGetNonexistentSegment()
{
    Plane plane;
    plane.addSegment("ECONOMY", 100, 2);

    auto segment = plane.getSegment("FIRST_CLASS");
    assert(segment == nullptr);

    std::cout << "Get nonexistent segment test passed!\n";
}

void testPlane()
{
    Plane plane;

    // Добавляем сегменты
    plane.addSegment("ECONOMY", 100, 2);
    plane.addSegment("BUSINESS", 50, 1);

    // Добавляем членов экипажа
    auto pilot = std::make_shared<Pilot>();
    auto attendant = std::make_shared<FlightAttendant>();
    assert(plane.addCrewMember(pilot));
    assert(plane.addCrewMember(attendant));

    // Добавляем пассажиров
    auto economyPassenger = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(plane.addPassenger(economyPassenger));

    auto businessPassenger = std::make_shared<BusinessPassenger>(std::vector<int> { 5 }, std::vector<int> { 20 });
    assert(plane.addPassenger(businessPassenger));

    // Выводим информацию о загрузке
    plane.printLoadInfo();

    std::cout << "Plane tests passed!\n";
}

void testExceedHandLuggagePieces()
{
    Plane plane;
    plane.addSegment("ECONOMY", 100, 2);

    // Пассажир с двумя единицами ручной клади, превышающими максимум (1)
    auto passenger = std::make_shared<EconomyPassenger>(std::vector<int> { 5, 5 }, std::vector<int> { 10 });
    assert(!plane.addPassenger(passenger));

    std::cout << "Exceed hand luggage pieces test passed!\n";
}

void testExceedHandLuggageWeight()
{
    Plane plane;
    plane.addSegment("ECONOMY", 100, 2);

    // Пассажир с ручной кладью весом 15 кг, превышающей максимум (10 кг)
    auto passenger = std::make_shared<EconomyPassenger>(std::vector<int> { 15 }, std::vector<int> { 10 });
    assert(!plane.addPassenger(passenger));

    std::cout << "Exceed hand luggage weight test passed!\n";
}

void testExceedLuggagePieces()
{
    Plane plane;
    plane.addSegment("ECONOMY", 100, 2);

    // Пассажир с двумя единицами багажа, превышающими максимум (1)
    auto passenger = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 10, 10 });
    assert(!plane.addPassenger(passenger));

    std::cout << "Exceed luggage pieces test passed!\n";
}

void testExceedLuggageWeight()
{
    Plane plane;
    plane.addSegment("ECONOMY", 100, 2);

    // Пассажир с багажом весом 30 кг, превышающим максимум (20 кг)
    auto passenger = std::make_shared<EconomyPassenger>(std::vector<int> { 5 }, std::vector<int> { 30 });
    assert(!plane.addPassenger(passenger));

    std::cout << "Exceed luggage weight test passed!\n";
}

void testNoEconomySegment()
{
    Plane plane;

    // Добавляем только сегмент BUSINESS
    plane.addSegment("BUSINESS", 10, 2);

    // Пассажир бизнес-класса с багажом, который не помещается
    auto passenger = std::make_shared<BusinessPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });

    assert(plane.addPassenger(passenger));

    // Проверяем, что багаж был снят с рейса
    auto businessSegment = plane.getSegment("BUSINESS");
    assert(businessSegment->getCurrentWeight() == 5); // Только ручная кладь

    std::cout << "No economy segment test passed!\n";
}

void testPlaceLuggageInEconomy()
{
    Plane plane;

    // Добавляем сегменты
    plane.addSegment("BUSINESS", 10, 2);
    plane.addSegment("ECONOMY", 50, 5);

    // Пассажир бизнес-класса с багажом
    auto passenger = std::make_shared<BusinessPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });

    // Добавляем пассажира
    assert(plane.addPassenger(passenger));

    // Проверяем, что багаж был размещён в эконом-классе
    auto economySegment = plane.getSegment("ECONOMY");
    assert(economySegment->getCurrentWeight() == 10);

    std::cout << "Place luggage in economy test passed!\n";
}

void testUnknownPassengerType()
{
    Plane plane;
    plane.addSegment("ECONOMY", 100, 2);

    // Пассажир с неизвестным типом
    class SpecialPassenger : public Passenger {
    public:
        SpecialPassenger(const std::vector<int>& handLuggageWeights, const std::vector<int>& luggageWeights)
                : Passenger(handLuggageWeights, luggageWeights)
        {
        }

        std::string getType() const override
        {
            return "SPECIAL";
        }

        int getMaxHandLuggagePieces() const override { return 1; }
        int getMaxHandLuggageTotalWeight() const override { return 10; }
        int getMaxLuggagePieces() const override { return 1; }
        int getMaxLuggageTotalWeight() const override { return 20; }
    };

    auto passenger = std::make_shared<SpecialPassenger>(std::vector<int> { 5 }, std::vector<int> { 10 });
    assert(!plane.addPassenger(passenger));

    std::cout << "Unknown passenger type test passed!\n";
}

int main()
{
    testSegmentMethods();
    testSegmentOverflow();
    testRemoveLuggageFromEconomy();
    testAddPassengerFailures();
    testExceedHandLuggagePieces();
    testExceedHandLuggageWeight();
    testExceedLuggagePieces();
    testExceedLuggageWeight();
    testPassengerWithoutLuggage();
    testBaggageNotAccommodated();
    testRemoveLuggageFromEconomySuccess();
    testEconomyPassengerBaggageRemoved();
    testNoEconomySegment();
    testPlaceLuggageInEconomy();
    testUnknownPassengerType();
    testGetNonexistentSegment();
    testPlane();
    return 0;
}
