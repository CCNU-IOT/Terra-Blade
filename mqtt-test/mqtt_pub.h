#ifndef MQTT_PUB_H
#define MQTT_PUB_H

#include <QObject>

class mqtt_pub : public QObject
{
    Q_OBJECT
public:
    explicit mqtt_pub(QObject *parent = nullptr);

signals:

};

#endif // MQTT_PUB_H
