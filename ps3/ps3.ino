/*
 * Simple3piMazeSolver - demo code for the Pololu 3pi Robot
 * 
 * This code will solve a line maze constructed with a black line on a
 * white background, as long as there are no loops.  It has two
 * phases: first, it learns the maze, with a "left hand on the wall"
 * strategy, and computes the most efficient path to the finish.
 * Second, it follows its most efficient solution.
 *
 * http://www.pololu.com/docs/0J21
 * http://www.pololu.com
 * http://forum.pololu.com
 *
 */
#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>
#include <SoftwareSerial.h>
Pololu3pi robot;
unsigned int sensors[5];
#include <avr/pgmspace.h>
const char welcome_line1[] PROGMEM = " Pololu";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "Maze";
const char demo_name_line2[] PROGMEM = "solver";
const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";
int maximum;
float kp=8;
float kd=2.7;
byte mode=1;
unsigned long startTime;
unsigned long Stopwatch;
float AtoB=0.625;
//int f=130;
//int T[42]=
//{
//   325,f-30,325,550,f,750,f,f,f,f    //1~10
//  ,f,f,335,f,800,f,f,f,f,f      //11~20
//  ,f,f,f,f,f,325,1050,f,550,325 //21~30
//  ,f,f,f,f,f,f,f,f,f,325        //31~40 
//  ,f   //41
//  ,400 //end
//};
int T[42]; 
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
  unsigned int counter; // used as a simple timer
  robot.init(2000);
  load_custom_characters(); // load the custom characters
  OrangutanLCD::printFromProgramSpace(welcome_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_line2);
  OrangutanBuzzer::playFromProgramSpace(welcome);
  delay(1000);
  OrangutanLCD::clear();
  OrangutanLCD::printFromProgramSpace(demo_name_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(demo_name_line2);
  delay(1000);
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
//=================================循跡======================================================

//-----------------------------循跡(正常)----------------------------------------------
void follow_segment()
{
  int last_proportional = 0;
  long integral=0;

  while(1)
  {
    unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    int proportional = ((int)position) - 2000;
    int derivative = proportional - last_proportional;
    integral += proportional;
    last_proportional = proportional;
    int power_difference = proportional/kp + integral/10000 + derivative*kd;
    if (power_difference > maximum)
      power_difference = maximum;
    if (power_difference < -maximum)
      power_difference = -maximum;

    if (power_difference < 0)
      OrangutanMotors::setSpeeds(maximum + power_difference, maximum);
    else
      OrangutanMotors::setSpeeds(maximum, maximum - power_difference);

    if (sensors[1] >800 && sensors[2] >800 && sensors[3] >800)
    {
      return;
    }
    else if (sensors[0] < 200 || sensors[4] <200)
    {
      // Found an intersection.
      return;
    }
  }
}
//-------------------------循跡(加速用)-----------------------------------
void follow_segment2(byte t)
{
  int last_proportional = 0;
  long integral=0;
  while(1)
  {
    unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    int proportional = ((int)position) - 2000;
    int derivative = proportional - last_proportional;
    integral += proportional;
    last_proportional = proportional;
    int power_difference = proportional/kp + integral/10000 + derivative*kd;
    if (power_difference > maximum)
      power_difference = maximum;
    if (power_difference < -maximum)
      power_difference = -maximum;

    if (power_difference < 0)
      OrangutanMotors::setSpeeds(maximum + power_difference, maximum);
    else
      OrangutanMotors::setSpeeds(maximum, maximum - power_difference);
    if(millis()-startTime>T[t])
    {
      if(t==0 || t==21 || t==22 || t==23 || t==24 || t==30 || t==31)//遇S不減速
      {
        return;
      }      
      else
      {        
        maximum=170;
        return;
      }
    }
    if (sensors[1] >800 && sensors[2] >800 && sensors[3] >800)
    {
      return;
    }
    else if (sensors[0] < 200 || sensors[4] <200)
    {
      // Found an intersection.
      return;
    }

  }
}
//=================================================================================

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

unsigned char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{

  if (found_left)
    return 'L';
  else if (found_straight)
    return 'S';
  else if (found_right)
    return 'R';
  else
    return 'B';
}
void loop()
{
  while (1)
  {
    if(mode==1)maximum = 160;       //初始速度
    else if(mode==2)maximum = 255;
       
    OrangutanMotors::setSpeeds(0, 0);
    OrangutanBuzzer::play(">>a32");
    while (!OrangutanPushbuttons::isPressed(BUTTON_B))
    {
      //----------------模式選擇----------------
      if(OrangutanPushbuttons::isPressed(BUTTON_C))
      {
        if(mode==1)mode++;
        maximum = 255;//起始速度
        OrangutanBuzzer::play(">>a32");
        OrangutanPushbuttons::waitForRelease(BUTTON_C);
      }
      else if(OrangutanPushbuttons::isPressed(BUTTON_A))
      {
        if(mode==2)mode--;
        maximum = 160;//起始速度
        OrangutanBuzzer::play(">>a32");
        OrangutanPushbuttons::waitForRelease(BUTTON_A);
      }      
      //-----------------------------------------------
      if (millis() % 2000 < 1000)
      {
        OrangutanLCD::clear();
        if(mode==1)OrangutanLCD::print("Mode A");
        else if(mode==2)OrangutanLCD::print("Mode B");
        OrangutanLCD::gotoXY(0, 1);
        OrangutanLCD::print("Press B");
      }
      else
        display_path();
      delay(30);
    }
    while (OrangutanPushbuttons::isPressed(BUTTON_B));

    delay(1000);
    Stopwatch=millis();//計時開始
    int i;

    if(mode==1)//慢速正常
    {
      for (i = 0; i < path_length; i++)
      {
        startTime=millis();
        follow_segment();
        T[i]=(millis()-startTime)*AtoB-80;
        run_turn(path[i]);
      }
      startTime=millis();
      follow_segment();
      T[41]=(millis()-startTime)*AtoB;
    }
    else if(mode==2)//直接衝刺
    {
      for (i = 0; i < path_length; i++)
      {        
        startTime=millis();
        maximum=255;
        follow_segment2(i);
        follow_segment();
        run_turn(path[i]);    
      }
      startTime=millis();
      maximum=255;
      follow_segment2(41);
      follow_segment();
    }    
      

    OrangutanMotors::setSpeeds(150, 150);
    delay(100);
     OrangutanMotors::setSpeeds(0, 0);
     OrangutanLCD::clear();
     OrangutanLCD::print(millis()-Stopwatch);
     OrangutanLCD::print("ms");
//     delay(2000);
//     OrangutanMotors::setSpeeds(-150, -150);
//     delay(3500);
//     OrangutanMotors::setSpeeds(0, 0);
     while (!OrangutanPushbuttons::isPressed(BUTTON_B));
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
  }
}


