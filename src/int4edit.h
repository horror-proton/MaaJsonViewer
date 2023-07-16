#pragma once

#include <QGridLayout>
#include <QJsonArray>
#include <QSpinBox>
#include <QWidget>

class Int4Edit : public QWidget {
public:
  Int4Edit(QWidget *parent = nullptr) {
    auto lo = new QGridLayout;
    lo->setMargin(0);
    setLayout(lo);
    for (size_t i = 0; i < 4; ++i) {
      auto sb = new QSpinBox;
      sb->setMaximum(1920);
      lo->addWidget(sb, 0, i);
      m_spin_boxes[i] = sb;
    }
  }

  void set_value(int x, int y, int z, int w) {
    m_spin_boxes[0]->setValue(x);
    m_spin_boxes[1]->setValue(y);
    m_spin_boxes[2]->setValue(z);
    m_spin_boxes[3]->setValue(w);
  }

  template <typename C> void set_value(const C &container) {
    m_spin_boxes[0]->setValue(container[0]);
    m_spin_boxes[1]->setValue(container[1]);
    m_spin_boxes[2]->setValue(container[2]);
    m_spin_boxes[3]->setValue(container[3]);
  }

  void set_value(const QJsonArray &a) {
    set_value(a[0].toDouble(), a[1].toDouble(), a[2].toDouble(),
              a[3].toDouble());
  }

private:
  QSpinBox *m_spin_boxes[4] = {};
};