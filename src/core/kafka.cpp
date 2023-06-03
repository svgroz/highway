#include "kafka.hpp"

#include <memory>
#include <qhash.h>
#include <spdlog/spdlog.h>
#include <string>
#include <utility>
#include <vector>

#include <QString>

#include "librdkafka/rdkafkacpp.h"

using namespace highway::kafka;

Consumer::Consumer(ConsumerProperties consumerProperties, QObject *parent)
    : QObject(parent), _consumerProperties(consumerProperties),
      _conf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)),
      _consumer(nullptr) {}

Consumer::~Consumer() {
  if (this->_consumer != nullptr) {
    auto consumerId = this->id().toStdString();
    SPDLOG_INFO("Consumer closing is strarted: consumer id={}", consumerId);
    this->_consumer->close();
    delete this->_consumer;
    this->_consumer = nullptr;
    SPDLOG_INFO("Consumer closed: conusmer id={}", consumerId);
  }

  if (this->_conf != nullptr) {
    delete this->_conf;
    this->_conf = nullptr;
  }
}

void Consumer::consumerProperties(ConsumerProperties consumerProperties) {
  this->_consumerProperties = consumerProperties;
}

ConsumerStatus Consumer::connect() {
  if (this->_consumer != nullptr) {
    return ConsumerStatus::CONNECTED;
  }

  std::string e;

  QHashIterator<QString, QString> iterator(_consumerProperties.properties);
  while (iterator.hasNext()) {
    auto item = iterator.next();
    auto k = item.key().toStdString();
    auto v = item.value().toStdString();

    if (RdKafka::Conf::ConfResult::CONF_OK != _conf->set(k, v, e)) {
      SPDLOG_WARN(
          "Unexpected consumer consiguration: key={} value={} error message={}",
          k, v, e);
    }
  }

  _consumer = RdKafka::KafkaConsumer::create(_conf, e);
  if (_consumer == nullptr) {
    SPDLOG_WARN("Could not connect consumer: consumer id={} error message={}",
                _consumerProperties.id.toStdString(), e);
    return ConsumerStatus::NOT_INITIALIZED;
  }

  auto topics = std::vector<std::string>();
  for (const auto &topic : _consumerProperties.topics) {
    std::string v = topic.toStdString();
    topics.push_back(v);
  }

  if (RdKafka::ErrorCode::ERR_NO_ERROR == _consumer->subscribe(topics)) {
    return ConsumerStatus::CONNECTED;
  } else {
    return ConsumerStatus::DISCONNECTED;
  }
}

ConsumerStatus Consumer::disocnnect() {
  if (this->_consumer == nullptr) {
    return ConsumerStatus::NOT_INITIALIZED;
  }

  this->_consumer->close();
  delete this->_consumer;
  this->_consumer = nullptr;
  return ConsumerStatus::DISCONNECTED;
}

ConsumerStatus Consumer::status() noexcept {
  return this->_consumer == nullptr ? ConsumerStatus::DISCONNECTED
                                    : ConsumerStatus::CONNECTED;
}

const ConsumerId Consumer::id() { return _consumerProperties.id; }
