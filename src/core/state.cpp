#include "state.hpp"

using namespace highway::state;

State::State(QObject *parent) : QObject(parent) {}

const highway::kafka::ConsumerId State::addConsumer(Consumer *c) {
  auto consumerId = c->id();
  _consumers.insert(consumerId, c);
  c->setParent(this);
  return consumerId;
};