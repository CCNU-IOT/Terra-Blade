package com.example.grass;

import static androidx.constraintlayout.helper.widget.MotionEffect.TAG;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.IMqttToken;


import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class MqttManager {

    private static final String BROKER = "tcp://your_broker_address:port";
    private static final String CLIENT_ID = "your_client_id";
    private static final int QOS = 2;

    private final MqttClient mqttClient;
    private final MqttConnectOptions connOpts;

    // 使用一个接口作为回调，以便在Activity中接收消息。
    public interface MqttManagerCallback {
        void onMessageReceived(String topic, String message);
    }

    private MqttManagerCallback callback;

    public MqttManager(MqttManagerCallback callback) {
        this.callback = callback;
        MemoryPersistence persistence = new MemoryPersistence();

        try {
            mqttClient = new MqttClient(BROKER, CLIENT_ID, persistence);
            connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);

            // 设置回调
            mqttClient.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable throwable) {
                    System.out.println("与MQTT代理服务器的连接丢失，尝试重新连接...");
                    Log.d("MQTT", "与MQTT代理服务器的连接丢失，尝试重新连接...");
                    // 重新连接
                    connect();
                }

                @Override
                public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                    if (callback != null) {
                        callback.onMessageReceived(topic, new String(mqttMessage.getPayload()));
                        //调试用
                        System.out.println("收到消息:");
                        System.out.println("主题: " + topic);
                        System.out.println("内容: " + new String(mqttMessage.getPayload()));
                    }
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
                    System.out.println("消息已发送");
                }
            });

        } catch (MqttException e) {
            throw new RuntimeException("Could not initialize MQTT client", e);
        }
    }


    public boolean connect() {
        try {
            mqttClient.connect(connOpts);
            System.out.println("连接到MQTT代理服务器");
            return true;
        } catch (MqttException e) {
            e.printStackTrace();
            // 简单的重试机制
            for (int i = 0; i < 3; i++) {
                try {
                    Thread.sleep(2000); // 等待2秒
                    mqttClient.connect(connOpts);
                    System.out.println("连接到MQTT代理服务器");
                    return true;
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }
            return false;
        }
    }



    public void subscribe(String topic) {
        try {
            mqttClient.subscribe(topic, QOS);
            System.out.println("订阅了主题：" + topic);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void publish(String topic, String message) {
        try {
            MqttMessage mqttMessage = new MqttMessage(message.getBytes());
            mqttMessage.setQos(QOS);
            mqttClient.publish(topic, mqttMessage);
            System.out.println("发布了消息：" + message);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }


    public void disconnect() {
        if (mqttClient != null && mqttClient.isConnected()) {
            try {
                mqttClient.disconnect();
            } catch (MqttException e) {
                e.printStackTrace();
            }
        }
    }

}
