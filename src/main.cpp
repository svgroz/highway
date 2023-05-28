#include <memory>
#define SPDLOG_ACTIVE_LEVEL                                                    \
  SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <QApplication>

#include "facade.hpp"
#include "kafka.hpp"
#include "state.hpp"
#include "ui.hpp"
#include "ui_main.h"

int main(int argc, char *argv[]) {
  using namespace highway;

  spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
  SPDLOG_INFO("Starting app");

  auto app = new QApplication(argc, argv);

  auto state = std::make_shared<state::State>();
  auto facade = std::make_shared<bridge::Facade>(state);
  ui::UI ui(facade);

  return app->exec();
}
