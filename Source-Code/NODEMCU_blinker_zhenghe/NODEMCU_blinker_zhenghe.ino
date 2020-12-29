#define BLINKER_WIFI

#include <Blinker.h>

#include<Adafruit_NeoPixel.h>

#include<SoftwareSerial.h>

SoftwareSerial mySerial(D6,D7);//RX=D6,TX=D7

char auth[] = "0e3bf6e3a997";
char ssid[] = "TP-LINK1";
char pswd[] = "xsy001122";

char a='0';

#define RGB_1 "RGBKey"

BlinkerRGB WS2812(RGB_1);

BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");

BlinkerButton Button1("btn-rec");
BlinkerButton Button2("btn-play");


#ifdef __AVR__
#include<avr/power.h>
#endif

#define PIN D4
#define NUMPIXELS 24
Adafruit_NeoPixel pixels(NUMPIXELS,PIN,NEO_GRB+NEO_KHZ800);
#define DELAYVAL 500

#define PLAY D1
#define REC D2

int recordTime=5000;

// Download Adafruit DHT-sensor-library library here:
// https://github.com/adafruit/DHT-sensor-library
#include <DHT.h>

#define DHTPIN D5

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

uint32_t read_time = 0;

float humi_read, temp_read;

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    
    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    Blinker.print(BlinkerTime);
    Blinker.print("millis", BlinkerTime);
}

void heartbeat()
{
    HUMI.print(humi_read);
    TEMP.print(temp_read);
}
void ws2812_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("R value: ", r_value);
    BLINKER_LOG("G value: ", g_value);
    BLINKER_LOG("B value: ", b_value);
    BLINKER_LOG("Rrightness value: ", bright_value);

    pixels.setBrightness(bright_value);

    for(int i = 0; i < NUMPIXELS; i++){
        pixels.setPixelColor(i, r_value, g_value, b_value);
    }
    pixels.show();
}

void button1_callback(const String&state)
{
  BLINKER_LOG("get button state:",state);
  digitalWrite(REC,HIGH);
  delay(recordTime);
  digitalWrite(REC,LOW);
  Blinker.vibrate();
}

void button2_callback(const String&state)
{
  BLINKER_LOG("get button state:",state);
  digitalWrite(PLAY,HIGH);
  delay(50);
  digitalWrite(PLAY,LOW);
  
  delay(recordTime);
  
}

void setup()
{
    Serial.begin(9600);
    BLINKER_DEBUG.stream(Serial);

    mySerial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(PLAY,OUTPUT);
    pinMode(REC,OUTPUT);
    
    
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);

    Button1.attach(button1_callback);
    Button2.attach(button2_callback);

    WS2812.attach(ws2812_callback);
    dht.begin();

    #if defined(__AVR_ATtiny85__)&&(F_CPU==16000000)
    clock_prescale_set(clock_div_1);
    #endif
    pixels.begin();
}

void loop()
{
    Blinker.run();
    
    if (read_time == 0 || (millis() - read_time) >= 2000)
    {
        read_time = millis();

        float h = dht.readHumidity();
        float t = dht.readTemperature();        

        if (isnan(h) || isnan(t)) {
            BLINKER_LOG("Failed to read from DHT sensor!");
            return;
        }

        float hic = dht.computeHeatIndex(t, h, false);

        humi_read = h;
        temp_read = t;

        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        BLINKER_LOG("Heat index: ", hic, " *C");
    }

    while(mySerial.available())
    {
      a=mySerial.read();
    }
    if(a=='1')
    {
      digitalWrite(PLAY,HIGH);
      delay(50);
      digitalWrite(PLAY,LOW);
      delay(recordTime);
    }
}
