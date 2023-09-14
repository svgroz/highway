#pragma once

#include <boost/msm/back/common_types.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/completion_event.hpp>
#include <boost/msm/front/euml/common.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

#include <spdlog/spdlog.h>

namespace higway::fsm::consumer {
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

struct consumer_fsm_
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
} // namespace higway::fsm::consumer