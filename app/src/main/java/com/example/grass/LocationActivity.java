package com.example.grass;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import android.app.Activity;
import android.location.LocationListener;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.baidu.location.BDAbstractLocationListener;
import com.baidu.location.BDLocation;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.mapapi.CoordType;
import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.BaiduMapOptions;
import com.baidu.mapapi.map.BitmapDescriptor;
import com.baidu.mapapi.map.BitmapDescriptorFactory;
import com.baidu.mapapi.map.MapStatus;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.Marker;
import com.baidu.mapapi.map.MarkerOptions;
import com.baidu.mapapi.map.MyLocationConfiguration;
import com.baidu.mapapi.map.MyLocationData;
import com.baidu.mapapi.model.LatLng;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import java.util.List;

public class LocationActivity extends AppCompatActivity implements MqttManager.MqttManagerCallback {
    MqttManager mqttManager;
    TextView longitudeTextView;
    TextView latitudeTextView;

    private HandlerThread handlerThread;
    private Handler mHandler;

    private MapView mMapView = null;
    BaiduMap mBaiduMap=null;
    LocationClient mLocationClient=null;
    private Marker mCarMarker;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);




        SDKInitializer.setAgreePrivacy(getApplicationContext(),true);
        //在使用SDK各组件之前初始化context信息，传入ApplicationContext
        SDKInitializer.initialize(getApplicationContext());
        //自4.3.0起，百度地图SDK所有接口均支持百度坐标和国测局坐标，用此方法设置您使用的坐标类型.
         //包括BD09LL和GCJ02两种坐标，默认是BD09LL坐标。
        SDKInitializer.setCoordType(CoordType.BD09LL);
        LocationClient.setAgreePrivacy(true);
        //setAgreePrivacy接口需要在LocationClient实例化之前调用
       //如果setAgreePrivacy接口参数设置为了false，则定位功能不会实现
         //true，表示用户同意隐私合规政策
        //false，表示用户不同意隐私合规政策
         //加了这几行后改好了


        setContentView(R.layout.activity_location);



        mMapView = (MapView) findViewById(R.id.baiduMapView);
        mBaiduMap = mMapView.getMap();
        // 初始化车辆标记
        LatLng defaultLocation = new LatLng(0, 0);  //默认坐标值
        BitmapDescriptor descriptor = BitmapDescriptorFactory.fromResource(R.drawable.img);  //车辆图标
        mCarMarker = (Marker) mBaiduMap.addOverlay(new MarkerOptions().position(defaultLocation).icon(descriptor));



        // 创建 HandlerThread 并获取其 Looper
        handlerThread = new HandlerThread("mqttConnectThread");
        handlerThread.start();
        mHandler = new Handler(handlerThread.getLooper());

        longitudeTextView = findViewById(R.id.longitudeTextView);
        latitudeTextView = findViewById(R.id.latitudeTextView);

        mqttManager = new MqttManager(this);

        // 使用 Handler 在新线程中建立 MQTT 连接和订阅
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                mqttManager.connect();
                mqttManager.subscribe("car/location");
            }
        });


        //获取地图控件引用
        mMapView = (MapView) findViewById(R.id.baiduMapView);
        //获取到地图
        mBaiduMap = mMapView.getMap();
        //设置地图放大的倍数
        init();
        //设置地图定位的一些参数，如定位图标，精度圈颜色等
        configure();
        //定位初始化
        try {
            init_location();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }


    }


    private void updateCarLocationOnMap(double latitude, double longitude) {
        LatLng carLocation = new LatLng(latitude, longitude);
        mCarMarker.setPosition(carLocation);
        mBaiduMap.animateMapStatus(MapStatusUpdateFactory.newLatLng(carLocation));
    }



    @Override
    protected void onResume() {
        mMapView.onResume();
        super.onResume();
    }

    @Override
    protected void onPause() {
        mMapView.onPause();
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        mLocationClient.stop();
        mBaiduMap.setMyLocationEnabled(false);
        mMapView.onDestroy();
        mMapView = null;
        super.onDestroy();
        mqttManager.disconnect();
    }
    /**
     * 继承抽象类BDAbstractListener并重写其onReceieveLocation方法来获取定位数据，并将其传给MapView。
     */
    public class MyLocationListener extends BDAbstractLocationListener {
        @Override
        public void onReceiveLocation(BDLocation location) {
            //mapView 销毁后不在处理新接收的位置
            if (location == null || mMapView == null){
                return;
            }
            MyLocationData locData = new MyLocationData.Builder()
                    .accuracy(location.getRadius())
                    // 此处设置开发者获取到的方向信息，顺时针0-360
                    .direction(location.getDirection()).latitude(location.getLatitude())
                    .longitude(location.getLongitude()).build();
            mBaiduMap.setMyLocationData(locData);
        }
    }

    /**
     *    设置地图放大的倍数
     */
    public  void init(){
        //设置地图放大的倍数
        MapStatus.Builder  builder=new MapStatus.Builder();
        builder.zoom(18f);
        mBaiduMap.setMapStatus(MapStatusUpdateFactory.newMapStatus(builder.build()));
    }

    /**
     * 自定义内容:
     * 参数说明
     * (1)定位模式 地图SDK支持三种定位模式：NORMAL（普通态）, FOLLOWING（跟随态）, COMPASS（罗盘态）
     * （2）是否开启方向
     * （3）自定义定位图标 支持自定义定位图标样式，
     * （4）自定义精度圈填充颜色
     * （5）自定义精度圈边框颜色
     */
    public void configure(){
        mBaiduMap.setMyLocationConfiguration(new MyLocationConfiguration(MyLocationConfiguration.LocationMode.FOLLOWING,true,
                BitmapDescriptorFactory.fromResource(R.drawable.img),
                0xAAFFFF88,0xAA00FF00));
    }

    /**
     * 定位的初始化
     */

    public void init_location() {
        try {
            mBaiduMap.setMyLocationEnabled(true);
            mLocationClient = new LocationClient(LocationActivity.this);

            // 通过LocationClientOption设置LocationClient相关参数
            LocationClientOption option = new LocationClientOption();
            option.setOpenGps(true); // 打开gps
            option.setOpenGps(true);

            option.setCoorType("bd09ll"); // 设置坐标类型
            option.setScanSpan(1000);

            // 设置locationClientOption
            mLocationClient.setLocOption(option);

            // 注册LocationListener监听器
            MyLocationListener myLocationListener = new MyLocationListener();
            mLocationClient.registerLocationListener(myLocationListener);

            // 开启地图定位图层
            mLocationClient.start();
        } catch (Exception e) {
            e.printStackTrace();
            // 可以使用Toast或其他方法显示错误信息给用户
            Toast.makeText(LocationActivity.this, "定位初始化失败，请检查设置", Toast.LENGTH_SHORT).show();
        }
    }




    @Override
    public void onMessageReceived(String topic, String message) {
        try {
            JsonObject jsonObject = JsonParser.parseString(message).getAsJsonObject();
            if (jsonObject.has("longitude") && jsonObject.has("latitude")) {
                final double longitude = jsonObject.get("longitude").getAsDouble();
                final double latitude = jsonObject.get("latitude").getAsDouble();

                runOnUiThread(() -> {
                    latitudeTextView.setText("纬度: " + latitude);
                    longitudeTextView.setText("经度: " + longitude);

                    // 更新车辆在地图上的位置
                    updateCarLocationOnMap(latitude, longitude);
                });
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


//    @Override
//    public void onMessageReceived(String topic, String message) {
//        if ("car/location".equals(topic)) {
//            String[] location = message.split(",");
//            if (location.length == 2) {
//                double longitude = Double.parseDouble(location[0]);
//                double latitude = Double.parseDouble(location[1]);
//                longitudeTextView.setText("Longitude: " + longitude);
//                latitudeTextView.setText("Latitude: " + latitude);
//
//                // 更新车辆在地图上的位置
//                updateCarLocationOnMap(latitude, longitude);
//            }
//        }
//    }


}
