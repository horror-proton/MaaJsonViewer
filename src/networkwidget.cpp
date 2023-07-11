#include "networkwidget.h"
#include "node.h"
#include "nodeslotin.h"
#include "nodeslotout.h"
#include "qgraphicsscene.h"
#include "qnamespace.h"
#include <QJsonArray>
#include <QJsonObject>

#include "QDebug"
#include "QMouseEvent"
#include "textnode.h"
#include "wire.h"

NetworkWidget::NetworkWidget(QWidget *parent) : QGraphicsView(parent) {
  {
    auto scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
  }

  setTransformationAnchor(AnchorUnderMouse);
  setSceneRect({});
  setCacheMode(CacheBackground);
  setRenderHints(QPainter::Antialiasing);

  scale(1.5, 1.5);
  /*
    auto n1 = new Node(this);
    n1->setPos(100, 100);
    scene()->addItem(n1);

    auto n2 = new Node(this);
    n2->setPos(200, 200);
    scene()->addItem(n2);

    auto n3 = new Node(this);
    n3->setPos(-100, -100);
    scene()->addItem(n3);
  */
}

uint32_t z_order_curve_helper(uint32_t x) {
  x &= 0x55555555;
  x = (x ^ (x >> 1)) & 0x33333333;
  x = (x ^ (x >> 2)) & 0x0f0f0f0f;
  x = (x ^ (x >> 4)) & 0x00ff00ff;
  x = (x ^ (x >> 8)) & 0x0000ffff;
  return x;
}

void NetworkWidget::import_json(const QJsonObject &root) {
  int i = 0;
  for (auto k : root.keys()) {
    if (k == "Stop") // Do not add Stop node
      continue;
    auto ix = z_order_curve_helper(i >> 0);
    auto iy = z_order_curve_helper(i >> 1);

    auto node = new Node(this);
    node->setPos(ix * 150, iy * 150 + 10 * ix);
    node->m_label = k;
    scene()->addItem(node);
    m_node_key_map.insert_or_assign(k.toStdString(), node);
    ++i;
  }

  for (auto [k, node] : m_node_key_map) {
    auto nextarr = root[QString::fromStdString(k)]["next"].toArray();
    for (auto other_key : nextarr) {
      auto iter = m_node_key_map.find(other_key.toString().toStdString());
      if (iter == m_node_key_map.end()) {
        qDebug() << "node " << other_key.toString() << " not found";
        // TODO: connect to a textbox representing unresolved nodes
        auto dst_node = new TextNode(this, other_key.toString());
        auto src_slot = node->add_out_slot();
        auto dst_slot = dst_node->in_slot();

        // FIXME: what to do when parent was deleted
        dst_node->setParentItem(node);
        dst_node->setPos(src_slot->pos().x() + 20, src_slot->pos().y());
        createWire(src_slot, dst_slot);
        continue;
      }
      auto dst_node = iter->second;
      auto src_slot = node->add_out_slot();
      auto dst_slot = dst_node->in_slot();
      createWire(src_slot, dst_slot);
    }
    node->regenerate_reserved_out_slots();
  }
}

void NetworkWidget::drawBackground(QPainter *painter, const QRectF &rect) {
  painter->setPen(Qt::NoPen);
  painter->fillRect(rect, Qt::gray);
  painter->drawRect(sceneRect());
}

void NetworkWidget::mouseMoveEvent(QMouseEvent *event) {
  if (m_pending_wire) {
    auto scene_pos = mapToScene(event->pos());
    if (m_virt_dst) {
      m_virt_dst->setPos(scene_pos);
      m_pending_wire->adjust();
    }
    if (m_virt_src) {
      m_virt_src->setPos(scene_pos);
      m_pending_wire->adjust();
    }
  }
  QGraphicsView::mouseMoveEvent(event);
}

void NetworkWidget::mousePressEvent(QMouseEvent *event) {
  QGraphicsView::mousePressEvent(event);
}

void NetworkWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    clearPendingWire();
  }
  QGraphicsView::mouseReleaseEvent(event);
}

void NetworkWidget::addSrcToPendingWire(NodeSlotOut *src) {
  clearPendingWire();
  m_virt_dst = new NodeSlotIn(nullptr);
  m_virt_dst->setPos(src->mapToScene(0, 0));
  scene()->addItem(m_virt_dst);
  m_pending_wire = new Wire(this, src, m_virt_dst);
  scene()->addItem(m_pending_wire);

  // setMouseTracking(true);
}

void NetworkWidget::addPendingToDstWire(NodeSlotIn *dst) {
  clearPendingWire();
  m_virt_src = new NodeSlotOut(nullptr);
  m_virt_src->setPos(dst->mapToScene(0, 0));
  scene()->addItem(m_virt_src);
  m_pending_wire = new Wire(this, m_virt_src, dst);
  scene()->addItem(m_pending_wire);
}

void NetworkWidget::clearPendingWire() {
  if (m_pending_wire) {
    scene()->removeItem(m_pending_wire);
    delete m_pending_wire;
    m_pending_wire = nullptr;
  }
  if (m_virt_src) {
    scene()->removeItem(m_virt_src);
    delete m_virt_src;
    m_virt_src = nullptr;
  }
  if (m_virt_dst) {
    scene()->removeItem(m_virt_dst);
    delete m_virt_dst;
    m_virt_dst = nullptr;
  }
}

void NetworkWidget::createWire(NodeSlotOut *src, NodeSlotIn *dst) {
  auto wire = new Wire(this, src, dst);
  dst->m_wires.append(wire);
  src->m_wires.append(wire);
  scene()->addItem(wire);
  clearPendingWire();
}

void NetworkWidget::deleteWire(Wire *wire) {
  wire->m_src_slot->m_wires.removeOne(wire);
  wire->m_dst_slot->m_wires.removeOne(wire);
  scene()->removeItem(wire);
  delete wire;
}
