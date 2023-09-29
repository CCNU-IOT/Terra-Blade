#ifndef MQTT_PUB_H
#define MQTT_PUB_H

#include <QObject>
#include <iostream>
#include <mutex>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QLineEdit>
#include <MQTTClient.h>
#include <QTimer>

class mqtt_pub : public QObject
{
    Q_OBJECT
public:
    explicit mqtt_pub(QObject *parent = nullptr);
    ~mqtt_pub();
private:
    std::string pub_topic;
    std::string pub_ip;
    std::string pub_host;
    std::string pub_clientid;
    int pub_qos;
    std::string pub_mode;
    std::string pub_message;
    MQTTClient pub_client;
    MQTTClient_deliveryToken token;
    MQTTClient_message pubmsg;
    MQTTClient_connectOptions conn_opts;
    long int pub_timeout;
    int pub_rc;
    QTimer *pub_timer;
public:
    QLineEdit *mainwindow_line_mqtt_pub_ip;
    QLineEdit *mainwindow_line_mqtt_pub_host;
    QLineEdit *mainwindow_line_mqtt_pub_clientid;
    QLineEdit *mainwindow_line_mqtt_pub_topic;
    QLineEdit *mainwindow_line_mqtt_pub_message;
    QPushButton *mainwindow_button_mqtt_pub_connect;
    QPushButton *mainwindow_button_mqtt_pub_publish;
    QComboBox *mainwindow_combox_mqtt_pub_mode;
    QComboBox *mainwindow_combox_mqtt_pub_qos;
public:
    bool mqtt_pub_thread_connect();
    bool mqtt_pub_thread_disconnect();
    bool mqtt_pub_thread_publish();
    bool mqtt_pub_thread_closepublish();
private:
    bool mqtt_pub_thread_signal_send();
    bool mqtt_pub_thread_send_once_0_1s();
    bool mqtt_pub_thread_send_once_1s();
    bool mqtt_pub_thread_send_once_10s();
signals:
private slots:

};

#endif // MQTT_PUB_H
