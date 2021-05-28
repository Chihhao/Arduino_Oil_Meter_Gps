#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Sodaq_DS3231.h"
#include <TinyGPS++.h>

#define BATTERY_PIN A0
#define OIL_PIN A1
#define OIL_PIN_VCC 13

#define MAX_COL 20
#define MAX_ROW 4

static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
int iMySpeed = 0;
int iST = 0; //取得衛星數量
bool bNeedSyncTime = true;
  
LiquidCrystal_I2C lcd(0x27, MAX_COL, MAX_ROW);
DateTime dt(2020, 9, 12, 10, 00, 00, 6); // 年 月 日 時 分 秒 星期。週日-週六對應0-6

char WW[7][4]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
byte myChar1[8] = {B00000, B10000, B10000, B10000, B10000, B10000, B00000};
byte myChar2[8] = {B00000, B11000, B11000, B11000, B11000, B11000, B00000};
byte myChar3[8] = {B00000, B11100, B11100, B11100, B11100, B11100, B00000};
byte myChar4[8] = {B00000, B11110, B11110, B11110, B11110, B11110, B00000};
byte myChar5[8] = {B00000, B11111, B11111, B11111, B11111, B11111, B00000};

void setup(){   
  pinMode(OIL_PIN_VCC, OUTPUT);
  
  Serial.begin(9600); //for gps
  lcd.begin();
  lcd.createChar(0, myChar1);
  lcd.createChar(1, myChar2);
  lcd.createChar(2, myChar3);
  lcd.createChar(3, myChar4);
  lcd.createChar(4, myChar5);  
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("                    ");  
  lcd.setCursor(0, 1);
  lcd.print("Spd           |     ");
  lcd.setCursor(0, 2);
  lcd.print("Bat           |     ");
  lcd.setCursor(0, 3);
  lcd.print("Ful           |     ");
  rtc.begin();
  // 第一次使用時鐘模塊，或者需要校準時放開下列註釋
  // 一旦校準完畢，繼續註釋掉，並再次上傳
  // 定義dt的時候建議預留一些編譯和上傳的時間
  //rtc.setDateTime(dt);  
}

void loop(){  
  if(bNeedSyncTime) syncTime();
  printTime();
  printSpeed();
  printBattery();
  printFuel();
  smartDelay(1000);
}

void Clear(){
  for(int i=0; i<MAX_ROW; i++){
    lcd.setCursor(0, i);
    for(int j=0; j<MAX_COL; j++){
      lcd.print(" ");
    }
  }
  lcd.setCursor(0, 0);
}

int getOilPercent(float fVolt){
  if(fVolt<=0.41) {return 100;}
  if(fVolt>=2.25) {return 0;}
  int rtn=0;
  rtn=(int)(100*(fVolt-0.41)/(2.25-0.41));  
  return 100-rtn;
}

void printTime(){
  char sTemp[20];  
  DateTime now = rtc.now();
  int yy = now.year();
  int mm = now.month();
  int dd = now.date();  
  int hh = now.hour();
  int mn = now.minute();
  int ss = now.second();
  int ww = now.dayOfWeek(); 
  if(ww==7) ww=0;
  sprintf(sTemp, "%s %04d-%02d-%02d %02d:%02d", WW[ww],yy,mm,dd,hh,mn);
  lcd.setCursor(0,0);
  lcd.print(sTemp);
}

void printSpeed(){
  char sTemp[20];

  if(gps.charsProcessed() < 10){
    lcd.setCursor(0, 1);
    lcd.print("Spd NO DETECT |  0KM");
    return;
  } 
  
  if(gps.satellites.isValid()){
    iST = (int)gps.satellites.value();
    if(iST > 0){
      iMySpeed = round(gps.speed.kmph());
    }
    else{
      iMySpeed = 0;
    }
  }
  else{
    iST = 0;
  }

  if(iST==0){
    lcd.setCursor(0, 1);
    lcd.print("Spd NO SIGNAL |  0KM");
    return;    
  }
  
  if(iMySpeed<=1) iMySpeed=0; 
  if(iMySpeed>=100) iMySpeed=100;
  int iNoOfTen = iMySpeed/10;
  int iNoOfOne = iMySpeed%10;
  lcd.setCursor(4, 1);
  for(int i=0; i<iNoOfTen; i++){ lcd.write(byte(4));}
  if(iNoOfTen<10){
    if(iNoOfOne>=8)       { lcd.write(byte(3));}
    else if (iNoOfOne>=6) { lcd.write(byte(2));}
    else if (iNoOfOne>=4) { lcd.write(byte(1));}
    else if (iNoOfOne>=2) { lcd.write(byte(0));}
    else {lcd.print(" ");}
    for(int i=0; i<9-iNoOfTen; i++){ lcd.print(" ");}
  }  
  
  lcd.setCursor(15, 1);
  sprintf(sTemp, "%3dKM", iMySpeed);    
  lcd.print(sTemp);
}

