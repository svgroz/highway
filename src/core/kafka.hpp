#pragma once

#include <qobject.h>
#include <qtmetamacros.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

#include <librdkafka/rdkafkacpp.h>

namespace highway::kafka {

using ConsumerProperties = struct {
  QString id;
  QList<QString> topics;
  QHash<QString, QString> properties;
};

using ConsumerStatus = enum { CONNECTED, DISCONNECTED, NOT_INITIALIZED };

using ConsumerId = QString;

class Consumer final : public QObject {
  Q_OBJECT
public:
  explicit Consumer(ConsumerProperties properties, QObject *parent = nullptr);
  Consumer(Consumer &) = delete;
  ~Consumer() override;
  void consumerProperties(ConsumerProperties consumerProperties);
  ConsumerStatus connect();
  ConsumerStatus disocnnect();
  ConsumerStatus status() noexcept;
  const ConsumerId id();

private:
  ConsumerProperties _consumerProperties;
  RdKafka::Conf *_conf;
  RdKafka::KafkaConsumer *_consumer;
};

}; // namespace highway::kafka
