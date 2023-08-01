#include "textnode.h"
#include "networkwidget.h"
#include "nodeslotin.h"
#include "wire.h"

TextNode::TextNode(NetworkWidget *parent, QString text)
    : m_parent_network(parent), m_text(std::move(text)) {
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setFlag(ItemIsSelectable);

  auto *p_in_slot = new NodeSlotIn(this);
  p_in_slot->setPos(-10, 0);
  m_in_slot = p_in_slot;
}

void TextNode::paint(QPainter *painter,
                     const QStyleOptionGraphicsItem * /*option*/,
                     QWidget * /*widget*/) {
  painter->drawRoundedRect(-10, -5, 100, 10, 5, 5);

  QFont font = painter->font();
  font.setStyleHint(QFont::TypeWriter);
  font.setPixelSize(8);
  painter->setFont(font);

  painter->setPen(Qt::white);
  painter->drawText(QRect{0, -5, 80, 10}, m_text);
}

QRectF TextNode::boundingRect() const { return {-10, -10, 100, 20}; }

QVariant TextNode::itemChange(GraphicsItemChange change,
                              const QVariant &value) {
  switch (change) {
  case QGraphicsItem::ItemPositionHasChanged:
    for (auto *w : m_in_slot->m_wires)
      w->adjust();
    break;
  default:
    break;
  }
  return QGraphicsItem::itemChange(change, value);
}
