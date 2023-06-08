#include "facade.hpp"
#include "kafka.hpp"
#include <qobject.h>
#include <spdlog/spdlog.h>

using namespace highway::facade;

Facade::Facade(QObject *parent) : QObject(parent), _state(new State(this)) {}

void Facade::addConsumer(ConsumerProperties consumerProperties) {
  auto consumer = new highway::kafka::Consumer(consumerProperties, this);

  auto id = _state->addConsumer(consumer);
  consumer->connect();
}
