#pragma once

#include "kafka.hpp"
#include "state.hpp"

#include <QObject>

namespace highway::facade {

using State = highway::state::State;
using ConsumerProperties = highway::kafka::ConsumerProperties;

using ConnectionStatus = struct {
  highway::kafka::ConsumerId consumerId;
  highway::kafka::ConsumerStatus consumerStatus;
};

class Facade final : public QObject {
  Q_OBJECT
public:
  explicit Facade(QObject *parent = nullptr);
  Facade(Facade &&) = delete;
  ~Facade() final = default;

  ConnectionStatus addConsumer(ConsumerProperties consumerProperties);

private:
  State *_state;
};

} // namespace highway::facade