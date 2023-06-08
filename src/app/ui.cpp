#include "ui.hpp"

#include "facade.hpp"
#include "ui_connection_properties.h"
#include "ui_main_window.h"

#include <QDialog>
#include <QHash>
#include <QObject>
#include <QPushButton>
#include <QRegularExpression>
#include <QString>
#include <QValidator>
#include <QWidget>

#include <functional>
#include <memory>

#include <spdlog/spdlog.h>
#include <vector>

using namespace highway::ui;

UI::UI(std::shared_ptr<highway::facade::Facade> facade, QObject *parent)
    : _facade(facade), QObject(parent), _mainWindowWidget(), _mainWindow() {

  _mainWindow.setupUi(&_mainWindowWidget);

  QObject::connect(_mainWindow.newConnectionButton, &QPushButton::clicked, this,
                   &UI::showConnectionPropertiesForm);

  _mainWindowWidget.show();
}

void UI::saveConnectionProperties(
    Ui::ConnectionPropertiesDialog _connectionProperties) {
  auto connectionId = _connectionProperties.connectionId->text().toStdString();
  auto topics = _connectionProperties.topicsToListen->text().toStdString();

  auto table = _connectionProperties.propertiesTableWidget;

  std::unordered_map<std::string, std::string> properties;

  const auto rowCount = table->rowCount();

  for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
    QString key = table->item(rowIndex, 0)->text();
    QString value = table->item(rowIndex, 1)->text();
    properties.insert({key.toStdString(), value.toStdString()});
  }

  auto consumerProperties = highway::facade::ConsumerProperties{
      .id = connectionId,
      .topics = std::vector<std::string>{topics},
      .properties = properties};

  // auto [consumerId, consumerStatus] =
  _facade->addConsumer(consumerProperties);

  // auto i = new QListWidgetItem(this->_mainWindow.connectionListWidget);
  // i->setText(consumerId);
}

void UI::showConnectionPropertiesForm() {
  auto _connectionProperiesDialog = new QDialog(&this->_mainWindowWidget);
  _connectionProperiesDialog->setAttribute(Qt::WA_DeleteOnClose, true);

  auto connectionPropertiesDialog = Ui::ConnectionPropertiesDialog();
  connectionPropertiesDialog.setupUi(_connectionProperiesDialog);

  auto topicsRegex = QRegularExpression("[a-zA-z0-9,_]+");
  auto topicsValidator =
      new QRegularExpressionValidator(topicsRegex, _connectionProperiesDialog);

  connectionPropertiesDialog.connectionId->setValidator(topicsValidator);

  QObject::connect(
      connectionPropertiesDialog.buttonBox, &QDialogButtonBox::accepted, this,
      [=]() { this->saveConnectionProperties(connectionPropertiesDialog); });

  _connectionProperiesDialog->show();

  return;
}
