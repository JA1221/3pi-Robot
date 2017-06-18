//**************************
void ModeC()
{
      for (byte i = 0; i < path_length; i++)
  { 
    maximum=modeBspeed;        
    startTime=millis();
    follow_segment2(i);
    follow_segment();
    run_turn(path[i]);
  }
  maximum=modeBspeed;
  startTime=millis();
  follow_segment2(path_length);
  follow_segment();
}

