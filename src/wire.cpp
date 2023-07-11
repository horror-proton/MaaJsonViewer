#include "wire.h"
#include "networkwidget.h"
#include "qdebug.h"
#include <cmath>
#include <cstddef>

Wire::Wire(NetworkWidget *parent, NodeSlotOut *src, NodeSlotIn *dst)
    : m_parent_network(parent), m_src_slot(src), m_dst_slot(dst) {
  if (src->isOrphan() || dst->isOrphan())
    setAcceptedMouseButtons(Qt::NoButton);
}

QPointF Wire::scenePointSrc() const {
  if (!m_src_slot)
    return {};
  return m_src_slot->mapToScene(0, 0);
}
QPointF Wire::scenePointDst() const {
  if (!m_dst_slot)
    return {};
  return m_dst_slot->mapToScene(0, 0);
}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
  if (!m_src_slot || !m_dst_slot)
    return;

  auto src_pt = scenePointSrc();
  auto dst_pt = scenePointDst();

  if (m_is_from_selected)
    painter->setPen(Qt::green);
  else if (m_is_to_selected)
    painter->setPen(Qt::red);
  else
    painter->setPen(Qt::lightGray);

  static constexpr double radius = 10;
  static constexpr size_t subdiv = 18;

  const auto ptdist =
      std::sqrt(QPointF::dotProduct(dst_pt - src_pt, dst_pt - src_pt));

  if (ptdist <= 4 * radius) {
    painter->drawLine(src_pt, dst_pt);
    return;
  }

  const int sgn = (dst_pt.y() >= src_pt.y()) ? 1 : -1;

  const bool loop =
      dst_pt.x() < src_pt.x() &&
      std::abs(dst_pt.y() - src_pt.y()) < 4 * radius; // FIXME: shit here

  const auto c1 = QPointF(src_pt.x(), src_pt.y() + (loop ? -1 : sgn) * radius);
  const auto c2 = QPointF(dst_pt.x(), dst_pt.y() + (loop ? -1 : -sgn) * radius);

  auto c_angle = std::atan2((c2 - c1).y(), (c2 - c1).x());

  const auto half_c_dist =
      0.5 * std::sqrt(QPointF::dotProduct(c2 - c1, c2 - c1));
  const auto t_angle = std::asin(radius / half_c_dist);

  const auto phi = c_angle + sgn * t_angle;

  static thread_local QPointF divs[subdiv * 2];

  if (loop) {
    if (c_angle > 0)
      c_angle -= 2 * 3.14159;
    for (std::size_t i = 0; i < subdiv; ++i) {
      auto t = c_angle / subdiv * i;
      divs[i] = {c1.x() - radius * std::sin(t), c1.y() + radius * std::cos(t)};
    }
    for (std::size_t i = 0; i < subdiv; ++i) {
      auto t = (2 * 3.14159 + c_angle) * (1. - 1. / subdiv * i);
      divs[i + subdiv] = {c2.x() - radius * std::sin(t),
                          c2.y() + radius * std::cos(t)};
    }
  } else {
    for (std::size_t i = 0; i < subdiv; ++i) {
      auto t = phi / subdiv * i;
      divs[i] = {c1.x() + radius * std::abs(std::sin(t)),
                 c1.y() - sgn * radius * std::cos(t)};
    }
    for (std::size_t i = 0; i < subdiv; ++i) {
      auto t = phi - phi / subdiv * i;
      divs[i + subdiv] = {c2.x() - radius * std::abs(std::sin(t)),
                          c2.y() + sgn * radius * std::cos(t)};
    }
  }

  painter->drawPolyline(divs, subdiv * 2);
}

QRectF Wire::boundingRect() const {
  if (!m_src_slot || !m_dst_slot)
    return QRectF{};

  auto src_pt = scenePointSrc();
  auto dst_pt = scenePointDst();

  return QRectF{src_pt,
                QSizeF{dst_pt.x() - src_pt.x(), dst_pt.y() - src_pt.y()}}
      .normalized()
      .adjusted(-30, -30, 60, 60);
}

void Wire::adjust() {
  // TODO: use cached position, update them here
  prepareGeometryChange();
}
