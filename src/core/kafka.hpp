#pragma once

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

namespace highway::kafka {

using ConsumerProperties = struct {
  QString id;
  QList<QString> topics;
  QHash<QString, QString> properties;
};

struct consumer_fsm;

using ConsumerId = QString;

class Consumer final : public QObject {
  Q_OBJECT
public:
  explicit Consumer(ConsumerProperties &properties, QObject *parent = nullptr);
  Consumer(Consumer &) = delete;
  ~Consumer() final;
  void connect();
  const ConsumerId id();

private:
  ConsumerProperties _consumerProperties;
  consumer_fsm *_fsm;
};

}; // namespace highway::kafka
