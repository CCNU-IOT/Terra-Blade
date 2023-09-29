#include "mqtt_pub.h"

mqtt_pub::mqtt_pub(QObject *parent)
    : QObject{parent}
{

}
mqtt_pub::~mqtt_pub()
{
}
bool mqtt_pub::mqtt_pub_thread_open()
{
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
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

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((pub_rc = MQTTClient_connect(pub_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to connect, return code " << pub_rc << std::endl;
        return false;
    }

    return true;
}
void mqtt_pub::mqtt_pub_thread_close()
{

}
void mqtt_pub::mqtt_pub_thread_run()
{

}
