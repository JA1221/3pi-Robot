// Code to perform various types of turns according to the parameter dir,
// which should be 'L' (left), 'R' (right), 'S' (straight), or 'B' (back).
// The delays here had to be calibrated for the 3pi's motors.
void turn(unsigned char dir)
{
  switch(dir)
  {
  case 'L':
    // Turn left. 
    OrangutanMotors::setSpeeds(-255, 255);
    delay(50);
    OrangutanMotors::setSpeeds(-100,100);
    do {
      unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    } while (sensors[2]>200);
    OrangutanMotors::setSpeeds(0,0);
    break;
  case 'R':
    // Turn right.
    OrangutanMotors::setSpeeds(255,-255);
    delay(50);
    OrangutanMotors::setSpeeds(100,-100);
    do {
      unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    } while (sensors[2]>200);
    OrangutanMotors::setSpeeds(0,0);
    break;
  case 'B':
    // Turn around.
    OrangutanMotors::setSpeeds(255,-255);
    delay(100);
    OrangutanMotors::setSpeeds(100,-100);
    do {
      unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    } while (sensors[2]>200);
    OrangutanMotors::setSpeeds(0,0);
    break;
  case 'S':
    // Don't do anything!
    break;
  }
}

// 衝刺用的轉彎（弧轉）
void run_turn(unsigned char dir)
{
  OrangutanBuzzer::play(">>a32");
  switch(dir)
  {
  case 'L':
    // Turn left. 
    OrangutanMotors::setSpeeds(-255, 255);
    delay(50);
    OrangutanMotors::setSpeeds(0,100);
    do {
      unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    } while (sensors[2]>200);
    OrangutanMotors::setSpeeds(0,0);
    break;
  case 'R':
    // Turn right.
    OrangutanMotors::setSpeeds(255,-255);
    delay(50);
    OrangutanMotors::setSpeeds(100,0);
    do {
      unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    } while (sensors[2]>200);
    OrangutanMotors::setSpeeds(0,0);
    break;
  case 'B':
    // Turn around.
    OrangutanMotors::setSpeeds(255,-255);
    delay(100);
    OrangutanMotors::setSpeeds(100,-100);
    do {
      unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON,1);
    } while (sensors[2]>200);
    OrangutanMotors::setSpeeds(0,0);
    break;
  case 'S':
    if(mode==1)
    {
      OrangutanMotors::setSpeeds(modeAspeed,modeAspeed);
      delay(50);
    }
    else if(mode==2 || mode==3)
    {
      OrangutanMotors::setSpeeds(255,255);
      delay(30);
    }    
    break;
  }
}

