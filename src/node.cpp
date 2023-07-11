#include "node.h"

#include "nodeslotin.h"
#include "nodeslotout.h"
#include "qfont.h"
#include "qnamespace.h"
#include "textnode.h"
#include "wire.h"

#include <cstddef>
#include <qdebug.h>

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Node::Node(NetworkWidget *parent) : m_parent_network(parent) {
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setFlag(ItemIsSelectable);
  auto p_in_slot = new NodeSlotIn(this);
  p_in_slot->setPos(-10, 0);
  m_in_slot = p_in_slot;

  regenerate_reserved_out_slots();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
  if (isSelected()) {
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::blue);
    painter->drawRoundedRect(-5, -5, 100, 100, 10, 10);
  }
  painter->setPen(Qt::lightGray);
  painter->setBrush(Qt::darkGray);
  painter->drawRoundedRect(-10, -10, 100, 100, 10, 10);

  QFont font = painter->font();
  font.setStyleHint(QFont::TypeWriter);
  font.setPixelSize(8);
  painter->setFont(font);

  painter->setPen(Qt::white);
  painter->drawText(QRect{0, -20, 80, 10}, m_label);

  if (m_is_next_of_selected) {
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::green);
    painter->drawRect(-10, -20, 10, 10);
  }
}

QRectF Node::boundingRect() const { return QRectF{-15, -20, 120, 130}; }

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
  switch (change) {
  case QGraphicsItem::ItemPositionHasChanged:
    for (auto w : m_in_slot->m_wires)
      w->adjust();
    for (auto s : m_out_slots)
      for (auto w : s->m_wires)
        w->adjust();
    break;
  case QGraphicsItem::ItemSelectedChange:
    for (auto os : m_out_slots)
      for (auto ow : os->m_wires) {
        ow->m_is_from_selected = value.toBool();
        ow->update();
        std::visit(overloaded{
                       [](std::nullptr_t) {},
                       [selected = value.toBool()](auto &&next_node) {
                         next_node->m_is_next_of_selected = selected;
                         next_node->update();
                       },

                   },
                   ow->m_dst_slot->m_parent_node);
      }
    if (m_in_slot) {
      for (auto iw : m_in_slot->m_wires) {
        iw->m_is_to_selected = value.toBool();
        iw->update();
      }
    }
  default:
    break;
  }
  return QGraphicsItem::itemChange(change, value);
}

NodeSlotOut *Node::add_out_slot(int index) {
  auto slot = new NodeSlotOut(this);
  m_out_slots.insert(index, slot);
  adjust_out_slots();
  return slot;
}

void Node::adjust_out_slots() {
  int i = 0;
  for (auto s : m_out_slots) {
    s->setPos(90, i * node_spacing);
    for (auto w : s->m_wires)
      w->adjust();
    ++i;
  }
}

void Node::regenerate_reserved_out_slots() {
  const int desired_size = m_out_slots.size() + 1;
  if (m_reserved_out_slots.size() > desired_size) {
    for (int i = desired_size; i < m_reserved_out_slots.size(); ++i) {
      auto slot = m_reserved_out_slots.at(i);
      slot->setParentItem(nullptr);
      m_parent_network->scene()->removeItem(slot);
      delete slot;
      m_reserved_out_slots.removeAt(i);
    }
  } else if (m_reserved_out_slots.size() < desired_size) {
    for (int i = m_reserved_out_slots.size(); i < desired_size; ++i) {
      auto slot = new NodeSlotOut(this, true);
      m_reserved_out_slots.append(slot);
    }
  }
  int i = 0;
  for (auto s : m_reserved_out_slots) {
    s->setPos(85, i * node_spacing - 0.5 * node_spacing);
    ++i;
  }
}

void Node::out_slot_disconnect(NodeSlotOut *slot) {
  auto w = slot->m_wires.front();
  auto dst = w->m_dst_slot;
  m_parent_network->addPendingToDstWire(dst);
  m_parent_network->deleteWire(w);
  m_out_slots.removeOne(slot);
  delete slot;
  adjust_out_slots();
  regenerate_reserved_out_slots();
}
