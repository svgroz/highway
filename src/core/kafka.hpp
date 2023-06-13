#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <librdkafka/rdkafkacpp.h>

namespace highway::kafka {

using ConsumerProperties = struct {
  std::string id;
  std::vector<std::string> topics;
  std::unordered_map<std::string, std::string> properties;
};

class consumer_fsm;

using ConsumerId = std::string;

using ConsumerCodes = enum { OK, COULD_NOT_CONNECT, TARGET_STATE_UNREACHEBLE };

class Consumer final {
public:
  explicit Consumer(ConsumerProperties properties);

  virtual ~Consumer() final;
  ConsumerCodes connect();
  const ConsumerId id() noexcept;

private:
  std::mutex _mutex;
  ConsumerProperties _consumerProperties;
  consumer_fsm *_fsm;
  std::unique_ptr<RdKafka::KafkaConsumer> _kafkaConsumer;
};

}; // namespace highway::kafka
