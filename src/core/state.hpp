#pragma once

#include <QList>
#include <QObject>
#include <algorithm>
#include <qhash.h>
#include <spdlog/spdlog.h>

#include "kafka.hpp"

namespace highway::state {

using Consumer = highway::kafka::Consumer;

class State final : public QObject {
  Q_OBJECT
public:
  explicit State(QObject *parent = nullptr);
  State(State &) = delete;
  State(State &&) = delete;
  ~State() final {
    SPDLOG_INFO("Closing state");
  };
  const highway::kafka::ConsumerId addConsumer(Consumer *c);

private:
  QHash<const QString, Consumer *> _consumers;
};
} // namespace highway::state