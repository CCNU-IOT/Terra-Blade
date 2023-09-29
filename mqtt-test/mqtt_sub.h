#ifndef MQTT_SUB_H
#define MQTT_SUB_H

#include <QObject>
#include <MQTTClient.h>
#include <QTimer>
#include <mutex>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
class mqtt_sub : public QObject
{
    Q_OBJECT
public:
    explicit mqtt_sub(QObject *parent = nullptr);
    ~mqtt_sub();
private:
    static volatile MQTTClient_deliveryToken deliveredtoken;
    static std::string sub_receive_message;
    static bool sub_receive_message_flag;
    std::string sub_topic;
    std::string sub_ip;
    std::string sub_host;
    std::string sub_clientid;
    int sub_qos;
    MQTTClient sub_client;
    MQTTClient_connectOptions conn_opts;
    long int sub_timeout;
    int sub_rc;
    QTimer *sub_timer;
    std::mutex my_lock;

public:
    QPushButton *mainwindow_button_mqtt_sub_connect;
    QPushButton *mainwindow_button_mqtt_sub_subscribe;
    QLineEdit *mainwindow_line_mqtt_sub_ip;
    QLineEdit *mainwindow_line_mqtt_sub_host;
    QLineEdit *mainwindow_line_mqtt_sub_clientid;
    QLineEdit *mainwindow_line_mqtt_sub_topic;
    QComboBox *mainwindow_combox_mqtt_sub_qos;
    QTextEdit *mainwindow_tedit_mqtt_sub_text;
public:
    bool mqtt_sub_thread_connect();
    bool mqtt_sub_thread_disconnect();
    bool mqtt_sub_thread_subscribe();
    void mqtt_sub_thread_closesubscribe();
private:
    static void delivered(void *context, MQTTClient_deliveryToken dt);
    static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
    static void connlost(void *context, char *cause);
    void mqtt_sub_receive_message();
    std::string mqtt_sub_thread_sub_time();
signals:

};

#endif // MQTT_SUB_H
