#pragma once

#include <QGraphicsItem>

#include "networkwidget.h"
#include "nodeslotin.h"
#include "nodeslotout.h"
#include "qlist.h"

#include <string>

class Node : public QGraphicsItem {
public:
  Node(NetworkWidget *parent);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;

  QRectF boundingRect() const override;

protected:
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) override;

public:
  static constexpr double node_spacing = 12.;

  NetworkWidget *parent_network() const { return m_parent_network; }
  NodeSlotIn *in_slot() const { return m_in_slot; }
  const auto &out_slot() const { return m_out_slots; }
  NodeSlotOut *add_out_slot(int index);
  NodeSlotOut *add_out_slot() { return add_out_slot(m_out_slots.size()); }
  NodeSlotOut *remove_out_slot();
  void adjust_out_slots();
  void regenerate_reserved_out_slots();

  // disconnect wire from out slot and create pending wire
  void out_slot_disconnect(NodeSlotOut *slot);

private:
  NetworkWidget *m_parent_network = nullptr;

  NodeSlotIn *m_in_slot = nullptr;
  QList<NodeSlotOut *> m_out_slots = {};
  QList<NodeSlotOut *> m_reserved_out_slots = {};

  friend NodeSlotOut;
  friend NodeSlotIn;

public:
  QString m_label = "?"; // TODO: render more info
};
