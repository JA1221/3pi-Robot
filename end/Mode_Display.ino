//********************Mode顯示在LCD*********************
void Mode_Display()
{
  OrangutanLCD::clear();
  if(mode==1)OrangutanLCD::print("Mode A");
  else if(mode==2)OrangutanLCD::print("Mode B");
  else if(mode==3)OrangutanLCD::print("Mode C");
  else if(mode==4)OrangutanLCD::print("Mode D");
  else if(mode==5)
  {
    if(mode2==1)
    {
      OrangutanLCD::print("SP:");
      OrangutanLCD::print(modeAspeed);
    }
    else if(mode2==2)
    {
      OrangutanLCD::print("st:");
      OrangutanLCD::print(st);
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
   else if(mode2==4)
   {
    
   }
   }
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print("Press B");
    delay(30);
}

