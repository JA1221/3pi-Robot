//********************Mode顯示在LCD*********************
void Mode_Display()
{
  OrangutanLCD::clear();
  if(mode==1)OrangutanLCD::print("Mode A");
  else if(mode==2)OrangutanLCD::print("Mode B");
  else if(mode==3)OrangutanLCD::print("Mode C");
  else if(mode==4)OrangutanLCD::print("Mode D");
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print("Press B");
    delay(30);
}

