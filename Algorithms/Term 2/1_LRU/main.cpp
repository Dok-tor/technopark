#include "lru.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Подключаем ICU
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <unicode/utf8.h>

// Проверка валидности строки (кодировка UTF-8 или CP1251)
bool isValidString(const std::string& str) {
    // Проверяем на запрещённые символы
    for (unsigned char c : str) {
        if (c == '\n' || c == '\r' || c == '\t' || c == '\0' || c == ' ') {
            return false;
        }
    }

    // Попытка декодировать как UTF-8
    UErrorCode status = U_ZERO_ERROR;
    UConverter* conv = ucnv_open("UTF-8", &status);
    if (U_FAILURE(status)) {
        return false;
    }

    const char* source = str.c_str();
    const char* sourceLimit = source + str.length();
    UChar targetBuffer[256];
    UChar* target = targetBuffer;
    const UChar* targetLimit = targetBuffer + sizeof(targetBuffer) / sizeof(UChar);

    ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, nullptr, true, &status);
    ucnv_close(conv);

    if (U_SUCCESS(status) && source == sourceLimit) {
        return true;
    }

    // Попытка декодировать как CP1251
    status = U_ZERO_ERROR;
    conv = ucnv_open("windows-1251", &status);
    if (U_FAILURE(status)) {
        return false;
    }

    source = str.c_str();
    sourceLimit = source + str.length();
    target = targetBuffer;

    ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, nullptr, true, &status);
    ucnv_close(conv);

    if (U_SUCCESS(status) && source == sourceLimit) {
        // Дополнительно проверяем, нет ли символов в диапазоне 0x80 - 0xBF
        for (unsigned char c : str) {
            if (c >= 0x80 && c <= 0xBF) {
                return false;
            }
        }
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
