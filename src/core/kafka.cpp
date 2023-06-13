#include "kafka.hpp"

#include <boost/msm/back/common_types.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/completion_event.hpp>
#include <boost/msm/front/euml/common.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

#include <librdkafka/rdkafkacpp.h>

#include <memory>
#include <mutex>
#include <spdlog/spdlog.h>

#include <utility>

namespace highway::kafka {
struct e_disconnect {};
struct e_connect {};
struct e_subscribe {};
struct e_unsubscribe {};

struct WaitingForDisconnect : public boost::msm::front::state<> {};
struct Disconnected : public boost::msm::front::state<> {};
struct WaitingForConnection : public boost::msm::front::state<> {};
struct Connected : public boost::msm::front::state<> {};
struct WaitingForSubscribtion : public boost::msm::front::state<> {};
struct Subscribed : public boost::msm::front::state<> {};
struct WaitingForUnsubscribtion : public boost::msm::front::state<> {};
struct Unsubscribed : public boost::msm::front::state<> {};

using namespace boost::msm::front;

const auto HADLED = boost::msm::back::HandledEnum::HANDLED_TRUE;

struct consumer_fsm_ // TODO add destructor
    : public boost::msm::front::state_machine_def<consumer_fsm_> {
  using initial_state = Disconnected;

  struct transition_table
      : boost::mpl::vector<
            Row<Disconnected, e_disconnect, Disconnected, none, none>,
            Row<Disconnected, e_connect, WaitingForConnection, none, none>,

            Row<WaitingForConnection, e_connect, Connected, none, none>,
            Row<WaitingForConnection, e_disconnect, Disconnected, none, none>,

            Row<Connected, e_subscribe, WaitingForSubscribtion, none, none>,
            Row<Connected, e_disconnect, WaitingForDisconnect, none, none>,

            Row<WaitingForSubscribtion, e_subscribe, Subscribed, none, none>,
            Row<WaitingForSubscribtion, e_unsubscribe, Subscribed, none,
                none>> {};

  template <class FSM, class Event>
  void no_transition(Event const &e, FSM &, int state) {
    SPDLOG_WARN("no transition from state on event: state={} event={}", state,
                typeid(e).name());
  }
};

class consumer_fsm : public boost::msm::back::state_machine<consumer_fsm_> {};

Consumer::Consumer(ConsumerProperties consumerProperties)
    : _mutex(), _consumerProperties(consumerProperties),
      _fsm(new consumer_fsm()), _kafkaConsumer(nullptr){};

Consumer::~Consumer() {
  if (this->_fsm) {
    delete this->_fsm;
    this->_fsm = nullptr;
  }
}

ConsumerCodes Consumer::connect() {
  std::unique_lock<std::mutex> lock(_mutex);
  if (HADLED != _fsm->process_event(e_connect())) {
    return ConsumerCodes::TARGET_STATE_UNREACHEBLE;
  }

  std::shared_ptr<RdKafka::Conf> conf = std::shared_ptr<RdKafka::Conf>{
      RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)};

  std::string e;
  for (auto p : _consumerProperties.properties) {
    if (RdKafka::Conf::ConfResult::CONF_OK != conf->set(p.first, p.second, e)) {
      SPDLOG_ERROR(
          "Could not set consumer confifuration property: key={} value={}",
          p.first, p.second);
    }
  }

  auto consumer = std::unique_ptr<RdKafka::KafkaConsumer>{
      RdKafka::KafkaConsumer::create(conf.get(), e)};

  if (consumer) {
    this->_kafkaConsumer.swap(consumer);
    _fsm->process_event(e_connect());
    return ConsumerCodes::OK;
  } else {
    _fsm->process_event(e_disconnect());
    return ConsumerCodes::COULD_NOT_CONNECT;
  }
};

const ConsumerId Consumer::id() noexcept { return _consumerProperties.id; };

}; // namespace highway::kafka
