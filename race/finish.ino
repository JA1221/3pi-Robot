//********************迷宮跑完後************************
void finish()
{  
   OrangutanMotors::setSpeeds(255, 255);
   delay(30);
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
     for (byte i = 0; i <=path_length; i++)
    {
     T2[i]=T[i]; 
    }
     while (!OrangutanPushbuttons::isPressed(BUTTON_B));
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
   }
   else if(mode==2)//-------------------Mode B---------------------
   {//按C存入衝刺秒數至EEEPROM
    for (byte i = 0; i < path_length; i++)
    {
      if(i==6||i==7||i==8||i==9||i==10||i==11||i==15||i==16||i==17||i==18||i==19||i==20||i==33||i==34||i==35||i==36||i==37||i==38)
      {
        T[i]=140;
      }
    }   
    
    while(1)
    {
      if(OrangutanPushbuttons::isPressed(BUTTON_B))                //OK結束
      {
        OrangutanPushbuttons::waitForRelease(BUTTON_B);
        break;
      }
    }
   }
   else if(mode==3)//----------------------Mod C--------------------------------
   {
    for (byte i = 0; i <= path_length; i++)
    {
      T[i]=T2[i];
    }
    for (byte i = 0; i < path_length; i++)
    {
      if(i==1||i==6||i==7||i==8||i==9||i==10||i==11||i==15||i==16||i==17||i==18||i==19||i==20||i==33||i==34||i==35||i==36||i==37||i==38)
      {
        T[i]=140;
      }
    }   
     while (!OrangutanPushbuttons::isPressed(BUTTON_B));
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
   }
   else if(mode==4)//----------------------Mode D-------------------------------
   {
     while (!OrangutanPushbuttons::isPressed(BUTTON_B));
     OrangutanPushbuttons::waitForRelease(BUTTON_B);
   }
}

