#pragma once

#include <QObject>
#include <QWidget>

#include "ui_untitled.h"

namespace highway::ui {

class UI : public QObject {
  Q_OBJECT
public:
  explicit UI(QObject *parent = nullptr);
  UI(UI &) = delete;
  virtual ~UI() = default;
  Ui::Form _form;

public slots:
  void toggle(bool);
  void addConsumer(std::string);

private:
  QWidget _widget;
};

} // namespace highway::ui