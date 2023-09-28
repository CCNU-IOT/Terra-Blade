package com.example.grass;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.baidu.mapapi.CoordType;
import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.MapView;
import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

public class MainActivity extends AppCompatActivity implements MqttManager.MqttManagerCallback {

    private MqttManager mqttManager;
    private HandlerThread handlerThread;
    private Handler mHandler;
    private Handler uiHandler = new Handler();

    private Button buttonForward;
    private Button buttonBackward;
    private Button buttonLeft;
    private Button buttonRight;
    private Button buttonStartPause;
    private TextView latitude;
    private  TextView longitude;

    private boolean isRunning = false;//判断当前状态
    private MapView mMapView=null;
    private static final String LOCATION_TOPIC = "locationTopic";
    private final Gson gson = new Gson();
    private final JsonParser jsonParser = new JsonParser();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //mMapView = (MapView) findViewById(R.id.baiduMapView);





        handlerThread = new HandlerThread("mqttConnectThread");
        handlerThread.start();
        mHandler = new Handler(handlerThread.getLooper());

        mqttManager = new MqttManager(this);

        mHandler.post(() -> {
            boolean isConnected = mqttManager.connect();
            runOnUiThread(() -> {
                if (isConnected) {
                    Toast.makeText(MainActivity.this, "连接成功", Toast.LENGTH_SHORT).show();
                    mqttManager.subscribe(LOCATION_TOPIC); // 连接成功后进行经度纬度的订阅
                } else {
                    Toast.makeText(MainActivity.this, "连接失败", Toast.LENGTH_SHORT).show();
                }
            });
        });

        buttonForward = findViewById(R.id.button_forward);
        buttonBackward = findViewById(R.id.button_backward);
        buttonLeft = findViewById(R.id.button_left);
        buttonRight = findViewById(R.id.button_right);
        buttonStartPause = findViewById(R.id.button_start_pause);
        latitude=findViewById(R.id.latitudeTextView);
        longitude=findViewById(R.id.longitudeTextView);


        buttonForward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                ControlCarDir("forward", event);
                return true;
            }
        });

        buttonBackward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                ControlCarDir("backward", event);
                return true;
            }
        });

        buttonLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                ControlCarDir("left", event);
                return true;
            }
        });

        buttonRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                ControlCarDir("right", event);
                return true;
            }
        });



    }


    //实现button的长按控制小车
    public void ControlCarDir(String direction, MotionEvent event) {
        Runnable action;
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            action = () -> {
                try {
                    mqttManager.publish("direction", direction);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            };
        } else if (event.getAction() == MotionEvent.ACTION_UP) {
            action = () -> {
                try {
                    mqttManager.publish("direction", "stop");
                } catch (Exception e) {
                    e.printStackTrace();
                }
            };
        } else {
            return;
        }
        mHandler.post(action);
    }



    @Override
    protected void onDestroy() {
        super.onDestroy();
        handlerThread.quitSafely();
        //在activity执行onDestroy时执行mMapView.onDestroy()，实现地图生命周期管理
        mMapView.onDestroy();
    }


    //处理收到的经纬度
    /*
    假设经度纬度以json格式发送
    {
    "longitude": "120.12345",
    "latitude": "30.12345"
      }

    */
    @Override
    public void onMessageReceived(String topic, String message) {
        try {
            JsonObject jsonObject = JsonParser.parseString(message).getAsJsonObject();
            if (jsonObject.has("longitude") && jsonObject.has("latitude")) {
                String longitudestring = jsonObject.get("longitude").getAsString();
                String latitudestring = jsonObject.get("latitude").getAsString();

                runOnUiThread(() -> {
                    latitude.setText("纬度: " + latitudestring );
                    longitude.setText("经度："+longitudestring);
                });
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }



    //地图生命周期管理
    @Override
    protected void onResume() {
        super.onResume();
        //在activity执行onResume时执行mMapView. onResume ()，实现地图生命周期管理
        mMapView.onResume();
    }
    @Override
    protected void onPause() {
        super.onPause();
        //在activity执行onPause时执行mMapView. onPause ()，实现地图生命周期管理
        mMapView.onPause();
    }

}

