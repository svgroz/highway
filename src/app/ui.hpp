#pragma once

#include "ui_connection_properties.h"
#include "ui_main_window.h"

#include <QMainWindow>
#include <QWidget>

#include <functional>

namespace highway::ui {

class UI final : public QObject {
  Q_OBJECT
public:
  explicit UI(QObject *parent = nullptr);
  UI(UI &) = delete;
  ~UI() final = default;

public slots:
  void showConnectionPropertiesForm();
  void connectionPropertiesFormDestroyed(QObject *);
  void saveConnectionProperties(bool);

private:
  QMainWindow *_mainWindowWidget;
  QWidget *_connectionProperiesWidget;
  Ui::MainWindow *_mainWindow;
  Ui::ConnectionPropertiesForm *_connectionPropertiesForm;
};
} // namespace highway::ui