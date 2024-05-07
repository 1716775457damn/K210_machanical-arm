#include <ThingsCloudWiFiManager.h>
#include <ThingsCloudMQTT.h>

//======================================================
// 设置 ssid / password，连接到你的 WiFi AP
const char *ssid = "TP-702";
const char *password = "18608415436";
// 在 ThingsCloud 控制台的设备详情页中，复制以下设备连接信息
// https://console.thingscloud.xyz
#define THINGSCLOUD_MQTT_HOST "gz-3-mqtt.iot-api.com"     // MOTT 服多器域名
#define THINGSCLOUD_PROJECT_KEY "sPq1aVSb20"              // 同一个项目所有设备的 ProjectKey相同
#define THINGSCLOUD_TYPE_KEY "tmv2muqlu2"          // 设备类型的 TypeKey，用于自动创建设备
#define THINGSCLOUD_API_ENDPOINT "http://gz-3-api.iot-api.com"      //用于获取设备证书以及自动创建设
//======================================================

ThingsCloudMQTT client(
    THINGSCLOUD_MQTT_HOST,
    "", // DeviceKey 留空，SDK 自动为模组生成唯一标识作为 DeviceKey
    THINGSCLOUD_PROJECT_KEY,
    THINGSCLOUD_TYPE_KEY,
    THINGSCLOUD_API_ENDPOINT);
// ESP模组生成 WiFi AP，用于配网
#define WiFi_AP_SSID "ESP32-TXW" // AP 的 SSID 前缀，出现在用户 App 的 WiFi 列表中，可修改为你喜欢的名称
#define WiFi_AP_PASSWORD ""         // AP 的连接密码，可以不设置
ThingsCloudWiFiManager wm(WiFi_AP_SSID, WiFi_AP_PASSWORD);

// 设置继电器的输入引脚
#define RELAY1_PIN 5
#define RELAY2_PIN 18
#define RELAY3_PIN 19
#define RELAY4_PIN 21

void setup() {
  Serial.begin(115200);

  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY2_PIN, HIGH);
  pinMode(RELAY3_PIN, OUTPUT);
  digitalWrite(RELAY3_PIN, HIGH);  
  pinMode(RELAY4_PIN, OUTPUT);
  digitalWrite(RELAY4_PIN, HIGH);

  // 允许 SDK 的日志输出
  client.enableDebuggingMessages();
  // 关联 MQTT 客户端和配网管理器
  wm. linkMQTTClient(&client);
  // 调试配网可以取消以下注释，用于清空之前保存的 WiFi 配网信息，每次设备开机都需要重新配网。
  // 实际产品中，将清空配网信息放在例如设备的按键事件中，由用户操作触发设备重新进入配网状态。
  // wm.resetSettings();
  // 如果设备未配网，则启动 AP 配网模式，等待 ThingsX App 为设备配网
  // 如果已配网，则直接连接 WiFi
  if (!wm.autoConnect()){
    Serial.println("\nWiFi provisioning failed, will restart to retry");
    delay(1000);
    ESP.restart();
  }
}

void handleAttributes(const JsonObject &obj) {
  if (obj.containsKey("relay1")) {
    // 接收到下发的 relay 属性
    if (obj["relay1"] == true) {
      // 闭合继电器，输出低电平
      digitalWrite(RELAY1_PIN, LOW);
      Serial.println("relay1 switch to ON");
    } else {
      // 断开继电器，输出高电平
      digitalWrite(RELAY1_PIN, HIGH);
      Serial.println("relay1 switch to OFF");
    }
  }
  if (obj.containsKey("relay2")) {
    // 接收到下发的 relay 属性
    if (obj["relay2"] == true) {
      // 闭合继电器，输出低电平
      digitalWrite(RELAY2_PIN, LOW);
      Serial.println("relay2 switch to ON");
    } else {
      // 断开继电器，输出高电平
      digitalWrite(RELAY2_PIN, HIGH);
      Serial.println("relay2 switch to OFF");
    }
  }
  if (obj.containsKey("relay3")) {
    // 接收到下发的 relay 属性
    if (obj["relay3"] == true) {
      // 闭合继电器，输出低电平
      digitalWrite(RELAY3_PIN, LOW);
      Serial.println("relay3 switch to ON");
    } else {
      // 断开继电器，输出高电平
      digitalWrite(RELAY3_PIN, HIGH);
      Serial.println("relay3 switch to OFF");
    }
  }
  if (obj.containsKey("relay4")) {
    // 接收到下发的 relay 属性
    if (obj["relay4"] == true) {
      // 闭合继电器，输出低电平
      digitalWrite(RELAY4_PIN, LOW);
      Serial.println("relay4 switch to ON");
    } else {
      // 断开继电器，输出高电平
      digitalWrite(RELAY4_PIN, HIGH);
      Serial.println("relay4 switch to OFF");
    }
  }
}

// 必须实现这个回调函数，当 MQTT 连接成功后执行该函数。
void onMQTTConnect() {

  // 订阅获取属性的回复消息
  client.onAttributesGetResponse([](const String &topic, const JsonObject &obj) {
    if (obj["result"] == 1) {
      handleAttributes(obj["attributes"]);
    }
  });

  // 订阅云平台下发属性的消息
  client.onAttributesPush([](const JsonObject &obj) {
    handleAttributes(obj);
  });

  // 读取设备在云平台上的属性，用于初始化继电器状态。
  // 云平台回复的属性会进入上边 client.onAttributesGetResponse 接收的消息。
  client.getAttributes();
}

void loop() {
  client.loop();
}


