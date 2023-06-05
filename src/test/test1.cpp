// Copyright 2010 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl
// library Distributed under the same license as the original. Copyright for the
// original version: Copyright 2005 David Abrahams and Aleksey Gurtovoy.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
// front-end
#include <boost/msm/front/euml/common.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>

using namespace std;
namespace msm = boost::msm;
using namespace msm::front;
namespace mpl = boost::mpl;
// for And_ operator
using namespace msm::front::euml;

namespace t {
// events
struct disconnect {};
struct connect {};
struct subscribe {};
struct unsubscribe {};

struct consumer_ : public msm::front::state_machine_def<consumer_> {
  struct Disconnected : public msm::front::state<> {};
  struct Connected : public msm::front::state<> {};
  struct Subscribed : public msm::front::state<> {};
  struct Unsubscribed : public msm::front::state<> {};

  using initial_state = Disconnected;

  struct connect_a {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &fsm, SourceState &, TargetState &) {
      cout << "transition with event:" << typeid(EVT).name() << endl;
      fsm.process_event(disconnect());
    }
  };

  struct subscribe_a {
    template <class EVT, class FSM, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &, SourceState &, TargetState &) {
      cout << "transition with event:" << typeid(EVT).name() << endl;
    }
  };
  // clang-format off
  struct transition_table : mpl::vector<
    Row < Disconnected, connect,   Disconnected,  connect_a,   none >,
    Row < Disconnected, connect,   Connected,     connect_a,   none >,
    Row < Connected,    subscribe, Subscribed,    subscribe_a, none >,
    Row < Connected,    disconnect, Disconnected,    subscribe_a, none >
  >{};
  // clang-format on
};
using consumer = msm::back::state_machine<consumer_>;
}; // namespace t

int main() {
  t::consumer c;

  c.process_event(t::connect());
  return 0;
}