// 　供大安電子科自走車　競賽用 　循線　兩輪自走車
//   2014/12/5 original coding by jiader, modify by 周為
//

#include <QTRSensors.h>
#include <LiquidCrystal.h>

// -------------------------------腳位宣告-------------------------------
#define ML_1 5      // left motor pins
#define ML_2 6
#define MR_1 11      // right motor pins
#define MR_2 3
#define Buzzer 10
#define KeyA 13 
#define KeyB 12
#define KeyC 9
#define MAX_DISTANCE 300

#define NUM_SENSORS             5  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
LiquidCrystal lcd(13,2,4,7,8,12,9);
QTRSensorsAnalog qtra((unsigned char[]) {A2,A3,A4,A5,A6}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);  
unsigned int sensorValues[NUM_SENSORS];
unsigned int position;
int counter;
int maxs;
int par1,par2,par3,par4;
float Kp=5.0, Ki=0.0005, Kd=5.0;
//float Kp=1.0, Ki=0.0005, Kd=5.0;      // PID 參數值

void setup()
{
  pinMode(MR_1, OUTPUT);
  pinMode(MR_2, OUTPUT);
  pinMode(ML_1, OUTPUT);
  pinMode(ML_2, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  tone(Buzzer, 2000, 200);
  Serial.begin(57600);
  lcd.begin(8,2);
  
  ////////////////////////////////////////////////////////////////////////////////////////　校正  ////////////////////////////////////////////////////////////////////////
  for (counter=0; counter<80; counter++)
  {
    if (counter < 20 || counter >= 60)
        setSpeeds(90, -60);
    else
        setSpeeds(-80, 60);
    // This function records a set of sensor readings and keeps
    // track of the minimum and maximum values encountered.  The
    // IR_EMITTERS_ON argument means that the IR LEDs will be
    // turned on during the reading, which is usually what you
    // want.
    qtra.calibrate();
    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(10);
  }
  ////////////////////////////////////////////////////////////////////////////////////////　校正  ////////////////////////////////////////////////////////////////////////
  Stop();
  delay(1000);
}

uint8_t levels[] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};


byte level8[] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void load_custom_characters()
{
  lcd.createChar(0,levels ); // no offset, e.g. one bar
  lcd.createChar(1,levels + 1);
  lcd.createChar(2,levels + 2);
  lcd.createChar(3,levels + 3);
  lcd.createChar(4,levels + 4);
  lcd.createChar(5,levels + 5);
  lcd.createChar(6,levels + 6);
  
  lcd.clear(); // the LCD must be cleared for the characters to take effect
}

void display_readings()
{
  unsigned char i;

  for (i=0;i<5;i++) {
    // Initialize the array of characters that we will use for the
    // graph.  Using the space, an extra copy of the one-bar
    // character, and character 255 (a full black box), we get 10
    // characters in the array.
    const char display_characters[10] = { ' ', 0, 0, 1, 2, 3, 4, 5, 6, 255 };

    // The variable c will have values from 0 to 9, since
    // calibrated values are in the range of 0 to 1000, and
    // 1000/101 is 9 with integer math.
    //char c = ;

    // Display the bar graph character.
    lcd.write(display_characters[sensorValues[i] / 101]);
  }
}


