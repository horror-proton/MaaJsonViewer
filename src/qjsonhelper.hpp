#pragma once

#include <QJsonArray>
#include <QJsonValue>

#include <array>
#include <optional>
#include <vector>

template <typename T> struct json_type_info;

template <> struct json_type_info<int> {
  static std::optional<int> from_json(const QJsonValue &j) {
    if (!j.isDouble())
      return {};
    return j.toInt();
  }
};

template <> struct json_type_info<double> {
  static std::optional<double> from_json(const QJsonValue &j) {
    if (!j.isDouble())
      return {};
    return j.toDouble();
  }
};

template <> struct json_type_info<QString> {
  static std::optional<QString> from_json(const QJsonValue &j) {
    if (!j.isString())
      return {};
    return j.toString();
  }
};

template <> struct json_type_info<std::array<int, 4>> {
  static std::optional<std::array<int, 4>> from_json(const QJsonValue &j) {
    if (!j.isArray() || j.toArray().size() != 4)
      return {};
    auto result = std::array<int, 4>{};
    for (int i = 0; i < 4; ++i) {
      auto elem = j.toArray()[i];
      if (!elem.isDouble())
        return {};
      result[i] = static_cast<int>(elem.toDouble());
    }
    return result;
  }
};

template <typename T> struct json_type_info<std::vector<T>> {
  static std::optional<std::vector<T>> from_json(const QJsonValue &j) {
    if (!j.isArray())
      return {};
    std::vector<T> result{};
    for (auto &&s : j.toArray()) {
      // FIXME: unnecessary from QJsonValueRef to QJsonValue?
      if (auto val = json_type_info<T>::from_json(s))
        result.emplace_back(val.value());
      else
        return {};
    }
    return result;
  }
};

//===================================================

inline std::optional<std::vector<QString>>
from_string_or_string_list(const QJsonValue &j) {
  if (auto val = json_type_info<std::vector<QString>>::from_json(j))
    return val;
  if (auto val = json_type_info<QString>::from_json(j))
    return std::vector{val.value()};
  return {};
}

inline std::optional<std::vector<std::array<int, 4>>>
from_int4_or_int4_list(const QJsonValue &j) {
  if (auto val = json_type_info<std::vector<std::array<int, 4>>>::from_json(j))
    return val;
  if (auto val = json_type_info<std::array<int, 4>>::from_json(j))
    return std::vector{val.value()};
  return {};
}
