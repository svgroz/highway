#pragma once

#include "kafka.hpp"
#include <QObject>
#include <iostream>

namespace highway::state {

typedef std::shared_ptr<highway::kafka::KafkaConsumer> KConsumer;

class State : public QObject {
  Q_OBJECT
public:
  explicit State(QObject *parent = nullptr);
  State(State &) = delete;
  virtual ~State() = default;

public slots:
  auto addKafkaConsumer(KConsumer) -> void;

signals:
  void kafkaConsumerAdded(std::string);

private:
  std::vector<KConsumer> consumers;
};
} // namespace highway::state