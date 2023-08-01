#pragma once

#include "networkwidget.h"
#include <QGraphicsItem>

class TextNode : public QGraphicsItem {
public:
  TextNode(NetworkWidget *parent, QString text);
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

  QRectF boundingRect() const override;

  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) override;

public:
  NetworkWidget *parent_network() const { return m_parent_network; }
  NodeSlotIn *in_slot() const { return m_in_slot; }

private:
  NetworkWidget *m_parent_network;
  NodeSlotIn *m_in_slot;
  QString m_text;
  bool m_is_next_of_selected = false;

  friend Node;
};
