#include "kafka.hpp"

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/completion_event.hpp>
#include <boost/msm/front/euml/common.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

#include "librdkafka/rdkafkacpp.h"

#include <spdlog/spdlog.h>

namespace highway::kafka {
struct disconnect {};
struct connect {
  connect(RdKafka::Conf *conf) : _conf(conf){};
  RdKafka::Conf *_conf;
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

struct consumer_fsm_
    : public boost::msm::front::state_machine_def<consumer_fsm_> {
  using initial_state = Disconnected;

  std::unique_ptr<RdKafka::KafkaConsumer> _consumer =
      std::unique_ptr<RdKafka::KafkaConsumer>{};

  struct connect_action {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &event, FSM &fsm, SourceState &, TargetState &) {
      std::string e;
      fsm._consumer = std::unique_ptr<RdKafka::KafkaConsumer>{
          RdKafka::Consumer::create(event._conf, e)};
      if (fsm._consumer) {
        fsm.process_event(connect_successed());
      } else {
        fsm.process_event(connect_unsuccessed());
      }
    }
  };

  struct disconnect_action {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &fsm, SourceState &, TargetState &) {
      fsm._consumer.close();
      fsm._consumer = std::unique_ptr<RdKafka::KafkaConsumer>{};
    }
  };

  struct subscribe_action {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &fsm, SourceState &, TargetState &) {}
  };

  struct unsubscribe_action {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &fsm, SourceState &, TargetState &) {}
  };

  struct transition_table
      : boost::mpl::vector<
            // clang-format off
    Row< Disconnected, connect,               none,         connect_action,     none>,
    Row< Disconnected, connect_successed,     Connected,    none,               none>,
    Row< Disconnected, connect_unsuccessed,   Disconnected, none,               none>,
            

    Row< Connected,    subscribe,             none,         none,               none>,
    Row< Connected,    subscribe_successed,   Subscribed,   none,               none>,
    Row< Connected,    subscribe_unsuccessed, Unsubscribed, none,               none>,
    Row< Connected,    disconnect,            Disconnected, disconnect_action,  none>,

    Row< Subscribed,   unsubscribe,           Unsubscribed, unsubscribe_action, none>
            // clang-format on
            > {};
};

struct consumer_fsm : boost::msm::back::state_machine<consumer_fsm_> {};

Consumer::Consumer(ConsumerProperties consumerProperties, QObject *parent)
    : QObject(parent), _consumerProperties(consumerProperties),
      _fsm(std::make_unique<consumer_fsm>()){};

void Consumer::connect(){};

const ConsumerId Consumer::id() { return _consumerProperties.id; };

}; // namespace highway::kafka
