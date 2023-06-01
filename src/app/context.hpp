#pragma once

#include <QObject>

#include "state.hpp"
#include "ui.hpp"

namespace highway::context {
class Context final : public QObject {
  Q_OBJECT
public:
  explicit Context(QObject *parent = nullptr);
  Context(Context &) = delete;
  Context(Context &&) = delete;
  ~Context() final = default;

private:
  highway::ui::UI *_ui;
  highway::state::State *_state;
};
} // namespace highway::context