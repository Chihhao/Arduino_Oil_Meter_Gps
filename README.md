# Arduino_Oil_Meter_Gps (機車儀錶板)  
## 連接原本的油量偵測電阻 (滿油10Ω，沒油100Ω)  
## 使用 NEO-7N GPS 模組取得速度與現在時間  
 
原厰的儀表損壞，換新太貴，發揮Maker精神！  
機車車種：(2006) SIZUKI GSR125  

## 成品如下  
![image](https://github.com/Chihhao/Arduino_Oil_Meter_Gps/blob/main/image/1.jpg)

## 功能  
1. 現在時間 (來自DS3231)(GPS校正)  
2. 速度顯示 (來自GPS)  
3. 電壓顯示 (利用電阻分壓，量測機車電瓶電壓)  
4. 油量顯示 (利用原厰的油量計)  

## 材料
(1) Arduino Pro Mini  
(2) LCD2004  
(3) DS3231 RTC (可以省略，因為GPS即可取得時間)  
(4) NEO-7N GPS Module  
(5) 7808 穩壓器 (7805~7808皆可)(最好加上散熱片)  
(6) 3.15A 保險絲 (差不多即可)  
(7) 220Ω 電阻 x 2  (大顆一點比較保險，1/2W以上)  
(8) 22KΩ 電阻 x 1  
(9) 10KΩ 電阻 x 1  
(10) 0.1uF  電容 x 1  
(11) 1uF    電容 x 1  
(12) 1000uF 電容 x 1  

## 函式庫 
(1) 請複製library目錄中的所有資料到Arduino的library目錄 (ex: C:\Users\USERNAME\Documents\Arduino\libraries)  
(2) 或者可以自行下載，網址如下  
https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library  
https://github.com/SodaqMoja/Sodaq_DS3231  
https://github.com/mikalhart/TinyGPSPlus  

## Wiring  
![image](https://github.com/Chihhao/Arduino_Oil_Meter_Gps/blob/main/image/wiring.png)  

## 原厰接線圖 (UT125同GSR125)  
![image](https://github.com/Chihhao/Arduino_Oil_Meter_Gps/blob/main/image/wiring2.png)  
![image](https://github.com/Chihhao/Arduino_Oil_Meter_Gps/blob/main/image/4.png)  
![image](https://github.com/Chihhao/Arduino_Oil_Meter_Gps/blob/main/image/5.png)  

## 照片  
![image](https://github.com/Chihhao/Arduino_Oil_Meter_Gps/blob/main/image/2.jpg)  
![image](https://github.com/Chihhao/Arduino_Oil_Meter_Gps/blob/main/image/3.jpg)  

