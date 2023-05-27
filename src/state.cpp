#include "state.hpp"
#include <spdlog/spdlog.h>

using namespace highway::state;

State::State(QObject *parent) : QObject(parent), consumers(){};

void State::addKafkaConsumer(KConsumer kConsumer) {
  this->consumers.push_back(kConsumer);
  auto name = kConsumer->name();
  SPDLOG_INFO("Kafka consumer added: consumer={}", name);
  emit this->kafkaConsumerAdded(name);
};
