#pragma once

#include "networkwidget.h"
#include "qgraphicsitem.h"

class Node;
class Wire;

class NodeSlotIn : public QGraphicsItem {
public:
  NodeSlotIn(Node *parent);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;

  QRectF boundingRect() const override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
  constexpr Node *parentNode() const { return m_parent_node; }
  bool isOrphan() { return !m_parent_node; }


private:
  Node *m_parent_node = nullptr;
  QList<Wire *> m_wires = {};
  friend NetworkWidget;
  friend Node;
};
