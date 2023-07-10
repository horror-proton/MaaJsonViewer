#include "networkwidget.h"
#include "qdebug.h"
#include "qmainwindow.h"
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QMainWindow main_window;

  auto network = new NetworkWidget;
  main_window.setCentralWidget(network);

  QJsonObject root;

  QFile f;
  {
    f.setFileName("/usr/share/maa-assistant-arknights/resource/tasks.json");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    auto buf = f.readAll();
    root = QJsonDocument::fromJson(buf).object();
  }
  network->import_json(root);

  main_window.show();
  return app.exec();
}
