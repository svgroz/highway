#include "facade.hpp"

#include <memory>

#include <spdlog/spdlog.h>

#include "kafka.hpp"

using namespace highway::facade;

Facade::Facade() : _state(std::make_unique<State>()){};

void Facade::addConsumer(ConsumerProperties &consumerProperties) {
  auto consumer = highway::kafka::Consumer(consumerProperties);

  auto id = _state->addConsumer(consumer);
}
