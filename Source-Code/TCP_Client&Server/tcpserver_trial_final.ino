#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
SoftwareSerial mySerial(D5,D6);//RX=D1,TX=D2


const char *ssid="NAME";//你的wifi名称 your wifi name
const char *password="PASSWORD";//你的wifi密码 your wifi password
WiFiServer server(8089);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  mySerial.begin(9600);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  WiFi.begin("NAME","");
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());
  server.begin();
  }
  

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client=server.available();
  if (client)
  {
    Serial.println("[Client connected]");
    String readBuff;
    
    while(client.connected())
    {
      if (client.available())
      {
        char c=client.read();
        readBuff+=c;
        
        if(c=='\r'||c=='\n')
        {
          client.print("Received:"+readBuff);
          Serial.println("Received:"+readBuff);
          mySerial.print(readBuff);
          readBuff="";
          break;
        }
      }
    }
    client.stop();
    Serial.println("[Client disconnected]");
  }
}
