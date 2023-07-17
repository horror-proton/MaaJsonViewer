#pragma once

#include "networkwidget.h"
#include "nodeslotin.h"
#include "nodeslotout.h"
#include "qgraphicsitem.h"
#include <QGraphicsItem>

class Wire : public QGraphicsItem {
public:
  Wire(NetworkWidget *parent, NodeSlotOut *src, NodeSlotIn *dst);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

  QRectF boundingRect() const override;

public:
  void adjust();

private:
  QPointF scenePointSrc() const;
  QPointF scenePointDst() const;

private:
  NetworkWidget *m_parent_network = nullptr;

  NodeSlotOut *m_src_slot = nullptr;
  NodeSlotIn *m_dst_slot = nullptr;

  friend NetworkWidget;
  friend NodeSlotIn;
  friend NodeSlotOut;
  friend Node;

  bool m_is_from_selected = false;
  bool m_is_to_selected = false;
};
