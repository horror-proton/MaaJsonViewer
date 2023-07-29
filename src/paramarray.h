#pragma once

#include "qboxlayout.h"
#include "qdebug.h"
#include "qlist.h"
#include "qobjectdefs.h"
#include "qpushbutton.h"
#include "qwidget.h"

#include <vector>

template <typename W> struct param_traits {
  static constexpr decltype(auto) get_value(const W &widget) {
    return (widget.value());
  }

  using value_type =
      std::remove_reference_t<decltype(get_value(std::declval<W>()))>;

  static constexpr decltype(auto) set_value(const W &widget,
                                            const value_type &value) {
    return (widget.setValue(value));
  }
};

template <typename Q> struct qobj_deleter {
  void operator()(Q *ptr) const { ptr->deleteLater(); }
};

template <typename T> class ParamArray : public QWidget {
public:
  using parameter = T;

  explicit ParamArray(QWidget *parent = nullptr)
      : QWidget(parent), m_layout(new QVBoxLayout) {
    m_layout->setMargin(0);
    setLayout(m_layout);

    auto *buttons = new QHBoxLayout;
    auto *add_button = new QPushButton("+");
    auto *sub_button = new QPushButton("-");
    buttons->addWidget(add_button);
    buttons->addWidget(sub_button);
    m_layout->addLayout(buttons);
    connect(add_button, &QPushButton::released, this, &ParamArray::add_param);
    connect(sub_button, &QPushButton::released, this, &ParamArray::pop_param);

    add_param();
  }

  parameter *add_param() {
    auto *p = new parameter;
    m_p.push_back(p);
    m_layout->insertWidget(m_layout->count() - 1, p);
    return p;
  }

  constexpr auto size() const { return m_p.size(); }

  constexpr auto get_values() const {
    std::vector<typename param_traits<parameter>::value_type> result{};
    result.reserve(m_p.size());
    for (auto p : m_p)
      result.push_back(param_traits<parameter>::get_value(*p));
    return result;
  }

  auto pop_param() {
    if (m_p.empty())
      return std::shared_ptr<parameter>{};
    parameter *p = m_p.back();
    m_p.pop_back();
    m_layout->removeWidget(p);
    return std::shared_ptr<parameter>(p, qobj_deleter<parameter>{});
  }

  /* This crashes gcc 13.1.1
  std::unique_ptr<parameter, decltype([](auto *ptr) { ptr->deleteLater(); })>
  pop_param() {
    if (m_p.empty())
      return {};
    auto *p = m_p.back();
    m_p.pop_back();
    m_layout->removeWidget(p);
    return p;
  }
  */

private:
  QVBoxLayout *m_layout;
  QList<parameter *> m_p;
};
