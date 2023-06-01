#define SPDLOG_ACTIVE_LEVEL                                                    \
  SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include "context.hpp"
#include <QApplication>
#include <memory>

using namespace highway;

int main(int argc, char **argv) {
  spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
  SPDLOG_INFO("Starting app");

  QApplication app(argc, argv);
  auto ctx = std::make_shared<context::Context>();
  return app.exec();
}
