//********************Mode顯示在LCD*********************
void Mode_Display()
{
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
}

