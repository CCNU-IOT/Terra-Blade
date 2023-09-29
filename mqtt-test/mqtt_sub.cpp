#include "mqtt_sub.h"
volatile MQTTClient_deliveryToken mqtt_sub::deliveredtoken = 0;
mqtt_sub::mqtt_sub(QObject *parent)
    : QObject{parent}
{
    sub_timeout = 10000L;
    conn_opts = MQTTClient_connectOptions_initializer;
}
mqtt_sub::~mqtt_sub()
{

}
void mqtt_sub::delivered(void *context, MQTTClient_deliveryToken dt)
{
    std::cout << "Message with token value " << dt << " delivery confirmed" << std::endl;
    deliveredtoken = dt;
}

int mqtt_sub::msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    std::cout << "Message arrived" << std::endl;
    std::cout << "     topic: " << topicName << std::endl;
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void mqtt_sub::connlost(void *context, char *cause)
{
    std::cout << "\nConnection lost" << std::endl;
    std::cout << "     cause: " << cause << std::endl;
}
bool mqtt_sub::mqtt_sub_thread_connect()
{
    std::string sub_address = "tcp://"
                              + mainwindow_line_mqtt_sub_ip->text().toStdString()
                              + ":"
                              + mainwindow_line_mqtt_sub_host->text().toStdString();
    //"tcp://mqtt.eclipseprojects.io:1883"
    sub_clientid = mainwindow_line_mqtt_sub_clientid->text().toStdString();
    if ((sub_rc = MQTTClient_create(&sub_client, sub_address.c_str(), sub_clientid.c_str(),
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to create client, return code " << sub_rc << std::endl;
        return false;//failure
    }
    if ((sub_rc = MQTTClient_setCallbacks(sub_client, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to set callbacks, return code " << sub_rc << std::endl;
        return false;//failure
    }
    conn_opts.keepAliveInterval = 2000;
    conn_opts.cleansession = 1;
    if ((sub_rc = MQTTClient_connect(sub_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to connect, return code " << sub_rc << std::endl;
        return false;
    }
    return true;
}
bool mqtt_sub::mqtt_sub_thread_disconnect()
{
    if ((sub_rc = MQTTClient_disconnect(sub_client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to disconnect, return code " << sub_rc << std::endl;
        return false;
    }
    MQTTClient_destroy(&sub_client);
    return true;
}
bool mqtt_sub::mqtt_sub_thread_subscribe()
{

}
void mqtt_sub::mqtt_sub_thread_closesubscribe()
{

}

