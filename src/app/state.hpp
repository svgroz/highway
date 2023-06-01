#pragma once

#include <QObject>
#include <qobject.h>
#include <qtmetamacros.h>

namespace highway::state {

class State : public QObject {
  Q_OBJECT
public:
  explicit State(QObject *parent = nullptr);
  State(State &) = delete;
  virtual ~State() = default;
};
} // namespace highway::state