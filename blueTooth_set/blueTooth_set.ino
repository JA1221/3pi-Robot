// iPOE-A1的藍牙模組設定		梅克2工作室
// 請在模組的KEY腳接上5V，燒錄本程式後開啟Serial Monitor
// 即可設定
// AT+NAME 設定裝置名稱
// AT+UART 設定鮑率
// AT+PSWD 設定配對密碼
//Bluetooth Master Setup
//AT+ROLE=1
//AT+CMODE=1
//AT+PSWD=自訂
//AT+UART=自訂,0,0
//
//Bluetooth Slave Setup
//AT+ROLE=0
//AT+CMODE=0
//AT+PSWD=自訂
//AT+UART=自訂,0,0

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

void setup()
{
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
  Serial.begin(9600);		// 啟用串列埠, 鮑率9600bps, 使用者端
  mySerial.begin(38400);		// 啟用Serial2, 鮑率38400bps, 藍牙端(設定)
//  mySerial.begin(38400);    //藍芽對連鮑率(要查)
}

void loop()
{

  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (mySerial.available())
    Serial.write(mySerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    mySerial.write(Serial.read());
}