void loop()
{ 
  lcd.setCursor(0, 0);
  maxs=100;
  par1=90;par2=-90;par3=165;par4=150;
  delay(1000);
  setSpeeds(0,0);
  while (readkey(KeyB)==HIGH) {  // 顯示感測器狀態, 按下按鍵 A 後離開
      lcd.setCursor(5,0);
      lcd.write(1);
      lcd.setCursor(0, 0);
      position = qtra.readLine(sensorValues,1,1); //,1,1
      lcd.print(position);
      lcd.setCursor(0,1);
      display_readings();
//      if (WaitData()) break;	// 若有藍牙資料傳入則跳出迴圈
      WaitData();
      delay(5);
  }
  delay(50);
  while (readkey(KeyB)==LOW)tone(Buzzer, 2000, 200);    // wait for release
  delay(500);
  tone(Buzzer, 2000, 200);
  delay(50);
  unsigned int last_proportional = 0;
  long integral = 0;
  int LM,RM;
  while (1) {
        position = qtra.readLine(sensorValues,1,1);  		// 讀取後5感測器的數值
	int proportional = (int)position - 2000;		// 計算與中間值的比例誤差量
	// 計算位置的變化量(derivative)與累積量integral
	int derivative = proportional - last_proportional;
	integral += proportional;
	// 記住上次的比例誤差量，並計算輸出控制量 power_difference
	last_proportional = proportional;
	int power_difference = proportional*Kp + integral*Ki + derivative*Kd;    /// 　計算 PID 輸出控制量
	// 限制 power_difference 的最大值範圍
	if (power_difference > maxs) power_difference = maxs;
	if (power_difference < -maxs)  power_difference = -maxs;
        // 計算左、右馬達實際控制量
	if (power_difference < 0) {
                LM=maxs + power_difference;
		RM=maxs;
	}
	else {
                LM=maxs;
		RM=maxs - power_difference;
	}
	setSpeeds(LM, RM); 
        delay(1);
/*
       lcd.clear();
       lcd.setCursor(0, 0);
       position = qtra.readLine(sensorValues,1,1); //,1,1
       lcd.print(position);
*/
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////  轉彎 /////////////////////////////////////////////////////////////////////////////////////////
void turnRight()
{
   setSpeeds(par1, par2);
   delay(par3);
   setSpeeds(0,0);
 /* for (counter=0; counter<par4; counter++) {
    pos0 = qtra0.readLine(sensor0Values,1,1);
    if (sensor0Values[1]<200)
       break;
    else
      if (sensor0Values[2]<200) setSpeeds(100, 0);
     
    delay(1);
 }
  */
}

void turnLeft()
{
   setSpeeds(par2, par1);
   delay(par3);
   setSpeeds(0,0);
 /* for (counter=0; counter<par4; counter++) {
    pos0 = qtra0.readLine(sensor0Values,1,1);
    if (sensor0Values[1]<200)
       break;
    else
      if (sensor0Values[2]<200) setSpeeds(100, 0);
     
    delay(1);
 }
  */
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////  轉彎 /////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////　馬達　///////////////////////////////////////////////////////////////////////////////////////
void setSpeeds(int m1Speed,int m2Speed)
{
	setM1Speed(m1Speed);
	setM2Speed(m2Speed);
}

// 設定右馬達轉速
void setM2Speed(int speed)
{
	unsigned char reverse = 0;

	if (speed < 0) {
		speed = -speed;	// make speed a positive quantity
		reverse = 1;	// preserve the direction
	}
	if (speed > 255)
		speed = 0xFF;
	else
        if (reverse) {
		digitalWrite(MR_2, LOW);
		analogWrite(MR_1, speed);
	}
	else {
		digitalWrite(MR_1, LOW);
		analogWrite(MR_2, speed);
	}
}

// 設定左馬達轉速
void setM1Speed(int speed)
{
	unsigned char reverse = 0;

	if (speed < 0) {
		speed = -speed;	// make speed a positive quantity
		reverse = 1;	// preserve the direction
	}
	if (speed > 255)
		speed = 0xFF;
	else
	if (reverse) {
		digitalWrite(ML_1, LOW);
		analogWrite(ML_2, speed);
	}
	else {
		digitalWrite(ML_2, LOW);
		analogWrite(ML_1, speed);
	}
}

void Brake()    // 煞車
{
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, HIGH);
}

void Stop()    //  停止的副程式
{
  digitalWrite(MR_1, LOW);  // 右輪停止
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);   // 左輪停止
  digitalWrite(ML_2, LOW);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////　馬達　///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 藍芽 ////////////////////////////////////////////////////////////////////////////////////////

// 從藍牙裝置讀取參數設定值
boolean WaitData() {
  boolean dt=false;
  if (Serial.available()) {		// 判斷串列埠緩衝區有無資料
    // get the headline word
    if (Serial.find("SP")) {		// 判斷串列資料的head是否為 SP
        maxs=Serial.parseInt();	// 從串列埠緩衝區中讀取下一個有效的整數資料
        maxs=constrain(maxs,0,255);
        Kp=Serial.parseFloat();
//        Ki=Serial.parseFloat();
        Kd=Serial.parseFloat();
        Serial.print("maxs=");
        Serial.print(maxs);              // 在 Serial Monitor 中顯示訊息
        Serial.print("\tKp=");
        Serial.println(Kp);
//        Serial.print("\tKi=");
//        Serial.println(Ki);
        Serial.print("\tKd=");
        Serial.println(Kd);
        tone(Buzzer,400,100);		// 嗶 1 聲通知 iPOE-A1 有收到新設定值
        dt=true;			// 設定有傳回值
    }
    Serial.flush();			// 清除串列埠緩衝區
  }
  return dt;
}

void Pause(byte ButtonPin)
{
  while (readkey(ButtonPin)==HIGH) {	// 若按鍵未按則繼續
    if (WaitData()) break;	// 若有藍牙資料傳入則跳出迴圈
    delay(5);
  }
  delay(50);	// 等待開關彈跳時間
  while (readkey(ButtonPin)==LOW);	// 等待按鍵放開
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////// 藍芽 ////////////////////////////////////////////////////////////////////////////////////////

boolean readkey(byte pin)
{
  pinMode(pin, INPUT_PULLUP);
  return digitalRead(pin);
}
