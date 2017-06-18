//***************************Mode D斜走********************
void ModeC()
{
  for (byte i = 0; i < path_length; i++)
  {
    if(i==6 ||i==34)
    {
      if(i==6)
      {
        OrangutanMotors::setSpeeds(50,50);
        delay(40);
        OrangutanMotors::setSpeeds(-50,50);
        delay(110);
        OrangutanMotors::setSpeeds(150,150);
//        delay(100);
        while(i!=11)
        {          
          do
          {
            unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
          } while (sensors[2]>200);
          OrangutanBuzzer::play(">>a32");
          if(i<11)delay(100);          
          i++;                        
        }
        OrangutanMotors::setSpeeds(-50,50);
        follow_segment();
        run_turn(path[i]);
      }
    }
    else
    {
      maximum=modeBspeed;        
      startTime=millis();
      follow_segment2(i);   
      follow_segment();
      run_turn(path[i]);
    }    
  }
}

