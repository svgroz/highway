#include "kafka.hpp"

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/completion_event.hpp>
#include <boost/msm/front/euml/common.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

#include <librdkafka/rdkafkacpp.h>

#include <memory>
#include <spdlog/spdlog.h>

#include <utility>

namespace highway::kafka {
struct disconnect {};
struct connect {
  connect(std::shared_ptr<RdKafka::Conf> conf) : _conf(conf){};
  std::shared_ptr<RdKafka::Conf> _conf;
};
struct connect_successed {};
struct connect_unsuccessed {};
struct subscribe {};
struct subscribe_successed {};
struct subscribe_unsuccessed {};
struct unsubscribe {};

struct Disconnected : public boost::msm::front::state<> {};
struct Connected : public boost::msm::front::state<> {};
struct Subscribed : public boost::msm::front::state<> {};
struct Unsubscribed : public boost::msm::front::state<> {};

using namespace boost::msm::front;

struct consumer_fsm_ // TODO add destructor
    : public boost::msm::front::state_machine_def<consumer_fsm_> {
  using initial_state = Disconnected;

  std::unique_ptr<RdKafka::KafkaConsumer> _consumer =
      std::unique_ptr<RdKafka::KafkaConsumer>{};

  struct connect_action {
    template <class FSM, class SourceState, class TargetState>
    void operator()(connect const &event, FSM &fsm, SourceState &, TargetState &) {
      std::string e;
      RdKafka::KafkaConsumer *consumer =
          RdKafka::KafkaConsumer::create(event._conf.get(), e);
      if (consumer != nullptr) {
        fsm._consumer.reset(consumer);
        fsm.process_event(connect_successed());
      } else {
        SPDLOG_ERROR("Could not connect to broker: error={}", e);
        fsm.process_event(connect_unsuccessed());
      }
    }
  };

  struct disconnect_action {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &fsm, SourceState &, TargetState &) {
      if (fsm._consumer) {
        fsm._consumer.close();
        fsm._consumer = std::unique_ptr<RdKafka::KafkaConsumer>{};
      }
    }
  };

  struct subscribe_action {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &, SourceState &, TargetState &) {}
  };

  struct unsubscribe_action {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &, SourceState &, TargetState &) {}
  };

  struct transition_table
      : boost::mpl::vector<
            // clang-format off
    Row< Disconnected, connect,               none,             connect_action,     none>,
    Row< Disconnected, connect_successed,     Connected,        none,               none>,
    Row< Disconnected, connect_unsuccessed,   Disconnected,     none,               none>
            // clang-format on
            > {};
};

class consumer_fsm : public boost::msm::back::state_machine<consumer_fsm_> {};

Consumer::Consumer(ConsumerProperties &consumerProperties)
    : _consumerProperties(std::move(consumerProperties)),
      _fsm(new consumer_fsm()){};

Consumer::~Consumer() {
  if (this->_fsm) {
    delete this->_fsm;
    this->_fsm = nullptr;
  }
}

void Consumer::connect() {
  auto c = std::shared_ptr<RdKafka::Conf>{
      RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)};

  std::string e;
  for (auto p : _consumerProperties.properties) {
    if (RdKafka::Conf::ConfResult::CONF_OK != c->set(p.first, p.second, e)) {
      SPDLOG_ERROR(
          "Could not set consumer confifuration property: key={}, value={}",
          p.first, p.second);
      break;
    }
  }

  _fsm->process_event(highway::kafka::connect(c));
};

const ConsumerId Consumer::id() { return _consumerProperties.id; };

}; // namespace highway::kafka
