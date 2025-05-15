#include "json.h"

JsonValue::JsonValue() : type_(ValueType::Null) {}

JsonValue::JsonValue(double number) : type_(ValueType::Number), number_(number) {}

JsonValue::JsonValue(const std::string& str) : type_(ValueType::String), string_(str) {}

JsonValue::JsonValue(const char* str) : JsonValue(std::string(str)) {}

JsonValue::JsonValue(const JsonValue& other) {
  Copy(other);
}

JsonValue::JsonValue(std::initializer_list<JsonValue> elements)
    : type_(ValueType::Array), array_(elements) {}

JsonValue& JsonValue::operator=(const JsonValue& other) {
  if (this != &other) {
    Clear();
    Copy(other);
  }
  return *this;
}

JsonValue::~JsonValue() {
  Clear();
}

JsonValue& JsonValue::operator[](const std::string& key) {
  EnsureObject();
  return (*object_)[key];
}

JsonValue& JsonValue::operator[](size_t index) {
  EnsureArray(index);
  return array_[index];
}

std::string JsonValue::ToString() const {
  std::ostringstream oss;
  ToStringInternal(oss);
  return oss.str();
}

void JsonValue::Copy(const JsonValue& other) {
  type_ = other.type_;
  switch (type_) {
    case ValueType::Object:
      object_ = new std::map<std::string, JsonValue>(*other.object_);
      break;
    case ValueType::String:
      string_ = other.string_;
      break;
    case ValueType::Number:
      number_ = other.number_;
      break;
    case ValueType::Array:
      array_ = std::move(other.array_);
    default:
      break;
  }
}

void JsonValue::Clear() {
  switch (type_) {
    case ValueType::Object:
      delete object_;
      object_ = nullptr;
      break;
    case ValueType::Array:
      array_.clear();
      break;
    default:
      break;
  }
  type_ = ValueType::Null;
}

void JsonValue::EnsureObject() {
  if (type_ != ValueType::Object) {
    Clear();
    type_ = ValueType::Object;
    object_ = new std::map<std::string, JsonValue>();
  }
}

void JsonValue::EnsureArray(size_t index) {
  if (type_ != ValueType::Array) {
    Clear();
    type_ = ValueType::Array;
  }
  if (array_.size() < index + 1) {
    array_.resize(index + 1);
  }
}

void JsonValue::ToStringInternal(std::ostringstream& oss) const {
  switch (type_) {
    case ValueType::Null:
      oss << "{}";
      break;
    case ValueType::Number:
      if (number_ == floor(number_) && number_ >= std::numeric_limits<int>::min() &&
          number_ <= std::numeric_limits<int>::max()) {
        oss << static_cast<int>(number_);
      } else {
        oss << number_;
      }
      break;
    case ValueType::String:
      oss << "\"" << EscapeString(string_) << "\"";
      break;
    case ValueType::Object: {
      oss << "{";
      bool first = true;
      for (const auto& pair : *object_) {
        if (!first)
          oss << ", ";
        first = false;
        oss << "\"" << EscapeString(pair.first) << "\": ";
        pair.second.ToStringInternal(oss);
      }
      oss << "}";
      break;
    }
    case ValueType::Array: {
      oss << "[";
      for (size_t i = 0; i < array_.size(); ++i) {
        if (i > 0) {
          oss << ",";
        }
        array_[i].ToStringInternal(oss);
      }
      oss << "]";
      break;
    }
    default:
      oss << "null";
      break;
  }
}

// static
std::string JsonValue::EscapeString(const std::string& str) {
  std::ostringstream oss;
  for (char c : str) {
    switch (c) {
      case '"':
        oss << "\\\"";
        break;
      case '\\':
        oss << "\\\\";
        break;
      case '\b':
        oss << "\\b";
        break;
      case '\f':
        oss << "\\f";
        break;
      case '\n':
        oss << "\\n";
        break;
      case '\r':
        oss << "\\r";
        break;
      case '\t':
        oss << "\\t";
        break;
      default:
        if (static_cast<unsigned char>(c) < 0x20) {
          oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
        } else {
          oss << c;
        }
        break;
    }
  }
  return oss.str();
}