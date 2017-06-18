//********************迷宮跑完後************************
// N=>
void finish()
{  
   OrangutanMotors::setSpeeds(150, 150);
   delay(40);
   OrangutanMotors::setSpeeds(0, 0);
   t=millis()-Stopwatch;
   OrangutanLCD::clear();   
   OrangutanLCD::print(t);
   OrangutanLCD::print("ms");
   OrangutanLCD::gotoXY(0, 1);
   if(mode==1)OrangutanLCD::print("   OK   ");
   else if(mode==2)OrangutanLCD::print("N  OK  S");
   else if(mode==3)OrangutanLCD::print("   OK  S");     
   Serial.print(t);
   Serial.println("ms");
   
   if(mode==1)//-------------------Mode A---------------------
   {
     while (!OrangutanPushbuttons::isPressed(BUTTON_B));
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
   }
   else if(mode==2)//-------------------Mode B------------
   {//A不更新校正秒數 B更新校正秒數 C存入當次衝刺秒數至EEEPROM(儲存後會問部否更新校正後的衝刺秒數)
    while(1)
    {
      if(OrangutanPushbuttons::isPressed(BUTTON_A))
      {        
        OrangutanPushbuttons::waitForRelease(BUTTON_A);
        break;
      }
      else if(OrangutanPushbuttons::isPressed(BUTTON_B))
      {
        for(byte i=0;i<42;i++)T[i]=T2[i];
        OrangutanPushbuttons::waitForRelease(BUTTON_B);
        break;
      }
      else if(OrangutanPushbuttons::isPressed(BUTTON_C))
      {        
        for(byte i=0;i<42;i++)
        {
          int ROM=T[i]/10;
          EEPROM.write(i,ROM);
        }
        OrangutanLCD::clear();
        OrangutanLCD::print(" Saved! ");
        delay(2000);
        OrangutanLCD::clear();
        OrangutanLCD::print("U Fixes?");
        OrangutanLCD::gotoXY(0, 1);
        OrangutanLCD::print("Y      N");
        while(1)
        {
          if(OrangutanPushbuttons::isPressed(BUTTON_A))
          {
            for(byte i=0;i<42;i++)T[i]=T2[i];            
            OrangutanPushbuttons::waitForRelease(BUTTON_A);
            break;
          }
          if(OrangutanPushbuttons::isPressed(BUTTON_C))
          {
            OrangutanPushbuttons::waitForRelease(BUTTON_C);
            break;
          }
        }
        break;
      }
    }
   }
   else if(mode==3)//-------------------Mode C---------------------
   {//按C存入衝刺秒數至EEEPROM
    while(1)
    {
      if(OrangutanPushbuttons::isPressed(BUTTON_C))
      {        
         for(byte i=0;i<42;i++)
        {
          int ROM=T[i]/10;
          EEPROM.write(i,ROM);
        }
        break;
      }
      else if(OrangutanPushbuttons::isPressed(BUTTON_B))break;
    }
   }
}

