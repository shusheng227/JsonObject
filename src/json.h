#ifndef UTIL_JSON_H_
#define UTIL_JSON_H_

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class JsonValue {
 public:
  JsonValue();
  JsonValue(double number);
  JsonValue(const std::string& str);
  JsonValue(const char* str);
  JsonValue(const JsonValue& other);
  JsonValue(std::initializer_list<JsonValue> elements);

  ~JsonValue();

  JsonValue& operator=(const JsonValue& other);
  JsonValue& operator[](const std::string& key);
  JsonValue& operator[](size_t index);

  std::string ToString() const;

 private:
  static std::string EscapeString(const std::string& str);

  void Copy(const JsonValue& other);
  void Clear();
  void EnsureObject();
  void EnsureArray(size_t index);
  void ToStringInternal(std::ostringstream& oss) const;

 private:
  enum class ValueType { Null, Number, String, Object, Array };

  ValueType type_ = ValueType::Null;
  double number_ = 0;
  std::string string_;
  std::map<std::string, JsonValue>* object_ = nullptr;
  std::vector<JsonValue> array_;
};

#endif