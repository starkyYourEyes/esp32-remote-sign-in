# esp32实现我去图书馆远程蓝牙签到
### 背景
由于冬天非常寒冷，很容易把我们冻死，所以早上人与床往往难舍难分~~（其实是因为懒）~~，但是另一方面又要早起去图书馆签到，因此我们很容易陷入两难的境地。  
### 实现
刚好前些日子在小黄鱼上看到了一个我去图书馆远程签到的软件，当即给他买了下来，很快，啊。但是呢，总的来说，很亏。因为需要两个移动设备才能签到成功，可咱现在没有 : <  
刚好这两天又看到了一个关于esp32蓝牙模拟应用的视频([bilibili](https://www.bilibili.com/video/BV1E54y137r3/?spm_id_from=333.880.my_history.page.click&vd_source=946d91ef8e978d916f518f74017b6842))，于是去GitHub上面搜了一下，果然找到一个关于用[esp32模拟蓝牙签到](https://github.com/zanjie1999/dingBle/tree/master)的项目，京东上面一看，一个esp32的板子才二十来块钱，直接就给他拿下了。等了三天，板子到货了，直接拿去跟着GitHub的教程跑了一遍，可惜并没有达到预期效果。  
于是慢慢顺着问题寻找解决方案，第一次知道原来这个数据线还分充电线和数据线的！充电线只能充电，数据线还可以进行数据传输，但我一开始用的几根线都是充电线（这些usb线都是当时买的一些其他设备送的，也怪不得），导致电脑已知无法识别板子，哎 : (，又花了十块钱买了根数据线（以前买东西都买的快递，现在发现有些东西直接外卖就行了~真的很快啊，半个小时就到了）然后就开始各种找教程找方法，一开始都是问的chatgpt，但是chatgpt给的代码全都是错误，果然AI还是靠不住啊，关键时候还是得靠自己。网上找了一圈，又找不到相关的代码，最后没办法，干脆直接开始学习esp32蓝牙开发。这里找到一个[很不错的教程](https://blog.csdn.net/Naisu_kun/article/details/115958024)，很是感谢。跟着教程一步一步，最结合上面的GitHub的部分代码，经过在宿舍的测试，最后也是实现了远程签到。  

---
以后在宿舍就能直接用手机签到了QWQ，爽。  

### 代码
以下是代码部分：
```cpp
#include <BLEDevice.h>
#define SERVICE_UUID "FDA50693-A4E2-4FB1-AFCF-C6EB07647825"  // 自己通过brightbeacon以及nRF Connect抓到的签到的UUID
uint8_t bleRaw[] = {0x02,0x01,0x06,0x1A,0xFF,0x4C,0x00,0x02,0x15,0xFD,0xA5,0x06,0x93,0xA4,0xE2,0x4F,0xB1,0xAF,0xCF,0xC6,0xEB,0x07,0x64,0x78,0x25,0x27,0xD7,0xA5,0xEA,0xC5};  // nRF Connect中抓到的广播数据
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("签到成功辽~");  // 好像并没什么卵用
      pServer->startAdvertising(); // 有客户端连接后广播会停止，这里再次打开使其它设备可以搜索到
    };
    void onDisconnect(BLEServer* pServer) {}
};

void setup() {
  Serial.begin(115200);
  Serial.println();
  BLEDevice::init("ESP32-BLE");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising(); // 获取Advertising对象
  // 设备信息设置成空白的
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  pAdvertising->setScanResponseData(oScanResponseData);
  // 里面有个 m_customScanResponseData = true; 和 m_customScanResponseData = true; 所以只能先随便设置一下
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  pAdvertising->setAdvertisementData(oAdvertisementData);
    // 简单粗暴直接底层api重新设置一下抓到的raw
  esp_err_t errRc = ::esp_ble_gap_config_adv_data_raw(bleRaw, 31);
  if (errRc != ESP_OK) {
    Serial.printf("esp_ble_gap_config_adv_data_raw: %d\n", errRc);
  }
  // pAdvertising->setMaxInterval(30);
  pAdvertising->addServiceUUID(SERVICE_UUID); // 广播Service的UUID
  BLEDevice::startAdvertising();
}

void loop() {}
```
### todo
目前还是早上醒来手动进行签到，下一步打算开发一个手机上面的脚本，早上到时间就直接自动进行签到，实现真正的解放双手。
