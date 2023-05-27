#define SPDLOG_ACTIVE_LEVEL                                                    \
  SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <QApplication>

#include "kafka.hpp"
#include "state.hpp"
#include "ui.hpp"

int main(int argc, char *argv[]) {
  using namespace highway;

  spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
  SPDLOG_INFO("Starting app");

  auto app = new QApplication(argc, argv);

  ui::UI ui;
  state::State state;
  kafka::TL tl;

  QObject::connect(ui._form.addConnectionButton, &QPushButton::clicked, &tl,
                   &kafka::TL::t2);

  QObject::connect(&tl, &kafka::TL::t1, &state,
                   &state::State::addKafkaConsumer);

  QObject::connect(&state, &state::State::kafkaConsumerAdded, &ui,
                   &ui::UI::addConsumer);

  ui.toggle(true);

  return app->exec();
}
