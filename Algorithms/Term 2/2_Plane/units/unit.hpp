// units/unit.hpp
#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>

class Unit {
protected:
  int id;

  static int getNextId() {
    static int global_id = 0;
    return ++global_id;
  }

public:
  Unit() : id(getNextId()) {}

  int getId() const { return id; }

  virtual std::string getType() const = 0;
  virtual int getTotalHandLuggageWeight() const = 0;
  virtual int getTotalLuggageWeight() const = 0;

  virtual ~Unit() = default;
};

#endif // UNIT_HPP
