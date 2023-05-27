#include "ui.hpp"

using namespace highway::ui;

UI::UI(QObject *parent)
    : QObject(parent), _widget(QWidget()), _form(Ui::Form()) {
  _form.setupUi(&_widget);
}

void UI::toggle(bool val) {
  if (val) {
    _widget.show();
  } else {
    _widget.hide();
  }
}

void UI::addConsumer(std::string consumer) {
  _form.listWidget->addItem(QString::fromStdString(consumer));
}
