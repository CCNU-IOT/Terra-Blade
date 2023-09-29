#include "mqtt_pub.h"

mqtt_pub::mqtt_pub(QObject *parent)
    : QObject{parent}
{
    pub_timeout = 10000L;
    pubmsg = MQTTClient_message_initializer;
    conn_opts = MQTTClient_connectOptions_initializer;
}
mqtt_pub::~mqtt_pub()
{
}
bool mqtt_pub::mqtt_pub_thread_connect()
{
    std::string pub_address = "tcp://"
                              + mainwindow_line_mqtt_pub_ip->text().toStdString()
                              + ":"
                              + mainwindow_line_mqtt_pub_host->text().toStdString();
    //"tcp://mqtt.eclipseprojects.io:1883"
    pub_clientid = mainwindow_line_mqtt_pub_clientid->text().toStdString();
    if ((pub_rc = MQTTClient_create(&pub_client, pub_address.c_str(), pub_clientid.c_str(),
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to create client, return code " << pub_rc << std::endl;
        return false;//faliure
    }

    conn_opts.keepAliveInterval = 2000;
    conn_opts.cleansession = 1;
    if ((pub_rc = MQTTClient_connect(pub_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to connect, return code " << pub_rc << std::endl;
        return false;
    }
    return true;
}
bool mqtt_pub::mqtt_pub_thread_disconnect()
{
    if ((pub_rc = MQTTClient_disconnect(pub_client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to disconnect, return code " << pub_rc << std::endl;
        return false;
    }
    MQTTClient_destroy(&pub_client);
    return true;
}
bool mqtt_pub::mqtt_pub_thread_publish()
{
    pub_mode = mainwindow_combox_mqtt_pub_mode->currentText().toStdString();
    pub_qos = mainwindow_combox_mqtt_pub_qos->currentIndex();
    pub_topic = mainwindow_line_mqtt_pub_topic->text().toStdString();
    pub_message = mainwindow_line_mqtt_pub_message->text().toStdString();
    if (pub_message.empty())
    {
        return false;//pub message empty
    }
    else
    {
        if (pub_mode == "single send")
        {
            mqtt_pub_thread_signal_send();
        }
        else if (pub_mode == "send once 0.1s")
        {
            mainwindow_button_mqtt_pub_publish->setText(QString::fromStdString("Close publish"));
            mainwindow_line_mqtt_pub_topic->setEnabled(false);
            mainwindow_line_mqtt_pub_message->setEnabled(false);
            mainwindow_combox_mqtt_pub_mode->setEnabled(false);
            mainwindow_combox_mqtt_pub_qos->setEnabled(false);
            pub_timer = new QTimer(this);
            connect(pub_timer, &QTimer::timeout, this, QOverload<>::of(&mqtt_pub::mqtt_pub_thread_signal_send));
            pub_timer->start(100);//0.1s
        }
        else if (pub_mode == "send once 1s")
        {
            mainwindow_button_mqtt_pub_publish->setText(QString::fromStdString("Close publish"));
            mainwindow_line_mqtt_pub_topic->setEnabled(false);
            mainwindow_line_mqtt_pub_message->setEnabled(false);
            mainwindow_combox_mqtt_pub_mode->setEnabled(false);
            mainwindow_combox_mqtt_pub_qos->setEnabled(false);
            pub_timer = new QTimer(this);
            connect(pub_timer, &QTimer::timeout, this, QOverload<>::of(&mqtt_pub::mqtt_pub_thread_signal_send));
            pub_timer->start(1000);//1s
        }
        else if (pub_mode == "send once 10s")
        {
            mainwindow_button_mqtt_pub_publish->setText(QString::fromStdString("Close publish"));
            mainwindow_line_mqtt_pub_topic->setEnabled(false);
            mainwindow_line_mqtt_pub_message->setEnabled(false);
            mainwindow_combox_mqtt_pub_mode->setEnabled(false);
            mainwindow_combox_mqtt_pub_qos->setEnabled(false);
            pub_timer = new QTimer(this);
            connect(pub_timer, &QTimer::timeout, this, QOverload<>::of(&mqtt_pub::mqtt_pub_thread_signal_send));
            pub_timer->start(10000);//10s
        }
        return true;//pub message no empty
    }
}
void mqtt_pub::mqtt_pub_thread_closepublish()
{
    if (pub_timer->isActive())
    {
        pub_timer->stop();
    }
    mainwindow_line_mqtt_pub_topic->setEnabled(true);
    mainwindow_line_mqtt_pub_message->setEnabled(true);
    mainwindow_combox_mqtt_pub_mode->setEnabled(true);
    mainwindow_combox_mqtt_pub_qos->setEnabled(true);
    mainwindow_button_mqtt_pub_publish->setText("Publish");
    delete pub_timer;
}
void mqtt_pub::mqtt_pub_thread_signal_send()
{
    std::lock_guard<std::mutex> my_lock_guard(my_lock);
    pubmsg.payload = (void *)pub_message.c_str();
    pubmsg.payloadlen = pub_message.length();
    pubmsg.qos = pub_qos;
    pubmsg.retained = 0;
    if ((pub_rc = MQTTClient_publishMessage(pub_client, pub_topic.c_str(), &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to publish message, return code " << pub_rc << std::endl;
    }
    mainwindow_tedit_mqtt_pub_text->insertPlainText(QString::fromStdString(pub_message + "\n"));//print
    pub_rc = MQTTClient_waitForCompletion(pub_client, token, pub_timeout);
    std::cout << "Message with delivery token " << token << " delivered." << std::endl;
}
