//----------------------------------------------------
void follow_segment()
{
  int last_proportional = 0;
  long integral=0;

  while(1)
  {
    unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    int proportional = ((int)position) - 2000;
    int derivative = proportional - last_proportional;
    integral += proportional;
    last_proportional = proportional;
    int power_difference = proportional/kp + integral/10000 + derivative*kd;
    if (power_difference > maximum)
      power_difference = maximum;
    if (power_difference < -maximum)
      power_difference = -maximum;

    if (power_difference < 0)
      OrangutanMotors::setSpeeds(maximum + power_difference, maximum);
    else
      OrangutanMotors::setSpeeds(maximum, maximum - power_difference);

    if (sensors[1] >800 && sensors[2] >800 && sensors[3] >800)
    {
      return;
    }
    else if (sensors[0] < 200 || sensors[4] <200)
    {
      // Found an intersection.
      return;
    }
  }
}
//-------------------------循跡(加速用)-----------------------------------
void follow_segment2(byte t)
{
  int last_proportional = 0;
  long integral=0;
  while(1)
  {
    unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    int proportional = ((int)position) - 2000;
    int derivative = proportional - last_proportional;
    integral += proportional;
    last_proportional = proportional;
    int power_difference = proportional/kp + integral/10000 + derivative*kd;
    if (power_difference > maximum)
      power_difference = maximum;
    if (power_difference < -maximum)
      power_difference = -maximum;

    if (power_difference < 0)
      OrangutanMotors::setSpeeds(maximum + power_difference, maximum);
    else
      OrangutanMotors::setSpeeds(maximum, maximum - power_difference);
    if(millis()-startTime>T[t])
    {
      if(t==0 || t==21 || t==22 || t==23 || t==24 || t==30 || t==31)//遇S不減速
      {
        return;
      }      
      else
      {
        OrangutanMotors::setSpeeds(modeAspeed,modeAspeed);        
        maximum=modeAspeed;
        return;
      }
    }
    if (sensors[1] >800 && sensors[2] >800 && sensors[3] >800)
    {
      return;
    }
    else if (sensors[0] < 200 || sensors[4] <200)
    {
      return;
    }
  }
}
