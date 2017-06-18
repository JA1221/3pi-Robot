//********************迷宮跑完後************************
void finish()
{  
   OrangutanMotors::setSpeeds(255, 255);
   delay(40);
   OrangutanMotors::setSpeeds(0, 0);
   t=millis()-Stopwatch;
   OrangutanLCD::clear();   
   OrangutanLCD::print(t);
   OrangutanLCD::print("ms");
   OrangutanLCD::gotoXY(0, 1);
   if(mode==1)OrangutanLCD::print("   OK   ");
   else if(mode==2)OrangutanLCD::print("   OK  S");
   else if(mode==3)OrangutanLCD::print("   OK   ");
   else if(mode==4)OrangutanLCD::print("   OK   ");     
   Serial.print(t);
   Serial.println("ms");
   
   if(mode==1)//-------------------Mode A---------------------
   {
     while (!OrangutanPushbuttons::isPressed(BUTTON_B))Bluetooth1();
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
   }
   else if(mode==2)//-------------------Mode B---------------------
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
        OrangutanLCD::gotoXY(0, 1);
        OrangutanLCD::print("Saved!!!");
        delay(1000);
        break;
      }
      else if(OrangutanPushbuttons::isPressed(BUTTON_B))                //OK結束
      {
        OrangutanPushbuttons::waitForRelease(BUTTON_B);
        break;
      }
    }
   }
   else if(mode==3)//----------------------Mod C--------------------------------
   {
     while (!OrangutanPushbuttons::isPressed(BUTTON_B))Bluetooth1();
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
   }
   else if(mode==4)//----------------------Mode D-------------------------------
   {
     while (!OrangutanPushbuttons::isPressed(BUTTON_B))Bluetooth1();
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
   }
}

