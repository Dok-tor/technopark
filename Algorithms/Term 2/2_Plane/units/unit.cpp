#include "unit.hpp"

// Инициализация глобального счётчика для уникальных идентификаторов
static int global_id = 0;

Unit::Unit()
        : id(++global_id)
{
}

Unit::~Unit() = default;

int Unit::getId() const
{
    return id;
}
