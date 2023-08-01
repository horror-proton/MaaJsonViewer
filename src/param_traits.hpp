#pragma once

#include <QtWidgets>
#include <type_traits>

template <typename W> struct param_traits {
  static decltype(auto) get_value(const W &widget) { return widget.value(); }

  using value_type =
      std::remove_cvref_t<decltype(get_value(std::declval<W>()))>;

  static decltype(auto) set_value(W &widget, const value_type &value) {
    return widget.setValue(value);
  }

  // static constexpr auto value_changed_signal =
  //     static_cast<void (W::*)(value_type)>(&W::valueChanged);
};

template <> struct param_traits<QLineEdit> {
  static decltype(auto) get_value(const QLineEdit &widget) {
    return widget.text();
  }

  using value_type = QString;

  static decltype(auto) set_value(QLineEdit &widget, const value_type &value) {
    return widget.setText(value);
  }

  // static constexpr auto value_changed_signal = &QLineEdit::textChanged;
};
