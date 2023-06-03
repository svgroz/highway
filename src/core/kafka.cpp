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
      _conf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)) {

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
                consumerProperties.id.toStdString(), e);
  }
}

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

bool Consumer::connect() {
  if (this->_consumer != nullptr) {
    return true;
  }

  std::string e;
  _consumer = RdKafka::KafkaConsumer::create(_conf, e);
  if (_consumer == nullptr) {
    SPDLOG_WARN("Could not connect consumer: consumer id={} error message={}",
                _consumerProperties.id.toStdString(), e);
    return false;
  }

  auto topics = std::vector<std::string>();
  for (const auto &topic : _consumerProperties.topics) {
    std::string v = topic.toStdString();
    topics.push_back(v);
  }

  _consumer->subscribe(topics);

  return true;
}

bool Consumer::isConnected() { return this->_consumer == nullptr; }

const ConsumerId Consumer::id() { return _consumerProperties.id; }
