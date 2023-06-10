#pragma once

#include <algorithm>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "kafka.hpp"

namespace highway::state {

using Consumer = highway::kafka::Consumer;

class State final {
public:
  explicit State() = default;
  State(State &) = delete;
  State &operator=(const State &) = delete;

  State(State &&) = default;
  State &operator=(State &&) = default;

  virtual ~State() final = default;

  const highway::kafka::ConsumerId addConsumer(Consumer &c);

private:
  std::unordered_map<highway::kafka::ConsumerId, Consumer> _consumers;
};
} // namespace highway::state