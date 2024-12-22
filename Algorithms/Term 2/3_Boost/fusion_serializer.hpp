#include <iostream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <cassert>

// Boost.Fusion
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/fusion/include/size.hpp>

// nlohmann/json
#include <nlohmann/json.hpp>

/******************************************************************************
 * 1) Объявляем структуры
 ******************************************************************************/
BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    MyStruct1,
    (int, r0)
)

BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    MyStruct2,
    (float, val)
)

BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    MyStruct3,
    (int, r1)
    (std::string, some_str)
    (std::vector<int>, vals)
    (pkg::MyStruct2, ms2_val)
    (std::vector<pkg::MyStruct1>, ms1_vals)
);

BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    ValidStruct,
    (int, valid_int)
    (float, valid_float)
    (std::string, valid_str)
    (std::vector<int>, valid_vec)
    (pkg::MyStruct2, valid_nested)
)

BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    InvalidStruct1,
    (int, valid_int)
    (double, unsupported_double) // double не поддерживается в текущей реализации
)

BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    InvalidStruct2,
    (std::string, valid_str)
    (std::vector<std::vector<int>>, unsupported_nested_vector) // std::vector<std::vector<T>> не поддерживается
)

BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    InvalidStruct3,
    (std::string, valid_str)
    (std::vector<void*>, unsupported_void_ptr_vector) // std::vector<void*> не поддерживается
);

/******************************************************************************
 * 2) Метапрограммные утилиты для проверки типов
 ******************************************************************************/
template <typename T>
struct IsFusionStruct : boost::fusion::traits::is_sequence<T> {};

template <typename T>
struct IsStdVector : std::false_type {};

template <typename U>
struct IsStdVector<std::vector<U>> : std::true_type {};

template <typename T>
constexpr bool IsStdVector_v = IsStdVector<T>::value;

/******************************************************************************
 * 3) Утилита для "статического цикла" [static_for]
 ******************************************************************************/
template <int Begin, int End>
struct StaticFor
{
    template <class F>
    static void run(const F& f)
    {
        f(std::integral_constant<int, Begin>{});
        StaticFor<Begin + 1, End>::run(f);
    }
};

template <int End>
struct StaticFor<End, End>
{
    template <class F>
    static void run(const F&) { /* завершаем рекурсию */ }
};

/******************************************************************************
 * 4) Универсальные функции (де)сериализации одного поля
 ******************************************************************************/
using json = nlohmann::json;

template <typename T>
void SerializeField(json& j, const std::string& fieldName, const T& value);

template <typename T>
void SerializeField(json& j, const std::string& fieldName, const std::vector<T>& vec);

template <typename T>
void DeserializeField(const json& j, const std::string& fieldName, T& outVal);

template <typename T>
void DeserializeField(const json& j, const std::string& fieldName, std::vector<T>& outVal);

/******************************************************************************
 * 5) Функции Serialize / Deserialize
 ******************************************************************************/
template <typename FusionT>
std::string Serialize(const FusionT& fusionObj)
{
    static_assert(IsFusionStruct<FusionT>::value, "Serialize: FusionT must be a Boost.Fusion struct.");

    constexpr int N = boost::fusion::result_of::size<FusionT>::value;
    json j;

    // Проходимся по индексам 0..N-1
    StaticFor<0, N>::run([&](auto iC)
    {
        constexpr int I = decltype(iC)::value;
        // Имя поля
        using boost::fusion::extension::struct_member_name;
        std::string fieldName = struct_member_name<FusionT, I>::call();

        // Значение поля
        auto const& fieldVal = boost::fusion::at_c<I>(fusionObj);

        // Сериализуем
        SerializeField(j, fieldName, fieldVal);
    });

    return j.dump(4);
}

template <typename FusionT>
FusionT Deserialize(std::string_view jsonStr)
{
    static_assert(IsFusionStruct<FusionT>::value, "Deserialize: FusionT must be a Boost.Fusion struct.");

    json j = json::parse(jsonStr.begin(), jsonStr.end());
    constexpr int N = boost::fusion::result_of::size<FusionT>::value;

    if (j.size() != static_cast<size_t>(N))
    {
        throw std::runtime_error("JSON fields count != struct fields count");
    }

    FusionT result{};
    // Проходимся по индексам 0..N-1
    StaticFor<0, N>::run([&](auto iC)
    {
        constexpr int I = decltype(iC)::value;
        using boost::fusion::extension::struct_member_name;
        std::string fieldName = struct_member_name<FusionT, I>::call();

        auto& fieldRef = boost::fusion::at_c<I>(result);
        DeserializeField(j, fieldName, fieldRef);
    });

    return result;
}

/******************************************************************************
 * 6) Реализация SerializeField
 ******************************************************************************/
template <typename T>
void SerializeField(json& j, const std::string& fieldName, const T& value)
{
    if constexpr (std::is_same_v<T, int> ||
                  std::is_same_v<T, float> ||
                  std::is_same_v<T, std::string>)
    {
        j[fieldName] = value;
    }
    else if constexpr (IsStdVector_v<T>)
    {
        SerializeField(j, fieldName, value);
    }
    else if constexpr (IsFusionStruct<T>::value)
    {
        auto innerJsonStr = Serialize(value);
        j[fieldName] = json::parse(innerJsonStr);
    }
    else
    {
        static_assert(!sizeof(T*), "Unsupported type in SerializeField");
    }
}

// vector<T>
template <typename T>
void SerializeField(json& j, const std::string& fieldName, const std::vector<T>& vec)
{
    json arr = json::array();
    for (auto const& elem : vec)
    {
        json tmp;
        SerializeField(tmp, "__dummy", elem);
        arr.push_back(tmp["__dummy"]);
    }
    j[fieldName] = arr;
}

/******************************************************************************
 * 7) Реализация DeserializeField
 ******************************************************************************/
template <typename T>
void DeserializeField(const json& j, const std::string& fieldName, T& outVal)
{
    if (!j.contains(fieldName))
    {
        throw std::runtime_error("Missing field: " + fieldName);
    }

    if constexpr (std::is_same_v<T, int>)
    {
        if (!j[fieldName].is_number_integer())
            throw std::runtime_error("Field '" + fieldName + "' is not an integer");
        outVal = j[fieldName].get<int>();
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        if (!j[fieldName].is_number_float() && !j[fieldName].is_number_integer())
            throw std::runtime_error("Field '" + fieldName + "' is not a float");
        outVal = j[fieldName].get<float>();
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        if (!j[fieldName].is_string())
            throw std::runtime_error("Field '" + fieldName + "' is not a string");
        outVal = j[fieldName].get<std::string>();
    }
    else if constexpr (IsStdVector_v<T>)
    {
        DeserializeField(j, fieldName, outVal);
    }
    else if constexpr (IsFusionStruct<T>::value)
    {
        auto nestedJson = j[fieldName];
        outVal = Deserialize<T>(nestedJson.dump());
    }
    else
    {
        static_assert(!sizeof(T*), "Unsupported type in DeserializeField");
    }
}

// vector<T>
template <typename T>
void DeserializeField(const json& j, const std::string& fieldName, std::vector<T>& outVal)
{
    if (!j[fieldName].is_array())
        throw std::runtime_error("Field '" + fieldName + "' is not an array");

    auto arr = j[fieldName];
    outVal.clear();
    outVal.reserve(arr.size());
    for (auto& elem : arr)
    {
        T tmpVal{};
        json tmp;
        tmp["__dummy"] = elem;
        DeserializeField(tmp, "__dummy", tmpVal);
        outVal.push_back(tmpVal);
    }
}