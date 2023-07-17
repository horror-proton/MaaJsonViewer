#pragma once

#include "qgraphicsview.h"
#include "qobjectdefs.h"
#include <QDirIterator>
#include <QGraphicsView>
#include <optional>
#include <unordered_map>

class Wire;
class Node;
class NodeSlotIn;
class NodeSlotOut;

class NetworkWidget : public QGraphicsView {
  Q_OBJECT

public:
  explicit NetworkWidget(QWidget *parent = nullptr);

  void import_json(const QJsonObject &root, const QDir &img_dir);

  std::unordered_map<std::string, Node *> m_node_key_map;

protected:
  void drawBackground(QPainter *painter, const QRectF &rect) override;

  void mouseMoveEvent(QMouseEvent *event) override;

  void mousePressEvent(QMouseEvent *event) override;

  void mouseReleaseEvent(QMouseEvent *event) override;

  void wheelEvent(QWheelEvent *event) override;

signals:
  void node_selection_changed(Node *);

public:
  void scaleView(double factor);
  // void moveView(const QPoint &delta);

  void addSrcToPendingWire(NodeSlotOut *src);
  void addPendingToDstWire(NodeSlotIn *dst);
  void clearPendingWire();

  void createWire(NodeSlotOut *src, NodeSlotIn *dst);

  // delete a wire from scene and both slot, you should tell nodes to deal with
  // those modified slots manually
  void deleteWire(Wire *wire);

private:
  Wire *m_pending_wire = nullptr; // TODO: use unique_prt
  NodeSlotIn *m_virt_dst = nullptr;
  NodeSlotOut *m_virt_src = nullptr;

  bool m_keep_pending_wire_once = false; // any idea how to remove this?

  friend NodeSlotOut;
  friend NodeSlotIn;

  std::optional<QPoint> m_mmb_down = std::nullopt;
};
