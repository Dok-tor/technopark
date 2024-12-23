#include "fusion_serializer.hpp"
#include <cassert>

int main() {
    using namespace pkg;

    // Тест 1: Проверяем сериализацию структуры с поддерживаемыми типами
    {
        ValidStruct valid_struct;
        valid_struct.valid_int = 42;
        valid_struct.valid_float = 3.14f;
        valid_struct.valid_str = "Test";
        valid_struct.valid_vec = {1, 2, 3};
        valid_struct.valid_nested.val = 1.618f;

        try {
            std::string jsonStr = Serialize(valid_struct);
            std::cout << "Serialized ValidStruct:\n" << jsonStr << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Test failed: ValidStruct threw an exception: " << e.what() << "\n";
            return 1;
        }
    }

    // // Тест 2: Проверяем, что InvalidStruct1 не компилируется
    // static_assert(!std::is_invocable_v<decltype(&Serialize<InvalidStruct1>), InvalidStruct1>,
    //               "InvalidStruct1 should not be serializable (unsupported double).");

    // // Тест 3: Проверяем, что InvalidStruct2 не компилируется
    // static_assert(!std::is_invocable_v<decltype(&Serialize<InvalidStruct2>), InvalidStruct2>,
    //               "InvalidStruct2 should not be serializable (unsupported nested vector).");

    // // Тест 4: Проверяем, что InvalidStruct3 не компилируется
    // static_assert(!std::is_invocable_v<decltype(&Serialize<InvalidStruct3>), InvalidStruct3>,
    //               "InvalidStruct3 should not be serializable (unsupported void* in vector).");

    // Тест 5: Проверяем корректную сериализацию и десериализацию структуры MyStruct3
    {
        MyStruct3 ms3;
        ms3.r1 = 123;
        ms3.some_str = "abcde";
        ms3.vals = {1, 2, 3};

        {
            MyStruct1 tmp1; tmp1.r0 = 1;
            MyStruct1 tmp2; tmp2.r0 = 2;
            ms3.ms1_vals.push_back(tmp1);
            ms3.ms1_vals.push_back(tmp2);
        }

        ms3.ms2_val.val = 1.22f;

        try {
            // Сериализация
            std::string jsonStr = Serialize(ms3);
            std::cout << "Serialized MyStruct3:\n" << jsonStr << "\n";

            // Десериализация
            auto ms3_copy = Deserialize<MyStruct3>(jsonStr);

            assert(ms3_copy.r1 == 123);
            assert(ms3_copy.some_str == "abcde");
            assert(ms3_copy.vals.size() == 3 && ms3_copy.vals[2] == 3);
            assert(ms3_copy.ms1_vals.size() == 2);
            assert(ms3_copy.ms1_vals[0].r0 == 1);
            assert(ms3_copy.ms1_vals[1].r0 == 2);
            assert(ms3_copy.ms2_val.val == 1.22f);

        } catch (const std::exception& e) {
            std::cerr << "Test failed: MyStruct3 threw an exception: " << e.what() << "\n";
            return 1;
        }
    }

    // Тест 6: Проверяем JSON с лишними полями
    {
        try {
            auto tooMany = R"({
                "r1": 1,
                "some_str": "abc",
                "vals": [],
                "ms2_val": { "val": 0.0 },
                "ms1_vals": [],
                "extra_field": 999
            })";
            auto fail1 = Deserialize<MyStruct3>(tooMany);
            std::cout << "ERROR: expected an exception for extra fields!\n";
            return 1;
        } catch (const std::runtime_error& e) {
            std::cout << "Got expected error (extra field): " << e.what() << "\n";
        }
    }

    // Тест 7: Проверяем JSON с неверным типом
    {
        try {
            auto wrongType = R"({
                "r1": "not_an_int",
                "some_str": "abc",
                "vals": [],
                "ms2_val": { "val": 0.0 },
                "ms1_vals": []
            })";
            auto fail2 = Deserialize<MyStruct3>(wrongType);
            std::cout << "ERROR: expected an exception for wrong type!\n";
            return 1;
        } catch (const std::runtime_error& e) {
            std::cout << "Got expected error (wrong type): " << e.what() << "\n";
        }
    }

    std::cout << "\nAll tests passed!\n";
    return 0;
}
