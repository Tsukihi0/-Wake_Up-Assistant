#include <UTFT.h>
#include<dht.h>
#include<Adafruit_NeoPixel.h>
#include<DS1302.h>
#include<Keypad.h>
#include<SoftwareSerial.h>

SoftwareSerial mySerial(49,51);//RX=49,TX=51
char TRIGGER='0';

#ifdef __AVR__
#include<avr/power.h>
#endif
//ws2812 led

dht DHT;

#define DHT22_PIN 7
//RH TP sensor

 //pressure sensor

#define SENSOR_THRESHOLD 300 //light sensor

#define PIN        6
#define NUMPIXELS 24
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500
//WS2812 led

DS1302 rtc(10,9,8);//rt clock

#define C_1 525
#define C_2 589
#define C_3 661
#define C_4 700
#define C_5 786
#define tonePin A8
//alarm tone



int val_AD=0;

//pressure sensor

char buf[50];
char day[10];
//rt clock usage

int tune[]={
  C_5
};
int duration[]={
  5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,10000,2500,2500
};
//alarm usage


int redstate=LOW;
int lastredbuttonstate=LOW;
//light on/off

//alarm set button

//alarm set button

int h=0;
int m=0;
int s=0;
//alarm origination

const byte ROWS=1;
const byte COLS=4;
char hexaKeys[ROWS][COLS]={{'1','2','3','4'}};
byte rowPins[ROWS]={11};
byte colPins[COLS]={5,4,3,2};
Keypad customKeypad=Keypad(makeKeymap(hexaKeys),rowPins,colPins,ROWS,COLS);
//keypad innitiation

UTFT myGLCD(YYROBOT_TFT144,A2,A1,A5,A4,A3);  // Remember to change the model parameter to suit your display module!
//YYROBOT_TFT144  屏幕型号，不用修改
//SDA----A2
//SCL----A1
//CS-----A5
//RST----A4
//RS----A3
//LED---A0  UTFT库里面设定的，如果需要修改需要修改库文件

extern uint8_t SmallFont[];//原始文件在库文件的DefaultFonts.c中
extern uint8_t BigFont[];//原始文件在库文件的DefaultFonts.c中
extern uint8_t SevenSegNumFont[];//原始文件在库文件的DefaultFonts.c中
const uint8_t tfont16[] =
{

0x04,0x01,0x04,0x01,0x04,0x02,0x04,0x04,0x04,0x08,0x04,0x30,0x04,0xC0,0xFF,0x00,
0x04,0xC0,0x04,0x30,0x04,0x08,0x04,0x04,0x04,0x02,0x04,0x01,0x04,0x01,0x00,0x00,/*"大",0*/
0x02,0x20,0x0C,0x20,0x88,0x20,0x69,0x20,0x09,0x20,0x09,0x22,0x89,0x21,0x69,0x7E,
0x09,0x60,0x09,0xA0,0x19,0x20,0x28,0x20,0xC8,0x20,0x0A,0x20,0x0C,0x20,0x00,0x00,/*"学",1*/
0x01,0x02,0x02,0x02,0x0C,0x42,0x78,0x42,0x08,0x42,0x08,0x42,0x08,0x42,0xFF,0xFE,
0x08,0x42,0x08,0x42,0x08,0x42,0x08,0x42,0x08,0x42,0x08,0x02,0x00,0x02,0x00,0x00,/*"生",2*/
0x00,0x00,0x00,0x00,0x1F,0xF8,0x11,0x10,0x11,0x10,0x11,0x10,0x11,0x10,0xFF,0xFE,
0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x1F,0xF9,0x00,0x01,0x00,0x0F,0x00,0x00,/*"电",3*/
0x01,0x00,0x41,0x00,0x41,0x00,0x41,0x00,0x41,0x00,0x41,0x02,0x41,0x01,0x47,0xFE,
0x45,0x00,0x49,0x00,0x51,0x00,0x61,0x00,0x41,0x00,0x01,0x00,0x01,0x00,0x00,0x00,/*"子",4*/
0x00,0x00,0x20,0x00,0x27,0xFF,0x24,0x40,0x34,0x80,0x2D,0x78,0xA4,0x48,0x64,0x48,
0x24,0x48,0x2D,0x78,0x34,0x82,0x24,0x41,0x27,0xFE,0x20,0x00,0x00,0x00,0x00,0x00,/*"商",5*/
0x02,0x80,0x0C,0x80,0xF7,0xFE,0x24,0x84,0x24,0x88,0x00,0x00,0x27,0xFF,0x24,0x90,
0x24,0x90,0xFF,0xFE,0x24,0x90,0x24,0x92,0xA4,0x91,0x67,0xFE,0x20,0x00,0x00,0x00,/*"铺",6*/


};


void print_time()
{
  Time t = rtc.time();
  memset(day, 0, sizeof(day));
    switch (t.day)
    {
    case 1: strcpy(day, ""); break;
    case 2: strcpy(day, ""); break;
    case 3: strcpy(day, ""); break;
    case 4: strcpy(day, ""); break;
    case 5: strcpy(day, ""); break;
    case 6: strcpy(day, ""); break;
    case 7: strcpy(day, ""); break;
    }
   
    snprintf(buf, sizeof(buf),"%s%04d-%02d-%02d %02d:%02d %02d", day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);
    //Serial.println(buf);
}

