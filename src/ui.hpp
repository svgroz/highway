#pragma once

#include "ui_connection_properties.h"
#include "ui_main_window.h"

#include <QMainWindow>
#include <QWidget>

#include <functional>

namespace highway::ui {



class UI : public QObject {
  Q_OBJECT
public:
  explicit UI(QObject *parent = nullptr);
  UI(UI &) = delete;
  virtual ~UI() = default;

public slots:
  void showConnectionPropertiesForm();
  void connectionPropertiesFormDestroyed(QObject *);

private:
  QMainWindow *_mainWindowWidget;
  QWidget *_connectionProperiesWidget;
  Ui::MainWindow *_mainWindow;
  Ui::ConnectionPropertiesForm *_connectionPropertiesForm;
};
} // namespace highway::ui