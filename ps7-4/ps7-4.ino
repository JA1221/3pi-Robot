#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
Pololu3pi robot;
unsigned int sensors[5];
#include <avr/pgmspace.h>
const char welcome_line1[] PROGMEM = " Pololu";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "Monster-";
const char demo_name_line2[] PROGMEM = "  Trio  ";
const char welcome[] PROGMEM = ">g32>>c32";
//const char go[] PROGMEM = "L16 cdegreg4";
const char go[] PROGMEM = ">>a32";
int maximum;
float kp=8;
float kd=4;
byte mode=1;
byte mode2;
byte fix;
int sl=16;
unsigned long ST;//藍芽用
unsigned int t;//站存時間
unsigned long startTime;//路段讀秒
unsigned long Stopwatch;//碼表計時
byte modeAspeed=170;
byte modeBspeed=255;
float AtoB=0.625;
byte as=70;
//int T[42];//衝刺秒數
int T[43]=
{
  263,45,252,442,92,628,85,83,70,80,
  97,85,257,95,626,85,80,88,80,104,
  83,83,70,50,63,212,770,90,435,263,
  76,56,61,81,78,90,82,90,87,255,
  85,349,50
};
int T2[42];//衝刺秒數(暫存)
const char levels[] PROGMEM = {
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
void load_custom_characters()
{
  OrangutanLCD::loadCustomCharacter(levels + 0, 0); // no offset, e.g. one bar
  OrangutanLCD::loadCustomCharacter(levels + 1, 1); // two bars
  OrangutanLCD::loadCustomCharacter(levels + 2, 2); // etc...
  OrangutanLCD::loadCustomCharacter(levels + 3, 3);
  OrangutanLCD::loadCustomCharacter(levels + 4, 4);
  OrangutanLCD::loadCustomCharacter(levels + 5, 5);
  OrangutanLCD::loadCustomCharacter(levels + 6, 6);
  OrangutanLCD::clear(); // the LCD must be cleared for the characters to take effect
}

// This function displays the sensor readings using a bar graph.
void display_readings(const unsigned int *calibrated_values)
{
  unsigned char i;

  for (i=0;i<5;i++) {
    const char display_characters[10] = { 
      ' ', 0, 0, 1, 2, 3, 4, 5, 6, 255     };
    char c = display_characters[calibrated_values[i] / 101];
    OrangutanLCD::print(c);
  }
}
void setup()//開機執行一次
{
  Serial.begin(57600);//RD0,RD1->RX,TX
  unsigned int counter; // used as a simple timer
  robot.init(2000);
  load_custom_characters(); // load the custom characters
  OrangutanLCD::printFromProgramSpace(welcome_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_line2);
  OrangutanBuzzer::playFromProgramSpace(welcome);
  delay(500);
  OrangutanLCD::clear();
  OrangutanLCD::printFromProgramSpace(demo_name_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(demo_name_line2);
  delay(1500);
  //---------------------------等待按B 顯示電池電量----------------------------------------
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    int bat = OrangutanAnalog::readBatteryMillivolts();
    OrangutanLCD::clear();
    OrangutanLCD::print(bat);
    OrangutanLCD::print("mV");
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print("Press B");
    delay(100);
  }
  OrangutanPushbuttons::waitForRelease(BUTTON_B);
  delay(1000);
  //------------------------------讀線--------------------------------
  for (counter=0; counter<80; counter++)
  {
    if (counter < 20 || counter >= 60)
      OrangutanMotors::setSpeeds(40, -40);
    else
      OrangutanMotors::setSpeeds(-40, 40);
    robot.calibrateLineSensors(IR_EMITTERS_ON);
    delay(20);
  }
  OrangutanMotors::setSpeeds(0, 0);
  //--------------------等待按B 顯示黑白條------------------------------
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    OrangutanLCD::clear();
    OrangutanLCD::print(position);
    OrangutanLCD::gotoXY(0, 1);
    display_readings(sensors);
    delay(100);
  }
  OrangutanPushbuttons::waitForRelease(BUTTON_B);

  OrangutanLCD::clear(); 

  OrangutanLCD::print("Go!");    
  OrangutanBuzzer::playFromProgramSpace(go);
  while(OrangutanBuzzer::isPlaying());
}
//---------------------------------------------------------------------------

char path[100] ="SLLRRLLRLRLLRRRRLLRRLSSSSRRRLLSSLRRLRLLLR";
unsigned char path_length = 41; // the length of the path
void display_path()
{
  path[path_length] = 0;
  
  OrangutanLCD::clear();
  OrangutanLCD::print(path);

  if (path_length > 8)
  {
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print(path + 8);
  }
}
//======================================================================================

void loop()
{
  while (1)
  {
    if(mode==1)maximum = modeAspeed;       //初始速度
    else if(mode==2)maximum = 255;       
    OrangutanMotors::setSpeeds(0, 0);
    OrangutanBuzzer::play(">>a32");
    while (!OrangutanPushbuttons::isPressed(BUTTON_B) || mode==5)
    {
      Mode_Selection();
      Mode_Display();
      Bluetooth1();
    }
    //---------------------------線迷宮開始------------------------------
    while (OrangutanPushbuttons::isPressed(BUTTON_B));
    delay(1000);
    Stopwatch=millis();//計時開始 
    if(mode==1)ModeA();
    else if(mode==2)ModeB();
    else if(mode==3)ModeC();
    else if(mode==4)ModeD();
    finish();
  }
}
