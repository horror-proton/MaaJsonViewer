#pragma once

#include "int4edit.h"
#include "node.h"
#include "qcheckbox.h"
#include "qgroupbox.h"
#include "qobjectdefs.h"
#include "qwidget.h"
#include <QComboBox>

class ParameterEditor : public QWidget {
  Q_OBJECT
public:
  explicit ParameterEditor(QWidget *parent = nullptr);

  void load_from_json(const QJsonObject &json);

  void store_to_json(QJsonObject &json);

private:
  Node *m_node = nullptr;

public slots:
  void update_selection(Node *node);

private:
  QGroupBox *m_node_group;
  QComboBox *m_recognition_combo;
  Int4Edit *m_roi_edit;
  QCheckBox *m_cache_check;
};
