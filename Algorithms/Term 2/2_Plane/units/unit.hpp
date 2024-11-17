// units/unit.hpp
#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>

class Unit {
protected:
    int id;

public:
    Unit();
    virtual ~Unit();

    int getId() const;

    virtual std::string getType() const = 0;
    virtual int getTotalHandLuggageWeight() const = 0;
    virtual int getTotalLuggageWeight() const = 0;
};

#endif // UNIT_HPP
