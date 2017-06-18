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
const char go[] PROGMEM = "L16 cdegreg4";
int maximum;
float kp=8;
float kd=4;
byte mode=1;
byte mode2;
unsigned long startTime;
unsigned long ST;unsigned long Stopwatch;
int modeAspeed=150;
float AtoB=0.38;
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
    while (!OrangutanPushbuttons::isPressed(BUTTON_B) || mode==4)
    {
      //---------------------------模式選擇---------------------------
      if(OrangutanPushbuttons::isPressed(BUTTON_A))
      {
        if(mode==1)
        {
          mode++;
          maximum=255;          
        }                
        else if(mode==2)
        {
          mode++;
          maximum = 255;//起始速度   
        }
        else if(mode==3)
        {
          mode=1;
          maximum=modeAspeed;        
        }
        else if(mode==4)
        {
          mode=mode2;
          mode2=0;
        }
        OrangutanBuzzer::play(">>a32");
        OrangutanPushbuttons::waitForRelease(BUTTON_A);
      } 
      else if(OrangutanPushbuttons::isPressed(BUTTON_C))
      {
        if(mode==1)mode2=1;
        else if(mode==2)mode2=2;
        else if(mode==3)mode2=3;
        mode=4;
        OrangutanBuzzer::play(">>a32");
        OrangutanPushbuttons::waitForRelease(BUTTON_C);
        if(mode2==1)
        {
          if(modeAspeed>=250)modeAspeed=120;
          else modeAspeed+=10;
        }
        else if(mode2==2)
        {
          if(AtoB>=0.55)AtoB=0.3;
          else  AtoB+=0.01;
        }
        else if(mode2==3)
        {
          for(byte i=0;i<42;i++)
          {
            T[i]=EEPROM.read(i)*10;
          }
        }       
      }
      //-----------------------------------------------------------------
        OrangutanLCD::clear();
        if(mode==1)OrangutanLCD::print("Mode A");
        else if(mode==2)OrangutanLCD::print("Mode B");
        else if(mode==3)OrangutanLCD::print("Mode C");
        else if(mode==4)
        {
          if(mode2==1)
          {
            OrangutanLCD::print("SP:");
            OrangutanLCD::print(modeAspeed);
          }
          else if(mode2==2)
          {
            OrangutanLCD::print("AB:x0.");
            int num=AtoB*100;
            OrangutanLCD::print(num);
          }
          else if(mode2==3)
          {
            OrangutanLCD::print("Read");
            OrangutanLCD::gotoXY(0, 1);
            OrangutanLCD::print("Success");
            delay(1000);
            mode=3;
            mode2=0;                      
          }
        }
        OrangutanLCD::gotoXY(0, 1);
        OrangutanLCD::print("Press B");
      delay(30);
      if(Serial.available())
      {
        char chr=Serial.read();
        if(chr=='T')
        {
         for(byte i=1;i<43;i++)
         {
          Serial.print(T[i-1]);
          if(i!=42)Serial.print(",");           
          if(i%10==0)Serial.println();          
         }
         Serial.println("");
        }        
      }
    }
    //---------------------------線迷宮開始------------------------------
    while (OrangutanPushbuttons::isPressed(BUTTON_B));

    delay(1000);
    Stopwatch=millis();//計時開始
    int i;
    //-----------------慢速正常----------------   
    if(mode==1)
    {
      for (i = 0; i < path_length; i++)
      {
        startTime=millis();
        follow_segment();
        T[i]=(millis()-startTime)*AtoB;
        run_turn(path[i]);
      }
      startTime=millis();
      follow_segment();
      T[41]=(millis()-startTime)*AtoB;
    }
    //-------------衝刺 修正衝刺時間---------------
    else if(mode==2)
    {
      for (i = 0; i < path_length; i++)
      {        
        maximum=255;
        startTime=millis();
        follow_segment2(i);    
        startTime=millis();    
        follow_segment();
        if((millis()-startTime)>250)
        {
          T[i]+=(millis()-startTime-250)*0.87;
        }
        startTime=millis();
        run_turn(path[i]);
        if((millis()-startTime)>300)
        {
          T[i]-=(millis()-startTime-300);
        }    
      }
      startTime=millis();
      maximum=255;
      follow_segment2(41);
      startTime=millis();
      follow_segment();
      if((millis()-startTime)>150)
      {
        T[i]+=(millis()-startTime-150)*0.87;
      }
    }
    //-------------------純衝刺------------------
    else if(mode==3)
    {      
      for (i = 0; i < path_length; i++)
      { 
        startTime=millis();
        maximum=255;        
        Serial.print(i+1);
        Serial.print(".");
        ST=millis();
        follow_segment2(i);
        Serial.print(millis()-ST);
        Serial.print(",");
        ST=millis();        
        follow_segment();
        Serial.print(millis()-ST);
        Serial.print(",");
        ST=millis(); 
        run_turn(path[i]);
        Serial.print(millis()-ST);
        Serial.println("");
      }
      startTime=millis();
      maximum=255;
      ST=millis();
      follow_segment2(41);
      Serial.print("42.");
      Serial.print(millis()-ST);
      Serial.print(",");
      ST=millis(); 
      follow_segment();
      Serial.print(millis()-ST);
      Serial.println("");          
    }
      

    OrangutanMotors::setSpeeds(150, 150);
    delay(80);
     OrangutanMotors::setSpeeds(0, 0);
     OrangutanLCD::clear();
     int Time=millis()-Stopwatch;
     OrangutanLCD::print(millis()-Stopwatch);
     OrangutanLCD::print("ms");
//     delay(2000);
//     OrangutanMotors::setSpeeds(-150, -150);
//     delay(3500);
//     OrangutanMotors::setSpeeds(0, 0);
     while (!OrangutanPushbuttons::isPressed(BUTTON_B))
     {
      if(OrangutanPushbuttons::isPressed(BUTTON_C))
      {
        for(byte i=0;i<42;i++)
        {
          int ROM=T[i]/10;
          EEPROM.write(i,ROM);
        }
        OrangutanLCD::gotoXY(0, 1);
        OrangutanLCD::print("Saved");        
      }
     }
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
  }
}