void setup()
{
  randomSeed(analogRead(0));
// Setup the LCD
  myGLCD.InitLCD();//初始化液晶
  myGLCD.InitLCD();//初始化两次有利于系统稳定
  myGLCD.setFont(SmallFont);//设置字体为SmallFont格式
  
  Serial.begin(9600);
  
  
  myGLCD.setColor(255, 255, 255);//设置字体颜色
  myGLCD.setBackColor(0, 0, 0);//设置背景颜色
  myGLCD.clrScr(); //清屏
  myGLCD.setFont(BigFont);//设置字体为SmallFont格式(8*12字符)
  myGLCD.print((const)"RH(%): ", 0, 0);  //打印HelloWorld，中间显示，显示在20行  
  myGLCD.setFont(BigFont); //设置大字体BigFont（16*16字符）
  myGLCD.print((const)"TP(C): ", 0, 20);  //打印YYROBOT，靠左显示，显示在20行 
  myGLCD.setFont(BigFont);
  myGLCD.print((const)"TIME ",30,40); 
  myGLCD.setFont(BigFont);
  myGLCD.print((const)"ALARM ",23,70); 
  
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();


  mySerial.begin(115200);
}

int setH(int h)
{
  if(h<24&&h>=0)
  h++;
  else if(h>=24)
  h=0;
  return h;
}

int setM(int m)
{
  if(m<60&&h>=0)
  m++;
  else if(m>=24)
  m=0;
  return m;
}

int setS(int s)
{
  if(s<24&&s>=0)
  s++;
  else if(s>=24)
  s=0;
  return s;
}



void loop()
{  
  int chk=DHT.read22(DHT22_PIN);
  char a[20]=" ";
  int i=0;
  while(!Serial.available()){break;}
  while(Serial.available())
  {
    a[i]=Serial.read();
    i++;
    delay(3); 
    
      
  }
  int u=0,v=0,b=0;
  for(u=5;u>=0;u--)
  {
    if(a[u]>48&&a[u]<57)
    
    b=b+(a[u]-48)*(10^v);
    v++;
  }
  

  
  char customKey=customKeypad.getKey();
  
  


  if(customKey=='2')
  h=setH(h);

  else if(customKey=='3')
  m=setM(m);

  else if(customKey=='4')
  s=setS(s);

  
  Time t = rtc.time();
  
  
  
  val_AD =b;//pressure sensor val

  int light=1023-analogRead(7);//light sensor val
  int light1=analogRead(7);
  

  myGLCD.setColor(255, 255, 255);//设置字体颜色
  myGLCD.setBackColor(0, 0, 0);//设置背景颜色
 
  myGLCD.setFont(SmallFont);
  
  print_time();
  myGLCD.print(buf,0,55);
  myGLCD.printNumI(h,30,85);
  myGLCD.print((const)":",45,85);
  myGLCD.printNumI(m,60,85);
  myGLCD.print((const)":",75,85);
  myGLCD.printNumI(s,90,85);
  
  myGLCD.printNumF((float)DHT.humidity ,1,95,2);
  myGLCD.printNumF((float)DHT.temperature ,1,95,22);
  

  
  
  
  delay(500);
  myGLCD.print((const)"    ",95,2);
  myGLCD.print((const)"    ",95,22);
  myGLCD.print((const)"          ",30,85);
  
  //lcd display
  
  if(val_AD!=0)
  {
    pixels.clear();
    pixels.fill(pixels.Color(0,0,0));
    pixels.show();
    
    
    if(h==t.hr&&m==t.min&&s==t.sec)
    {
      ////tone(tonePin,C_1,20000);
      //alarm noise
      TRIGGER='1';
      mySerial.print(TRIGGER);
      pixels.clear();
      pixels.fill(pixels.Color(255,94,0));
      pixels.show();
      
    }
    int lightdelay=0;
    int flag1=m-20;
    int flag2=m+40;
    if(m>=20&&m-20==t.min&&h==t.hr&&s==t.sec)
    {
      for(flag1=flag1;flag1<m;flag1++)
      { 
        lightdelay+=1;
        pixels.clear();
        pixels.fill(pixels.Color(lightdelay/5,94*lightdelay/1275,0));
        pixels.show();
        delay(4706);
      }
    }
    else if(m<20&&(m+40)==t.min&&(h-1)==t.hr&&s==t.sec)
    {
      for(flag2=flag2-60;flag2-60<m;flag2++)
      {
        lightdelay+=1;
        pixels.clear();
        pixels.fill(pixels.Color(lightdelay/5,94*lightdelay/1275,0));
        pixels.show();
        delay(4706);
      }
    }
  }
 
  
    noTone(tonePin);
    
  
  if(customKey=='1')
  {
    
    
      redstate=!redstate;
     
    
  }
  

  if(redstate==HIGH)
  {
    if(light1<=30)
    {
      pixels.clear();
      pixels.fill(pixels.Color(0,0,0));
      pixels.show();
    }
    else 
    {
      pixels.clear();
      pixels.fill(pixels.Color((double)light1/5,(double)94/1275*light1,0));
      pixels.show();
    }
    delay(1);
    noTone(tonePin);
  //light detect controll
  }
  else
  {
    pixels.clear();
    pixels.fill(pixels.Color(0,0,0));
    pixels.show();
    noTone(tonePin);
  }
  
}
