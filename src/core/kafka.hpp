#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace highway::kafka {

using ConsumerProperties = struct {
  std::string id;
  std::vector<std::string> topics;
  std::unordered_map<std::string, std::string> properties;
};

class consumer_fsm;

using ConsumerId = std::string;

class Consumer final {
public:
  explicit Consumer(ConsumerProperties &properties);
  Consumer(Consumer &) = delete;
  Consumer &operator=(const Consumer &) = delete;

  Consumer(Consumer &&) = default;
  Consumer &operator=(Consumer &&) = default;

  virtual ~Consumer() final;
  void connect();
  const ConsumerId id();

private:
  ConsumerProperties _consumerProperties;
  consumer_fsm *_fsm;
};

}; // namespace highway::kafka
