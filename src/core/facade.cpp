#include "facade.hpp"
#include "kafka.hpp"
#include <qobject.h>
#include <spdlog/spdlog.h>

using namespace highway::facade;

Facade::Facade(QObject *parent) : QObject(parent), _state(new State(this)) {}

ConnectionStatus Facade::addConsumer(ConsumerProperties consumerProperties) {
  auto consumer = new highway::kafka::Consumer(consumerProperties, this);
  auto id = _state->addConsumer(consumer);
  bool connected = consumer->connect();
  return {.consumerId = id, .isConnected = connected};
}
