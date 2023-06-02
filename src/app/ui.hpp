#pragma once

#include "ui_connection_properties.h"
#include "ui_main_window.h"

#include <QMainWindow>
#include <QWidget>

namespace highway::ui {

class UI final : public QObject {
  Q_OBJECT
public:
  explicit UI(QObject *parent = nullptr);
  UI(UI &) = delete;
  ~UI() final = default;

public slots:
  void showConnectionPropertiesForm();

private:
  QMainWindow *_mainWindowWidget;
  Ui::MainWindow *_mainWindow;
};
} // namespace highway::ui