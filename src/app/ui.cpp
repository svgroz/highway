#include "ui.hpp"
#include "ui_connection_properties.h"
#include "ui_main_window.h"

#include <QObject>
#include <QPushButton>
#include <QWidget>

#include <functional>

#include <qdialog.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qregularexpression.h>
#include <qvalidator.h>
#include <spdlog/spdlog.h>
#include <unordered_map>

using namespace highway::ui;

void saveConnectionProperties(
    Ui::ConnectionPropertiesDialog *_connectionProperties) {
  auto connectionId = _connectionProperties->connectionId->text().toStdString();
  auto topics = _connectionProperties->topicsToListen->text().toStdString();

  auto table = _connectionProperties->propertiesTableWidget;

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

UI::UI(QObject *parent)
    : QObject(parent), _mainWindowWidget(new QMainWindow()),
      _mainWindow(new Ui::MainWindow()) {

  _mainWindow->setupUi(_mainWindowWidget);
  _mainWindowWidget->show();

  QObject::connect(_mainWindow->newConnectionButton, &QPushButton::clicked,
                   this, &UI::showConnectionPropertiesForm);
}

void UI::showConnectionPropertiesForm() {
  auto _connectionProperiesWidget = new QDialog(this->_mainWindowWidget);
  _connectionProperiesWidget->setAttribute(Qt::WA_DeleteOnClose, true);

  auto _connectionPropertiesDialog = new Ui::ConnectionPropertiesDialog();
  _connectionPropertiesDialog->setupUi(_connectionProperiesWidget);

  auto topicsRegex = QRegularExpression("[a-zA-z0-9,_]+");
  auto topicsValidator =
      new QRegularExpressionValidator(topicsRegex, _connectionProperiesWidget);

  _connectionPropertiesDialog->connectionId->setValidator(topicsValidator);

  QObject::connect(_connectionPropertiesDialog->buttonBox,
                   &QDialogButtonBox::accepted, this,
                   [_connectionPropertiesDialog]() {
                     saveConnectionProperties(_connectionPropertiesDialog);
                   });

  _connectionProperiesWidget->show();

  return;
}
