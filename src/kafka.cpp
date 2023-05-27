#include "kafka.hpp"
#include <spdlog/spdlog.h>

namespace highway::kafka {

KafkaConfiguration::KafkaConfiguration(ConfType conf_type, QObject *parent)
    : QObject(parent) {
  RdKafka::Conf *c = RdKafka::Conf::create(conf_type == ConfType::CONF_GLOBAL
                                               ? RdKafka::Conf::CONF_GLOBAL
                                               : RdKafka::Conf::CONF_TOPIC);
  this->conf = std::unique_ptr<RdKafka::Conf>(c);
}

auto KafkaConfiguration::set(std::string key, std::string value) -> void {
  std::string e;
  if (conf->set(key, value, e) == RdKafka::Conf::CONF_OK) {
    return;
  } else {
    SPDLOG_ERROR(e);
    emit this->error_occured(e);
  }
};

KafkaConsumer::KafkaConsumer(std::shared_ptr<KafkaConfiguration> configuratuon,
                             QObject *parent)
    : QObject(parent), _configuration(configuratuon) {

  std::string e;
  RdKafka::KafkaConsumer *c =
      RdKafka::KafkaConsumer::create(this->_configuration->conf.get(), e);
  if (c == nullptr) {
    SPDLOG_ERROR(e);
    this->initialized = false;
  } else {
    this->_consumer = std::unique_ptr<RdKafka::KafkaConsumer>(c);
    this->initialized = true;
  }
}

auto KafkaConsumer::subscribe(std::vector<std::string> topics) -> void {
  if (topics.empty() || this->initialized == false) {
    return;
  }

  RdKafka::ErrorCode err = this->_consumer->subscribe(topics);
  if (err != RdKafka::ErrorCode::ERR_NO_ERROR) {
    auto error_message =
        fmt::format("Can't subscribe: error code={}", static_cast<int>(err));
    emit this->error_occured(error_message);
  }

  this->_topics = topics;
}

auto KafkaConsumer::name() -> std::string {
  std::string bootstrapServers;
  std::string groupId;

  _configuration->conf->get(std::string("bootstrap.servers"), bootstrapServers);
  _configuration->conf->get(std::string("group.id"), groupId);

  return fmt::format("{}:{}:{}", bootstrapServers, groupId,
                     fmt::join(this->_topics, ","));
};

KafkaConsumer::~KafkaConsumer() {
  if (this->initialized) {
    this->_consumer->close();
  }
}
} // namespace highway::kafka