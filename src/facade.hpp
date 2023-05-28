#pragma once

#include "facade.hpp"
#include "kafka.hpp"
#include "state.hpp"

#include <QObject>
#include <memory>

namespace highway::bridge {

class Facade : public QObject {
  Q_OBJECT
public:
  Facade(std::shared_ptr<highway::state::State> state,
         QObject *parent = nullptr);
  Facade(Facade &) = delete;
  state::KConsumer
  addKafkaConsumer(std::unordered_map<std::string, std::string> configuration);

private:
  std::shared_ptr<highway::state::State> _state;
};

} // namespace highway::bridge