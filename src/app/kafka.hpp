#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace highway::kafka {

class ConsumerProperties final {
public:
  explicit ConsumerProperties() = default;
  ConsumerProperties(ConsumerProperties &) = default;
  ConsumerProperties(ConsumerProperties &&) = default;
  virtual ~ConsumerProperties() final = default;
  std::string id;
  std::vector<std::string> topics;
  std::unordered_map<std::string, std::string> properties;
};

}; // namespace highway::kafka