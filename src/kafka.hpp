#pragma once

#include <QObject>
#include <librdkafka/rdkafkacpp.h>

#include <string>

namespace highway::kafka {

enum ConfType {
  CONF_GLOBAL, /**< Global configuration */
  CONF_TOPIC   /**< Topic specific configuration */
};

class KafkaConfiguration final : public QObject {
  Q_OBJECT
public:
  explicit KafkaConfiguration(ConfType, QObject *parent = nullptr);
  KafkaConfiguration(KafkaConfiguration &) = delete;
  virtual ~KafkaConfiguration() = default;
  auto set(std::string key, std::string value) -> void;
  std::unique_ptr<RdKafka::Conf> conf;
signals:
  auto error_occured(std::string error) -> void;
};

class KafkaConsumer final : public QObject {
  Q_OBJECT
public:
  explicit KafkaConsumer(std::shared_ptr<KafkaConfiguration>,
                         QObject *parent = nullptr);
  KafkaConsumer(KafkaConsumer &) = delete;
  virtual ~KafkaConsumer();

  auto name() -> std::string;

public slots:
  auto subscribe(std::vector<std::string> topics) -> void;

signals:
  auto error_occured(std::string) -> void;

private:
  bool initialized = false;
  std::shared_ptr<KafkaConfiguration> _configuration;
  std::vector<std::string> _topics;
  std::unique_ptr<RdKafka::KafkaConsumer> _consumer;
};
}; // namespace highway::kafka
