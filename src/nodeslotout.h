#pragma once

#include "networkwidget.h"
#include "qobjectdefs.h"
#include <QGraphicsItem>

class Node;

class NodeSlotOut : public QGraphicsItem {
public:
  NodeSlotOut(Node *parent, bool reserved = false);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;

  QRectF boundingRect() const override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
  constexpr bool isOrphan() const { return !m_parent_node; }

  constexpr bool isReservedNode() const { return m_reserved; }

  int nodeIndex() const;

  constexpr Node *parentNode() const { return m_parent_node; }

private:
  Node *m_parent_node = nullptr;
  const bool m_reserved = false;

  QList<Wire *> m_wires = {};
  friend NetworkWidget;
  friend Node;
};
