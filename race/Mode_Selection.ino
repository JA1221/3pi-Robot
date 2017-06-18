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
          mode++;
          maximum=modeBspeed;        
        }
        else if(mode==4)
        {
          mode=1;
          maximum=modeAspeed;
        }
        OrangutanBuzzer::play(">>a32");
        OrangutanPushbuttons::waitForRelease(BUTTON_A);
      }     
}

