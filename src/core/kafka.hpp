#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace highway::kafka {

using ConsumerProperties = struct {
  std::string id;
  std::vector<std::string> topics;
  std::unordered_map<std::string, std::string> properties;
};

struct consumer_fsm;

using ConsumerId = std::string;

class Consumer {
public:
  explicit Consumer(ConsumerProperties &properties);
  Consumer(Consumer &) = delete;
  Consumer &operator=(const Consumer &) = delete;

  Consumer(Consumer &&) = default;

  virtual ~Consumer();
  void connect();
  const ConsumerId id();

private:
  ConsumerProperties _consumerProperties;
  consumer_fsm *_fsm;
};

}; // namespace highway::kafka
