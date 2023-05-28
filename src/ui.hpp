#pragma once

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <memory>
#include <unordered_map>

#include "facade.hpp"

#include "ui_kafka_consumer_properties.h"
#include "ui_main_window.h"

namespace highway::ui {

typedef std::shared_ptr<highway::bridge::Facade> Facade;

class UI : public QObject {
  Q_OBJECT
public:
  explicit UI(Facade facade,
              QObject *parent = nullptr);
  UI(UI &) = delete;
  virtual ~UI() = default;
  Ui::MainWindow _mainWindowUI;
  Ui::AddKafkaConsumerForm _addConsumerForm;

public slots:
  void showAddConsumerWindow();
  void parseConsumerParameters(bool);

private:
  QMainWindow _mainWindow;
  bool _isConsumerWindowAvailable;
  QWidget _addConsumerWindow;
  Facade _facade;
};

} // namespace highway::ui