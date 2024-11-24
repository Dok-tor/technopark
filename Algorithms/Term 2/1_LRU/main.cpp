#include "lru.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Проверка на запрещённые символы
bool hasForbiddenCharacters(const std::string &str) {
  for (unsigned char c : str) {
    if (c == '\n' || c == '\r' || c == '\t' || c == '\0' || c == ' ') {
      return true;
    }
  }
  return false;
}

// Проверка валидности UTF-8
bool isValidUTF8(const std::string &str) {
  size_t i = 0;
  const size_t len = str.length();

  while (i < len) {
    unsigned char c = str[i];

    if (c <= 0x7F) {
      // 1-byte character
      i += 1;
    } else if ((c & 0xE0) == 0xC0) {
      // 2-byte character
      if (i + 1 >= len || (str[i + 1] & 0xC0) != 0x80 ||
          (c & 0xFE) == 0xC0) { // Overlong encoding
        return false;
      }
      i += 2;
    } else if ((c & 0xF0) == 0xE0) {
      // 3-byte character
      if (i + 2 >= len || (str[i + 1] & 0xC0) != 0x80 ||
          (str[i + 2] & 0xC0) != 0x80) {
        return false;
      }
      i += 3;
    } else if ((c & 0xF8) == 0xF0) {
      // 4-byte character
      if (i + 3 >= len || (str[i + 1] & 0xC0) != 0x80 ||
          (str[i + 2] & 0xC0) != 0x80 || (str[i + 3] & 0xC0) != 0x80 ||
          c > 0xF4) { // Maximum valid Unicode code point is U+10FFFF
        return false;
      }
      i += 4;
    } else {
      return false;
    }
  }

  return true;
}

// Проверка валидности CP1251 без символов 0x80 - 0xBF
bool isValidCP1251(const std::string &str) {
  for (unsigned char c : str) {
    if (c >= 0x80 && c <= 0xBF) {
      return false;
    }
  }
  return true;
}

// Проверка валидности строки (кодировка UTF-8 или CP1251)
bool isValidString(const std::string &str) {
  if (hasForbiddenCharacters(str)) {
    return false;
  }

  if (isValidUTF8(str)) {
    return true;
  }

  if (isValidCP1251(str)) {
    return true;
  }

  return false;
}

// Обработка входных данных
void processInput(const std::string &input,
                  LRUCache<std::string, std::vector<float>> &cache) {
  size_t tab_pos = input.find('\t');
  if (tab_pos != std::string::npos) {
    // Строка типа (а): <q>\t<f1> <f2> <f3> ...
    std::string q = input.substr(0, tab_pos);
    std::string floats_str = input.substr(tab_pos + 1);

    if (!isValidString(q)) {
      std::cout << "!STORERR!" << std::endl;
      return;
    }

    std::istringstream iss(floats_str);
    std::vector<float> embedding;
    float value;
    int count = 0;
    bool error = false;

    while (iss >> value) {
      embedding.push_back(value);
      ++count;
      if (count > 64) {
        error = true;
        break;
      }
    }

    if (count != 64 || error || !iss.eof()) {
      // Если не удалось считать ровно 64 числа или есть лишние данные
      std::cout << "!STORERR!" << std::endl;
      return;
    }

    if (cache.Put(q, embedding)) {
      std::cout << "!STORED!" << std::endl;
    } else {
      std::cout << "!STORERR!" << std::endl;
    }
  } else {
    // Строка типа (б): <q>
    const std::string &q = input;

    if (!isValidString(q)) {
      std::cout << "!NOEMBED!" << std::endl;
      return;
    }

    std::vector<float> *result = cache.Get(q);
    if (result) {
      for (size_t i = 0; i < result->size(); ++i) {
        std::cout << (*result)[i];
        if (i != result->size() - 1) {
          std::cout << " ";
        }
      }
      std::cout << std::endl;
    } else {
      std::cout << "!NOEMBED!" << std::endl;
    }
  }
}

int main() {
  size_t N = 3; // Максимальный размер кэша в записях
  size_t Size = 1000; // Максимальный размер кэша в байтах

  // Читаем значения N и Size
  std::cin >> N >> Size;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  LRUCache<std::string, std::vector<float>> cache(N, Size);
  std::string line;
  while (std::getline(std::cin, line)) {
    processInput(line, cache);
  }
  return 0;
}
