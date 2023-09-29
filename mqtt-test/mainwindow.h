#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MQTTClient.h>
#include <iostream>
#include "mqtt_pub.h"
#include "mqtt_sub.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    mqtt_pub *pub;
    mqtt_sub *sub;
private:
    void mqtt_mainwindow_init();
    void mqtt_mainwindow_share_gui_sub();
    void mqtt_mainwindow_share_gui_pub();
signals:
    void mqtt_mainwindow_open_sub_textchanged();
    void mqtt_mainwindow_open_pub_textchanged();
    void mqtt_mainwindow_sub_connect_textchanged();
    void mqtt_mainwindow_pub_connect_textchanged();
    void mqtt_mainwindow_sub_subscribe_textchanged();
    void mqtt_mainwindow_pub_publish_textchanged();
public slots:
    void mqtt_mainwindow_open_sub();
    void mqtt_mainwindow_open_pub();
    void mqtt_mainwindow_change_subtext();
    void mqtt_mainwindow_change_pubtext();
    void mqtt_mainwindow_sub_connect();
    void mqtt_mainwindow_pub_connect();
    void mqtt_mainwindow_change_sub_connect_text();
    void mqtt_mainwindow_change_pub_connect_text();
    void mqtt_mainwindow_sub_subscribe();
    void mqtt_mainwindow_pub_publish();
    void mqtt_mainwindow_change_sub_subscribe_text();
    void mqtt_mainwindow_change_pub_publish_text();
    void mqtt_mainwindow_pub_textedit_change_text();//textEdit slot
};
#endif // MAINWINDOW_H
