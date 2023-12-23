#include <BLEDevice.h>

#define SERVICE_UUID "FDA50693-A4E2-4FB1-AFCF-C6EB07647825"
uint8_t bleRaw[] = {0x02,0x01,0x06,0x1A,0xFF,0x4C,0x00,0x02,0x15,0xFD,0xA5,0x06,0x93,0xA4,0xE2,0x4F,0xB1,0xAF,0xCF,0xC6,0xEB,0x07,0x64,0x78,0x25,0x27,0xD7,0xA5,0xEA,0xC5};


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("签到成功辽~");
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







