#pragma once

#include "ui_connection_properties.h"
#include "ui_main_window.h"

#include <facade.hpp>

#include <QMainWindow>

namespace highway::ui {

class UI final : public QObject {
  Q_OBJECT
public:
  explicit UI(highway::facade::Facade *facade, QObject *parent = nullptr);
  UI(UI &) = delete;
  ~UI() final = default;

public slots:
  void showConnectionPropertiesForm();

private:
  highway::facade::Facade *_facade;
  QMainWindow _mainWindowWidget;
  Ui::MainWindow _mainWindow;

  void saveConnectionProperties(
      Ui::ConnectionPropertiesDialog _connectionProperties);
};
} // namespace highway::ui