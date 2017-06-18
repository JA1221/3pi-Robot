//*************************模式選擇*************************
void Mode_Selection()
{
  if(OrangutanPushbuttons::isPressed(BUTTON_A))
      {
        if(mode==1)
        {
          mode++;
          maximum=modeBspeed;          
        }                
        else if(mode==2)
        {
          mode++;
          maximum = modeBspeed;//起始速度   
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



      
      else if(OrangutanPushbuttons::isPressed(BUTTON_C))      //功能鍵
      {
        if(mode==1)mode2=1;
        else if(mode==2)mode2=2;
        else if(mode==3)mode2=3;
        mode=4;
        OrangutanBuzzer::play(">>a32");
        OrangutanPushbuttons::waitForRelease(BUTTON_C);
        if(mode2==1)//MA調整速度
        {
          if(modeAspeed>255)modeAspeed=155;
          else 
          {
          modeAspeed+=10;
          }
          maximum=modeAspeed;
        }
        else if(mode2==2)//MB調正轉換率
        {
          if(AtoB>=0.7)AtoB=0.55;
          else  AtoB+=0.01;
        }
        else if(mode2==3)//MC讀取EEPROM先前儲存的衝刺秒數
        {
          for(byte i=0;i<42;i++)
          {
            T[i]=EEPROM.read(i)*10;
            T[i]+=EEPROM.read(i+42);
          }
        }       
      }
}

