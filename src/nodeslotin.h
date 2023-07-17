#pragma once

#include "networkwidget.h"
#include "node.h"
#include "qgraphicsitem.h"
#include "textnode.h"

#include <cstddef>
#include <variant>

class Node;
class Wire;

class NodeSlotIn : public QGraphicsItem {
public:
  explicit NodeSlotIn(auto parent) : m_parent_node(parent) {
    if (parent) {
      setParentItem(static_cast<QGraphicsItem *>(parent));
      setAcceptedMouseButtons(Qt::LeftButton);
    } else {
      setAcceptedMouseButtons(Qt::NoButton);
    }
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

  QRectF boundingRect() const override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
  Node *parentNode() const {
    const auto *pp = std::get_if<Node *>(&m_parent_node);
    if (pp == nullptr)
      return nullptr;
    return *pp;
  }
  bool isOrphan() const {
    return std::holds_alternative<std::nullptr_t>(m_parent_node);
  }
  NetworkWidget *parent_network() const {
    return std::visit(
        [](auto &&ptr) -> NetworkWidget * {
          if constexpr (std::is_same_v<std::decay_t<decltype(ptr)>,
                                       std::nullptr_t>)
            return nullptr;
          else
            return ptr->parent_network();
        },
        m_parent_node);
  }

private:
  std::variant<std::nullptr_t, Node *, TextNode *> m_parent_node = nullptr;
  QList<Wire *> m_wires = {};
  friend NetworkWidget;
  friend Node;
  friend TextNode;
};
