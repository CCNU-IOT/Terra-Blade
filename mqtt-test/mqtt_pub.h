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
#include <chrono>
#include <sstream>
#include <iomanip>
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
    MQTTClient pub_client;
    MQTTClient_deliveryToken token;
    MQTTClient_message pubmsg;
    MQTTClient_connectOptions conn_opts;
    long int pub_timeout;
    int pub_rc;
    QTimer *pub_timer;
    std::mutex my_lock;
public:
    std::string pub_message;
    QLineEdit *mainwindow_line_mqtt_pub_ip;
    QLineEdit *mainwindow_line_mqtt_pub_host;
    QLineEdit *mainwindow_line_mqtt_pub_clientid;
    QLineEdit *mainwindow_line_mqtt_pub_topic;
    QLineEdit *mainwindow_line_mqtt_pub_message;
    QPushButton *mainwindow_button_mqtt_pub_connect;
    QPushButton *mainwindow_button_mqtt_pub_publish;
    QComboBox *mainwindow_combox_mqtt_pub_mode;
    QComboBox *mainwindow_combox_mqtt_pub_qos;
    QTextEdit *mainwindow_tedit_mqtt_pub_text;
public:
    bool mqtt_pub_thread_connect();
    bool mqtt_pub_thread_disconnect();
    bool mqtt_pub_thread_publish();
    void mqtt_pub_thread_closepublish();
private:
    void mqtt_pub_thread_signal_send();
    std::string mqtt_pub_thread_pub_time();
signals:
private slots:

};

#endif // MQTT_PUB_H
