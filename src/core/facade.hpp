#pragma once

#include "kafka.hpp"
#include "state.hpp"

#include <memory>

namespace highway::facade {

using State = highway::state::State;
using ConsumerProperties = highway::kafka::ConsumerProperties;

class Facade final {
public:
  explicit Facade();
  Facade(Facade &&) = delete;
  virtual ~Facade() final = default;

  void addConsumer(ConsumerProperties &consumerProperties);

private:
  std::unique_ptr<State> _state;
};

} // namespace highway::facade