#ifndef MQTT_SUB_H
#define MQTT_SUB_H

#include <QObject>

class mqtt_sub : public QObject
{
    Q_OBJECT
public:
    explicit mqtt_sub(QObject *parent = nullptr);

signals:

};

#endif // MQTT_SUB_H
