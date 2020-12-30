
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


const char *ssid="NAME";//你的wifi名称 your wifi name
const char *password="PASSWORD";//你的wifi密码 your wifi password
const IPAddress serverIP(xxx,xxx,xxx,xxx);//你的服务器地址 the address of your server
const int serverPort=xxxx;//你的服务器端口 the port of your server
WiFiClient client;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  
  WiFi.begin("NAME","PASSWORD");//改 change this
  
  
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
