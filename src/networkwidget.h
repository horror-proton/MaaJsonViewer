#pragma once

#include "qgraphicsview.h"
#include "qobjectdefs.h"
#include <QGraphicsView>
#include <unordered_map>

class Wire;
class Node;
class NodeSlotIn;
class NodeSlotOut;

class NetworkWidget : public QGraphicsView {
  Q_OBJECT

public:
  NetworkWidget(QWidget *parent = nullptr);

  void import_json(const QJsonObject &root);

  std::unordered_map<std::string, Node *> m_node_key_map;

protected:
  void drawBackground(QPainter *painter, const QRectF &rect) override;

  void mouseMoveEvent(QMouseEvent *event) override;

  void mousePressEvent(QMouseEvent *event) override;

  void mouseReleaseEvent(QMouseEvent *event) override;

public:
  void addSrcToPendingWire(NodeSlotOut *src);
  void addPendingToDstWire(NodeSlotIn *dst);
  void clearPendingWire();

  void createWire(NodeSlotOut *src, NodeSlotIn *dst);

  // delete a wire from scene and both slot, you should tell nodes to deal with
  // those modified slots manually
  void deleteWire(Wire *wire);

  Wire *m_pending_wire = nullptr; // TODO: use unique_prt
  NodeSlotIn *m_virt_dst = nullptr;
  NodeSlotOut *m_virt_src = nullptr;
};
