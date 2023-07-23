#include "networkwidget.h"
#include "parametereditor.h"
#include "qdebug.h"
#include "qmainwindow.h"
#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QMainWindow main_window;

  auto *network = new NetworkWidget;
  auto *param_editor = new ParameterEditor;

  auto *central = new QWidget;
  auto *lo = new QHBoxLayout(central);
  lo->addWidget(network);
  lo->addWidget(param_editor);
  QObject::connect(network, &NetworkWidget::node_selection_changed,
                   param_editor, &ParameterEditor::update_selection);
  main_window.setCentralWidget(central);

  QJsonObject root;
  {
    QFile f;
    f.setFileName("./MAA1999/assets/resource/pipeline/startup.json");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    auto buf = f.readAll();
    root = QJsonDocument::fromJson(buf).object();
  }

  network->import_json(root, QDir("./MAA1999/assets/resource/"
                                  "pipeline"));

  main_window.show();
  return QApplication::exec();
}
