#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::mqtt_mainwindow_init();
}

MainWindow::~MainWindow()
{
    delete pub;
    delete sub;
    delete ui;
}
void MainWindow::mqtt_mainwindow_open_sub()
{
    emit mqtt_mainwindow_open_sub_textchanged();
}
void MainWindow::mqtt_mainwindow_open_pub()
{
    emit mqtt_mainwindow_open_pub_textchanged();
}
void MainWindow::mqtt_mainwindow_change_subtext()
{
    std::string button_text = ui->pushButton_mqtt_sub_opensub->text().toStdString();
    if (button_text == "Open SUB")
    {
        std::cout << "SystemDebug::open_sub" << std::endl;
        ui->textEdit_mqtt_sub_text->setEnabled(true);
        ui->lineEdit_mqtt_sub_clientid->setEnabled(true);
        ui->lineEdit_mqtt_sub_ip->setEnabled(true);
        ui->lineEdit_mqtt_sub_host->setEnabled(true);
        ui->pushButton_mqtt_sub_connect->setEnabled(true);
        button_text = "Close SUB";
        ui->pushButton_mqtt_sub_opensub->setText(QString::fromStdString(button_text));
    }
    else if (button_text == "Close SUB")
    {
        std::cout << "SystemDebug::close_pub" << std::endl;
        ui->textEdit_mqtt_sub_text->setEnabled(false);
        ui->lineEdit_mqtt_sub_clientid->setEnabled(false);
        ui->lineEdit_mqtt_sub_ip->setEnabled(false);
        ui->lineEdit_mqtt_sub_topic->setEnabled(false);
        ui->lineEdit_mqtt_sub_host->setEnabled(false);
        ui->pushButton_mqtt_sub_connect->setEnabled(false);
        ui->pushButton_mqtt_sub_subscribe->setEnabled(false);
        ui->comboBox_mqtt_sub_qos->setEnabled(false);
        button_text = "Open SUB";
        ui->pushButton_mqtt_sub_opensub->setText(QString::fromStdString(button_text));
    }
}
void MainWindow::mqtt_mainwindow_change_pubtext()
{
    std::string button_text = ui->pushButton_mqtt_pub_openpub->text().toStdString();
    if (button_text == "Open PUB")
    {
        std::cout << "SystemDebug:open_pub" << std::endl;
        ui->textEdit_mqtt_pub_text->setEnabled(true);
        ui->lineEdit_mqtt_pub_clientid->setEnabled(true);
        ui->lineEdit_mqtt_pub_ip->setEnabled(true);
        ui->lineEdit_mqtt_pub_host->setEnabled(true);
        ui->pushButton_mqtt_pub_connect->setEnabled(true);
        button_text = "Close PUB";
        ui->pushButton_mqtt_pub_openpub->setText(QString::fromStdString(button_text));
    }
    else if (button_text == "Close PUB")
    {
        std::cout << "SystemDebug:close_pub" << std::endl;
        ui->textEdit_mqtt_pub_text->setEnabled(false);
        ui->lineEdit_mqtt_pub_clientid->setEnabled(false);
        ui->lineEdit_mqtt_pub_ip->setEnabled(false);
        ui->lineEdit_mqtt_pub_topic->setEnabled(false);
        ui->lineEdit_mqtt_pub_host->setEnabled(false);
        ui->pushButton_mqtt_pub_connect->setEnabled(false);
        ui->pushButton_mqtt_pub_publish->setEnabled(false);
        ui->comboBox_mqtt_pub_qos->setEnabled(false);
        button_text = "Open PUB";
        ui->pushButton_mqtt_pub_openpub->setText(QString::fromStdString(button_text));
    }
}
void MainWindow::mqtt_mainwindow_sub_connect()
{
    emit mqtt_mainwindow_sub_connect_textchanged();
}
void MainWindow::mqtt_mainwindow_pub_connect()
{
    emit mqtt_mainwindow_pub_connect_textchanged();
}
void MainWindow::mqtt_mainwindow_change_pub_connect_text()
{
    std::string button_text = ui->pushButton_mqtt_pub_connect->text().toStdString();
    if (button_text == "Connect")
    {
        if (pub->mqtt_pub_thread_connect())
        {
            std::cout << "SystemDebug:Connect" << std::endl;
            ui->lineEdit_mqtt_pub_clientid->setEnabled(false);
            ui->lineEdit_mqtt_pub_host->setEnabled(false);
            ui->lineEdit_mqtt_pub_ip->setEnabled(false);
            ui->pushButton_mqtt_pub_openpub->setEnabled(false);
            ui->lineEdit_mqtt_pub_message->setEnabled(true);
            ui->lineEdit_mqtt_pub_topic->setEnabled(true);
            ui->comboBox_mqtt_pub_mode->setEnabled(true);
            ui->comboBox_mqtt_pub_qos->setEnabled(true);
            ui->pushButton_mqtt_pub_publish->setEnabled(true);
            button_text = "Disconnect";
            ui->pushButton_mqtt_pub_connect->setText(QString::fromStdString(button_text));
        }
    }
    else if (button_text == "Disconnect")
    {
        if (pub->mqtt_pub_thread_disconnect())
        {
            std::cout << "SystemDebug:Disconnect" << std::endl;
            ui->lineEdit_mqtt_pub_clientid->setEnabled(true);
            ui->lineEdit_mqtt_pub_host->setEnabled(true);
            ui->lineEdit_mqtt_pub_ip->setEnabled(true);
            ui->pushButton_mqtt_pub_openpub->setEnabled(true);
            ui->lineEdit_mqtt_pub_message->setEnabled(false);
            ui->lineEdit_mqtt_pub_topic->setEnabled(false);
            ui->comboBox_mqtt_pub_mode->setEnabled(false);
            ui->comboBox_mqtt_pub_qos->setEnabled(false);
            ui->pushButton_mqtt_pub_publish->setEnabled(false);
            button_text = "Connect";
            ui->pushButton_mqtt_pub_connect->setText(QString::fromStdString(button_text));
        }
    }
}
void MainWindow::mqtt_mainwindow_change_sub_connect_text()
{
    std::string button_text = ui->pushButton_mqtt_sub_connect->text().toStdString();
    if (button_text == "Connect")
    {
        if (sub->mqtt_sub_thread_connect())
        {
            std::cout << "SystemDebug:Connect" << std::endl;
            ui->lineEdit_mqtt_sub_clientid->setEnabled(false);
            ui->lineEdit_mqtt_sub_host->setEnabled(false);
            ui->lineEdit_mqtt_sub_ip->setEnabled(false);
            ui->pushButton_mqtt_sub_opensub->setEnabled(false);
            ui->lineEdit_mqtt_sub_topic->setEnabled(true);
            ui->comboBox_mqtt_sub_qos->setEnabled(true);
            ui->pushButton_mqtt_sub_subscribe->setEnabled(true);
            button_text = "Disconnect";
            ui->pushButton_mqtt_sub_connect->setText(QString::fromStdString(button_text));
        }
    }
    else if (button_text == "Disconnect")
    {
        if (sub->mqtt_sub_thread_disconnect())
        {
            std::cout << "SystemDebug:Disconnect" << std::endl;
            ui->lineEdit_mqtt_sub_clientid->setEnabled(true);
            ui->lineEdit_mqtt_sub_host->setEnabled(true);
            ui->lineEdit_mqtt_sub_ip->setEnabled(true);
            ui->pushButton_mqtt_sub_opensub->setEnabled(true);
            ui->lineEdit_mqtt_sub_topic->setEnabled(false);
            ui->comboBox_mqtt_sub_qos->setEnabled(false);
            ui->pushButton_mqtt_sub_subscribe->setEnabled(false);
            button_text = "Connect";
            ui->pushButton_mqtt_sub_connect->setText(QString::fromStdString(button_text));
        }
    }
}
void MainWindow::mqtt_mainwindow_sub_subscribe()
{
    emit mqtt_mainwindow_sub_subscribe_textchanged();
}
void MainWindow::mqtt_mainwindow_pub_publish()
{
    emit mqtt_mainwindow_pub_publish_textchanged();
}
void MainWindow::mqtt_mainwindow_change_sub_subscribe_text()
{
    std::string button_text = ui->pushButton_mqtt_sub_subscribe->text().toStdString();
    if (button_text == "Subscribe")
    {
        sub->mqtt_sub_thread_subscribe();
        button_text = "Close Subscribe";
        ui->pushButton_mqtt_sub_subscribe->setText(QString::fromStdString(button_text));
        std::cout << "Subscribe" << std::endl;
    }
    else if (button_text == "Close Subscribe")
    {
        sub->mqtt_sub_thread_closesubscribe();
        button_text = "Subscribe";
        ui->pushButton_mqtt_sub_subscribe->setText(QString::fromStdString(button_text));
        std::cout << "Close Subscribe" << std::endl;
    }
}
void MainWindow::mqtt_mainwindow_change_pub_publish_text()
{
    std::string button_text = ui->pushButton_mqtt_pub_publish->text().toStdString();
    if (button_text == "Publish")
    {
        if (pub->mqtt_pub_thread_publish())
        {
            std::cout << "publish successful" << std::endl;
        }
        else
        {
            std::cout << "publish message no-allow empty" << std::endl;
        }
    }
    else if (button_text == "Close publish")
    {
        pub->mqtt_pub_thread_closepublish();
        std::cout << "Close publish" << std::endl;
    }
}
void MainWindow::mqtt_mainwindow_pub_textedit_change_text()
{
    ui->textEdit_mqtt_pub_text->moveCursor(QTextCursor::End);
}
void MainWindow::mqtt_mainwindow_sub_textedit_change_text()
{
    ui->textEdit_mqtt_sub_text->moveCursor(QTextCursor::End);
}
void MainWindow::mqtt_mainwindow_sub_claer_text()
{
    ui->textEdit_mqtt_sub_text->clear();
}
void MainWindow::mqtt_mainwindow_pub_clear_text()
{
    ui->textEdit_mqtt_pub_text->clear();
}
void MainWindow::mqtt_mainwindow_init()
{
    pub = new mqtt_pub(this);
    sub = new mqtt_sub(this);
    mqtt_mainwindow_share_gui_pub();
    mqtt_mainwindow_share_gui_sub();
    connect(ui->pushButton_mqtt_sub_opensub, SIGNAL(clicked()), this, SLOT(mqtt_mainwindow_open_sub()));
    connect(ui->pushButton_mqtt_pub_openpub, SIGNAL(clicked()), this, SLOT(mqtt_mainwindow_open_pub()));
    connect(this, SIGNAL(mqtt_mainwindow_open_sub_textchanged()), this, SLOT(mqtt_mainwindow_change_subtext()));
    connect(this, SIGNAL(mqtt_mainwindow_open_pub_textchanged()), this, SLOT(mqtt_mainwindow_change_pubtext()));
    connect(ui->pushButton_mqtt_sub_connect, SIGNAL(clicked()), this, SLOT(mqtt_mainwindow_sub_connect()));
    connect(ui->pushButton_mqtt_pub_connect, SIGNAL(clicked()), this, SLOT(mqtt_mainwindow_pub_connect()));
    connect(this, SIGNAL(mqtt_mainwindow_sub_connect_textchanged()), this, SLOT(mqtt_mainwindow_change_sub_connect_text()));
    connect(this, SIGNAL(mqtt_mainwindow_pub_connect_textchanged()), this, SLOT(mqtt_mainwindow_change_pub_connect_text()));
    connect(ui->pushButton_mqtt_sub_subscribe, SIGNAL(clicked()), this, SLOT(mqtt_mainwindow_sub_subscribe()));
    connect(ui->pushButton_mqtt_pub_publish, SIGNAL(clicked()), this, SLOT(mqtt_mainwindow_pub_publish()));
    connect(this, SIGNAL(mqtt_mainwindow_sub_subscribe_textchanged()), this, SLOT(mqtt_mainwindow_change_sub_subscribe_text()));
    connect(this, SIGNAL(mqtt_mainwindow_pub_publish_textchanged()), this, SLOT(mqtt_mainwindow_change_pub_publish_text()));
    connect(ui->textEdit_mqtt_pub_text, SIGNAL(textChanged()), this, SLOT(mqtt_mainwindow_pub_textedit_change_text()));
    connect(ui->textEdit_mqtt_sub_text, SIGNAL(textChanged()), this, SLOT(mqtt_mainwindow_sub_textedit_change_text()));
    connect(ui->pushButton_mqtt_pub_clear, SIGNAL(clicked()), this, SLOT(mqtt_mainwindow_pub_clear_text()));
    connect(ui->pushButton_mqtt_sub_clear, SIGNAL(clicked()), this, SLOT(mqtt_mainwindow_sub_claer_text()));

    ui->textEdit_mqtt_sub_text->setEnabled(false);
    ui->lineEdit_mqtt_sub_clientid->setEnabled(false);
    ui->lineEdit_mqtt_sub_ip->setEnabled(false);
    ui->lineEdit_mqtt_sub_topic->setEnabled(false);
    ui->lineEdit_mqtt_sub_host->setEnabled(false);
    ui->pushButton_mqtt_sub_connect->setEnabled(false);
    ui->pushButton_mqtt_sub_subscribe->setEnabled(false);
    ui->comboBox_mqtt_sub_qos->setEnabled(false);

    ui->textEdit_mqtt_pub_text->setEnabled(false);
    ui->lineEdit_mqtt_pub_clientid->setEnabled(false);
    ui->lineEdit_mqtt_pub_ip->setEnabled(false);
    ui->lineEdit_mqtt_pub_topic->setEnabled(false);
    ui->lineEdit_mqtt_pub_host->setEnabled(false);
    ui->lineEdit_mqtt_pub_message->setEnabled(false);
    ui->pushButton_mqtt_pub_connect->setEnabled(false);
    ui->pushButton_mqtt_pub_publish->setEnabled(false);
    ui->comboBox_mqtt_pub_qos->setEnabled(false);
    ui->comboBox_mqtt_pub_mode->setEnabled(false);

}
void MainWindow::mqtt_mainwindow_share_gui_sub()
{
    sub->mainwindow_button_mqtt_sub_connect = ui->pushButton_mqtt_sub_connect;
    sub->mainwindow_button_mqtt_sub_subscribe = ui->pushButton_mqtt_sub_subscribe;
    sub->mainwindow_line_mqtt_sub_clientid = ui->lineEdit_mqtt_sub_clientid;
    sub->mainwindow_line_mqtt_sub_host = ui->lineEdit_mqtt_sub_host;
    sub->mainwindow_line_mqtt_sub_ip = ui->lineEdit_mqtt_sub_ip;
    sub->mainwindow_line_mqtt_sub_topic = ui->lineEdit_mqtt_sub_topic;
    sub->mainwindow_combox_mqtt_sub_qos = ui->comboBox_mqtt_sub_qos;
    sub->mainwindow_tedit_mqtt_sub_text = ui->textEdit_mqtt_sub_text;
}
void MainWindow::mqtt_mainwindow_share_gui_pub()
{
    pub->mainwindow_button_mqtt_pub_connect = ui->pushButton_mqtt_pub_connect;
    pub->mainwindow_button_mqtt_pub_publish = ui->pushButton_mqtt_pub_publish;
    pub->mainwindow_combox_mqtt_pub_mode = ui->comboBox_mqtt_pub_mode;
    pub->mainwindow_combox_mqtt_pub_qos = ui->comboBox_mqtt_pub_qos;
    pub->mainwindow_line_mqtt_pub_clientid = ui->lineEdit_mqtt_pub_clientid;
    pub->mainwindow_line_mqtt_pub_host = ui->lineEdit_mqtt_pub_host;
    pub->mainwindow_line_mqtt_pub_ip = ui->lineEdit_mqtt_pub_ip;
    pub->mainwindow_line_mqtt_pub_message = ui->lineEdit_mqtt_pub_message;
    pub->mainwindow_line_mqtt_pub_topic = ui->lineEdit_mqtt_pub_topic;
    pub->mainwindow_tedit_mqtt_pub_text = ui->textEdit_mqtt_pub_text;
}