void printBattery(){
  char sTemp[20];    
  int volt_1024 = analogRead(BATTERY_PIN);
  float fVolt12 = (5.0/1024.0) * volt_1024 * (32.0/10.0);  
  char sVolt[6]; dtostrf(fVolt12, 4, 1, sVolt);

  int iPer = map((int)(fVolt12*10), 108, 140, 0, 100);  
  if(iPer<=0) iPer=0; 
  if(iPer>=100) iPer=100;
  int iNoOfTen = iPer/10;
  int iNoOfOne = iPer%10;
  lcd.setCursor(4, 2);
  for(int i=0; i<iNoOfTen; i++){ lcd.write(byte(4));}
  if(iNoOfTen<10){
    if(iNoOfOne>=8)       { lcd.write(byte(3));}
    else if (iNoOfOne>=6) { lcd.write(byte(2));}
    else if (iNoOfOne>=4) { lcd.write(byte(1));}
    else if (iNoOfOne>=2) { lcd.write(byte(0));}
    else {lcd.print(" ");}
    for(int i=0; i<9-iNoOfTen; i++){ lcd.print(" ");}
  }  
  
  lcd.setCursor(15, 2);
  sprintf(sTemp, "%sV", sVolt);    
  lcd.print(sTemp);
}

void printFuel(){
  char sTemp[20];  
  digitalWrite(OIL_PIN_VCC, HIGH);
  delay(5);  
  int volt_1024 = analogRead(OIL_PIN);  
  digitalWrite(OIL_PIN_VCC, LOW);
  float fVolt5 = (5.0/1024.0) * volt_1024;
  //char sVolt[6]; mdtostrf(fVolt5, 3, 1, sVolt);

  int iPer = getOilPercent(fVolt5);
  if(iPer<=0) iPer=0; 
  if(iPer>=100) iPer=100;
  int iNoOfTen = iPer/10;
  int iNoOfOne = iPer%10;
  lcd.setCursor(4, 3);
  for(int i=0; i<iNoOfTen; i++){ lcd.write(byte(4));}
  if(iNoOfTen<10){
    if(iNoOfOne>=8)       { lcd.write(byte(3));}
    else if (iNoOfOne>=6) { lcd.write(byte(2));}
    else if (iNoOfOne>=4) { lcd.write(byte(1));}
    else if (iNoOfOne>=2) { lcd.write(byte(0));}
    else {lcd.print(" ");}
    for(int i=0; i<9-iNoOfTen; i++){ lcd.print(" ");}
  }
    
  sprintf(sTemp, " %3d%%", iPer);
  lcd.setCursor(15, 3);
  lcd.print(sTemp);
}

static void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do{
    while (Serial.available())
      gps.encode(Serial.read());
  } while (millis() - start < ms);
}

void syncTime(){
  if(gps.charsProcessed() < 10) return;   
  if (!gps.date.isValid() || !gps.time.isValid()) return;
  int YYYY = gps.date.year();
  int MM = gps.date.month();
  int DD = gps.date.day();
  int hh24 = gps.time.hour()+8;
  int mm = gps.time.minute();
  int ss = gps.time.second();
  int ww = calcDayOfWeek(DD, MM, YYYY);
  if (YYYY<2021) return;
  DateTime dt(YYYY, MM, DD, hh24, mm, ss, ww); // 年 月 日 時 分 秒 星期。週日-週六對應0-6
  rtc.setDateTime(dt); 
  bNeedSyncTime = false;
}

int calcDayOfWeek(int d, int m, int y){                                                                        // https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7; 
    // Sun=0, Mon=1, Tue=2, Wed=3, Thu=4, Fri=5, Sat=6
}
