//********************迷宮跑完後************************
void finish()
{  
   OrangutanMotors::setSpeeds(150, 150);
   delay(80);
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
     while (!OrangutanPushbuttons::isPressed(BUTTON_B))Bluetooth1();
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
   }
   else if(mode==2)//-------------------Mode B------------
   {//A不更新校正秒數 B更新校正秒數 C存入當次衝刺秒數至EEEPROM(儲存後會問部否更新校正後的衝刺秒數)
    while(1)
    {
      Bluetooth1();
      if(OrangutanPushbuttons::isPressed(BUTTON_A))                 //N 取消當次補償
      {        
        OrangutanPushbuttons::waitForRelease(BUTTON_A);
        break;
      }
      else if(OrangutanPushbuttons::isPressed(BUTTON_B))            //OK 補償更新存入T陣列
      {
        OrangutanPushbuttons::waitForRelease(BUTTON_B);
        break;
      }
      else if(OrangutanPushbuttons::isPressed(BUTTON_C))                //存入EEPROM
      {        
        for(byte i=0;i<42;i++)
        {
          int ROM=T[i]/10;
          byte ROM2=T[i]%10;
          EEPROM.write(i,ROM);
          EEPROM.write(i+42,ROM2);
        }
        OrangutanLCD::clear();
        OrangutanLCD::print(" Saved! ");
        delay(2000);
        OrangutanLCD::clear();
        OrangutanLCD::print("U Fixes?");                          //是否更新補償資料
        OrangutanLCD::gotoXY(0, 1);
        OrangutanLCD::print("Y      N");
        while(1)
        {
          if(OrangutanPushbuttons::isPressed(BUTTON_A))               //更新補償資料到T陣列
          {
            for(byte i=0;i<42;i++)T[i]=T2[i];            
            OrangutanPushbuttons::waitForRelease(BUTTON_A);
            break;
          }
          if(OrangutanPushbuttons::isPressed(BUTTON_C))                    //不更新當次補償
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
      Bluetooth1();
      if(OrangutanPushbuttons::isPressed(BUTTON_C))                 //存入EEPROM
      {        
         for(byte i=0;i<42;i++)
        {
          int ROM=T[i]/10;
          byte ROM2=T[i]%10;
          EEPROM.write(i,ROM);
          EEPROM.write(i+42,ROM2);
        }
        OrangutanPushbuttons::waitForRelease(BUTTON_C);
        break;
      }
      else if(OrangutanPushbuttons::isPressed(BUTTON_B))                //OK結束
      {
        OrangutanPushbuttons::waitForRelease(BUTTON_B);
        break;
      }
    }
   }
}

