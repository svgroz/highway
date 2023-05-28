#include "ui.hpp"
#include "ui_kafka_consumer_properties.h"
#include "ui_main_window.h"

#include <QTreeWidget>

#include <memory>
#include <string>
#include <unordered_map>

using namespace highway::ui;

UI::UI(Facade facade, QObject *parent)
    : _facade(facade), QObject(parent), _isConsumerWindowAvailable(false) {

  _mainWindowUI.setupUi(&_mainWindow);
  _addConsumerForm.setupUi(&_addConsumerWindow);

  QObject::connect(_mainWindowUI.actionAdd_connection, &QAction::triggered,
                   this, &UI::UI::showAddConsumerWindow);

  QObject::connect(_addConsumerForm.saveButton, &QPushButton::clicked, this,
                   &UI::UI::parseConsumerParameters);

  _addConsumerWindow.hide();
  _mainWindow.show();
}

void UI::showAddConsumerWindow() {
  if (_addConsumerWindow.isHidden() == true) {
    _addConsumerWindow.show();
  } else {
    _addConsumerWindow.activateWindow();
  }
}

void UI::parseConsumerParameters(bool) {
  std::unordered_map<std::string, std::string> rawConsumerProperties = {};

  auto rowCount = _addConsumerForm.kafkaConsumerPropertiesTable->rowCount();
  for (int i = 0; i < rowCount; i++) {
    auto key = _addConsumerForm.kafkaConsumerPropertiesTable->item(i, 0);
    auto value = _addConsumerForm.kafkaConsumerPropertiesTable->item(i, 1);
    if (key && value) {
      std::string keyValue = key->text().toStdString();
      if (rawConsumerProperties.contains(keyValue) == false) {
        std::string valueValue = value->text().toStdString();
        rawConsumerProperties.insert({keyValue, valueValue});
      }
    }
  }

  auto consumer = _facade->addKafkaConsumer(rawConsumerProperties);

  auto consumesBranch = _mainWindowUI.treeWidget->topLevelItem(0);

  auto item = new QTreeWidgetItem(consumesBranch);
  item->setText(0, QString::fromStdString(consumer->name()));

  consumesBranch->addChild(item);
}
