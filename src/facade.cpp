#include "facade.hpp"
#include "state.hpp"
#include <qobject.h>
#include <string>
#include <unordered_map>

using namespace highway::bridge;

Facade::Facade(std::shared_ptr<highway::state::State> state, QObject *parent)
    : QObject(parent), _state(state){};

highway::state::KConsumer Facade::addKafkaConsumer(
    std::unordered_map<std::string, std::string> configuration) {

  auto kc = std::make_shared<highway::kafka::KafkaConfiguration>(
      highway::kafka::CONF_GLOBAL);

  for (std::pair<std::string, std::string> kv : configuration) {
    kc->set(kv.first, kv.second);
  }

  auto kConsumer = std::make_shared<highway::kafka::KafkaConsumer>(kc);

  _state->addKafkaConsumer(kConsumer);

  return kConsumer;
};