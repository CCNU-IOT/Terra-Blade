#include "mqtt_sub.h"
volatile MQTTClient_deliveryToken mqtt_sub::deliveredtoken = 0;
std::string mqtt_sub::sub_receive_message = "";
bool mqtt_sub::sub_receive_message_flag = false;
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
    sub_receive_message_flag = true;
    sub_receive_message = (char*)message->payload;
    std::cout << "Message arrived" << std::endl;
    std::cout << "     topic: " << topicName << std::endl;
    std::cout << "   message: " << sub_receive_message << std::endl;
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
    sub_topic = mainwindow_line_mqtt_sub_topic->text().toStdString();
    sub_qos = mainwindow_combox_mqtt_sub_qos->currentIndex();
    if ((sub_rc = MQTTClient_subscribe(sub_client, sub_topic.c_str(), sub_qos)) != MQTTCLIENT_SUCCESS)
    {
        std::cout << "Failed to subscribe, return code " << sub_rc << std::endl;
        return false;//failure
    }
    mainwindow_button_mqtt_sub_connect->setEnabled(false);
    mainwindow_line_mqtt_sub_topic->setEnabled(false);
    mainwindow_combox_mqtt_sub_qos->setEnabled(false);
    sub_timer = new QTimer(this);
    connect(sub_timer, &QTimer::timeout, this, QOverload<>::of(&mqtt_sub::mqtt_sub_receive_message));
    sub_timer->start(1);
    return true;
}
void mqtt_sub::mqtt_sub_thread_closesubscribe()
{
    if (sub_timer->isActive())
    {
        sub_timer->stop();
    }
    mainwindow_button_mqtt_sub_connect->setEnabled(true);
    mainwindow_line_mqtt_sub_topic->setEnabled(true);
    mainwindow_combox_mqtt_sub_qos->setEnabled(true);
    delete sub_timer;
}
void mqtt_sub::mqtt_sub_receive_message()
{
    std::lock_guard<std::mutex> my_lock_guard(my_lock);
    if (sub_receive_message_flag)
    {
        sub_receive_message_flag = false;
        std::string sub_time = mqtt_sub_thread_sub_time();
        mainwindow_tedit_mqtt_sub_text->insertPlainText(QString::fromStdString(sub_time + sub_receive_message + "\n"));
    }
}
std::string mqtt_sub::mqtt_sub_thread_sub_time()
{
    //get current time
    std::chrono::time_point currentTime = std::chrono::system_clock::now();
    //calculate ms
    auto timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);
    auto ms = timestamp.time_since_epoch();
    int milliseconds = ms.count() % 1000;
    //Converting current time to tm structures
    std::time_t time = std::chrono::system_clock::to_time_t(currentTime);
    struct std::tm *tmStruct = std::localtime(&time);

    //Extract time information
    int year = tmStruct->tm_year + 1900; // year + 1900
    int month = tmStruct->tm_mon + 1;    //month start from 0
    int day = tmStruct->tm_mday;
    int hour = tmStruct->tm_hour;
    int minute = tmStruct->tm_min;
    int second = tmStruct->tm_sec;

    std::stringstream time_out;
    time_out << std::setfill('0')
             << std::setw(4) << year << "-"
             << std::setw(2) << month << "-"
             << std::setw(2) << day << "-"
             << std::setw(2) << hour << ":"
             << std::setw(2) << minute << ":"
             << std::setw(2) << second << ":"
             << std::setw(3) << milliseconds;

    return time_out.str();
}
