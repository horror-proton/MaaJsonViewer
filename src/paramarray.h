#pragma once

#include "qboxlayout.h"
#include "qdebug.h"
#include "qlineedit.h"
#include "qlist.h"
#include "qobjectdefs.h"
#include "qpushbutton.h"
#include "qwidget.h"

#include <QDebug>

#include <vector>

#include "param_traits.hpp"

template <typename Q> struct qobj_deleter {
  void operator()(Q *ptr) const { ptr->deleteLater(); }
};

class ParamArrayBase : public QWidget {
  Q_OBJECT
public:
  using QWidget::QWidget;
  void emit_value_changed() { emit value_changed(); }
signals:
  void value_changed();
};

template <typename T> class ParamArray : public ParamArrayBase {
public:
  using parameter = T;

  explicit ParamArray(QWidget *parent = nullptr)
      : ParamArrayBase(parent), m_layout(new QVBoxLayout) {
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
  }

  parameter *add_param() {
    auto *p = new parameter;
    m_p.push_back(p);
    m_layout->insertWidget(m_layout->count() - 1, p);
    /* TODO
    connect(p, param_traits<parameter>::value_changed_signal, this,
            &ParamArray::emit_value_changed);
    */
    emit_value_changed();
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

  template <typename C> void set_values(const C &values) {
    using std::size;
    for (; m_p.size() > size(values);)
      pop_param();
    for (; m_p.size() < size(values);)
      add_param();
    for (int i = 0; i < m_p.size(); ++i)
      param_traits<parameter>::set_value(*m_p[i], values[i]);
    emit_value_changed();
  }

  auto pop_param() {
    if (m_p.empty())
      return std::shared_ptr<parameter>{};
    parameter *p = m_p.back();
    m_p.pop_back();
    m_layout->removeWidget(p);
    emit_value_changed();
    return std::shared_ptr<parameter>(p, qobj_deleter<parameter>{});
  }

private:
  QVBoxLayout *m_layout;
  QList<parameter *> m_p;
};
