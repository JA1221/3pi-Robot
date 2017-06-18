//***************************Mode D斜走********************
void ModeC()
{
  for (byte i = 0; i < path_length; i++)
  {
    if(i==6 )
    {
      if(i==6)
      {
        maximum=130;
        startTime=millis();
        follow_segment2(42);     
        OrangutanMotors::setSpeeds(-150,150);
        delay(20);
        OrangutanMotors::setSpeeds(-90,90);
        do
        {
           unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
         } while (sensors[0]<800 ||sensors[1]<800 && sensors[2]<800 && sensors[3]<800 && sensors[4]<800);
         delay(sl);
        OrangutanMotors::setSpeeds(0,0);
        OrangutanBuzzer::play(">>a32");
        
        OrangutanMotors::setSpeeds(170,170);        
        while(i!=11)
        {          
          do
          {
            unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
          } while (sensors[4]>200);
                    
          OrangutanBuzzer::play(">>a32");
          if(i<10)delay(80);          
          i++;                   
        }
        delay(20);
        OrangutanMotors::setSpeeds(0,90);
        delay(10);
        do
        {
          unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
        } while (sensors[2]>200);
        maximum=60;
        follow_segment();
        run_turn(path[i]);
        i++;
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

