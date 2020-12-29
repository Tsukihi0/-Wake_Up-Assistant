
#include<ESP8266WiFi.h>
#define SENSOR A0
int KEY_NUM=0;
int count=0;
int state=0;
int val_AD=0;
int Weight=0;
int vout=0;
int val=0;
int R_val=0;


const char *ssid="TP-LINK1";
const char *password="xsy001122";
const IPAddress serverIP(192,168,137,216);
const int serverPort=8089;
WiFiClient client;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  
  WiFi.begin("TP-LINK1","xsy001122");
  
  
  while (WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());


}

void loop() {
  // put your main code here, to run repeatedly:
  val_AD=1024-analogRead(SENSOR);
  Serial.println("尝试访问服务器");
  if(client.connect(serverIP,serverPort))
  {
    Serial.println("访问成功");
    Serial.println(val_AD);
    
    client.print(val_AD);

    client.print('\r');
    Serial.println("关闭当前连接");
    client.stop();
  }
  else
  {
    Serial.println("访问失败");
    client.stop();
  }
  delay(500);
  
  
}
