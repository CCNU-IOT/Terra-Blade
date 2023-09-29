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
    std::string pub_qos;
    std::string pub_mode;
    std::string pub_message;
    MQTTClient pub_client;
    int pub_rc;
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
    bool mqtt_pub_thread_open();
    void mqtt_pub_thread_close();
    void mqtt_pub_thread_run();
signals:
private slots:

};

#endif // MQTT_PUB_H
