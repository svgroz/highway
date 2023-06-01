#include "ui.hpp"
#include "ui_connection_properties.h"
#include "ui_main_window.h"

#include <QObject>
#include <QPushButton>
#include <QWidget>

#include <functional>

#include <qobject.h>
#include <qpushbutton.h>
#include <spdlog/spdlog.h>
#include <unordered_map>

using namespace highway::ui;

UI::UI(QObject *parent)
    : QObject(parent), _mainWindowWidget(new QMainWindow()),
      _connectionProperiesWidget(nullptr), _mainWindow(new Ui::MainWindow()),
      _connectionPropertiesForm(nullptr) {
  _mainWindow->setupUi(_mainWindowWidget);
  _mainWindowWidget->show();

  QObject::connect(_mainWindow->newConnectionButton, &QPushButton::clicked,
                   this, &UI::showConnectionPropertiesForm);
}

void UI::showConnectionPropertiesForm() {
  if (_connectionProperiesWidget == nullptr) {
    _connectionProperiesWidget = new QWidget();
    _connectionProperiesWidget->setAttribute(Qt::WA_DeleteOnClose, true);

    _connectionPropertiesForm = new Ui::ConnectionPropertiesForm();

    QObject::connect(_connectionProperiesWidget, &QWidget::destroyed, this,
                     &UI::connectionPropertiesFormDestroyed);
    _connectionPropertiesForm->setupUi(_connectionProperiesWidget);
    QObject::connect(_connectionPropertiesForm->saveButton, &QPushButton::clicked, this, &UI::saveConnectionProperties);

    _connectionProperiesWidget->show();
    return;
  }

  if (_connectionProperiesWidget->isHidden() ||
      _connectionProperiesWidget->isActiveWindow() == false) {
    _connectionProperiesWidget->show();
    _connectionProperiesWidget->activateWindow();
    return;
  }
}

void UI::connectionPropertiesFormDestroyed(QObject *) {
  if (_connectionPropertiesForm != nullptr) {
    delete _connectionPropertiesForm;
    _connectionPropertiesForm = nullptr;
  }

  _connectionProperiesWidget = nullptr;
}

void UI::saveConnectionProperties(bool) {
  auto connectionId = _connectionPropertiesForm->connectionId->text().toStdString();
  auto topics = _connectionPropertiesForm->topics->text().toStdString();

  auto table = _connectionPropertiesForm->connectionPropertiesTableWidget;

  auto r = std::unordered_map<std::string, std::string>();

  const auto rowCount = table->rowCount();

  SPDLOG_INFO("Adding new connection {} {}", connectionId, topics);
  for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
    const auto key = table->item(rowIndex, 0)->text().toStdString();
    const auto value = table->item(rowIndex, 1)->text().toStdString();
    SPDLOG_INFO("Connection propery: {}={}", key, value);
    r.insert({key, value});
  }
}
