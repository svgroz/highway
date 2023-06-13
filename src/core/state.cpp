#include "state.hpp"

using namespace highway::state;

const highway::kafka::ConsumerId State::addConsumer(Consumer &c) {
  auto consumerId = c.id();
  // _consumers.insert({consumerId, std::move(c)});
  return consumerId;
};
