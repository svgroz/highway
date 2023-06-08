#define SPDLOG_ACTIVE_LEVEL                                                    \
  SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

#include <QApplication>

#include "ui.hpp"

#include <facade.hpp>


using namespace highway;

int main(int argc, char **argv) {
  spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
  SPDLOG_INFO("Starting app");

  QApplication app(argc, argv);

  auto facade = new highway::facade::Facade(&app);
  auto ui = new ui::UI(facade, &app);
  
  return app.exec();
}
