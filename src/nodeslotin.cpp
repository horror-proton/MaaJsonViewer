#include "nodeslotin.h"
#include "QGraphicsSceneMouseEvent"
#include "node.h"
#include "qgraphicsitem.h"
#include "qnamespace.h"
#include "qpainter.h"
#include "wire.h"
#include <cmath>

#include "QDebug"

void NodeSlotIn::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  if (isOrphan()) {
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawEllipse(0, 0, 3, 3);
    return;
  }
  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::white);
  QPointF points[] = {{0, 0}, {5, 3}, {5, -3}};
  painter->drawPolygon(points, 3);
}

QRectF NodeSlotIn::boundingRect() const { return QRectF{0, -5, 5, 10}; }

void NodeSlotIn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  auto pending_wire = parent_network()->m_pending_wire;
  if (pending_wire && pending_wire->m_dst_slot->isOrphan()) {
    auto src_slot = pending_wire->m_src_slot;
    auto src_node = src_slot->parentNode();
    auto src_slot_index = src_slot->nodeIndex();
    auto node = src_node->add_out_slot(src_slot_index);
    src_node->regenerate_reserved_out_slots();

    parent_network()->createWire(node, this);
  }
  QGraphicsItem::mouseReleaseEvent(event);
}
void NodeSlotIn::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
  event->accept();
}
