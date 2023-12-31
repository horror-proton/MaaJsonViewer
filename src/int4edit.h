#pragma once

#include <QGridLayout>
#include <QJsonArray>
#include <QSpinBox>
#include <QWidget>

#include <array>

class Int4Edit : public QWidget {
public:
  explicit Int4Edit(QWidget *parent = nullptr) : QWidget(parent) {
    auto *lo = new QGridLayout;
    lo->setMargin(0);
    setLayout(lo);
    for (int i = 0; i < 4; ++i) {
      auto *sb = new QSpinBox;
      sb->setMaximum(1920);
      lo->addWidget(sb, 0, i);
      m_spin_boxes[i] = sb;
    }
  }

  [[nodiscard]] std::array<int, 4> value() const {
    return {
        m_spin_boxes[0]->value(),
        m_spin_boxes[1]->value(),
        m_spin_boxes[2]->value(),
        m_spin_boxes[3]->value(),
    };
  }

  void setValue(int x, int y, int z, int w) {
    m_spin_boxes[0]->setValue(x);
    m_spin_boxes[1]->setValue(y);
    m_spin_boxes[2]->setValue(z);
    m_spin_boxes[3]->setValue(w);
  }

  template <typename C> void setValue(const C &container) {
    m_spin_boxes[0]->setValue(container[0]);
    m_spin_boxes[1]->setValue(container[1]);
    m_spin_boxes[2]->setValue(container[2]);
    m_spin_boxes[3]->setValue(container[3]);
  }

  void setValue(const QJsonArray &a) {
    m_spin_boxes[0]->setValue(static_cast<int>(a[0].toDouble()));
    m_spin_boxes[1]->setValue(static_cast<int>(a[1].toDouble()));
    m_spin_boxes[2]->setValue(static_cast<int>(a[2].toDouble()));
    m_spin_boxes[3]->setValue(static_cast<int>(a[3].toDouble()));
  }

private:
  QSpinBox *m_spin_boxes[4] = {};
};
