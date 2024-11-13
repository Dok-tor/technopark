#include "unit.hpp"

// Инициализация глобального счётчика для уникальных идентификаторов
static int global_id = 0;

// Определение конструктора
Unit::Unit() : id(++global_id) {}

// Определение виртуального деструктора
Unit::~Unit() = default;

// Определение метода для получения идентификатора
int Unit::getId() const {
    return id;
}
