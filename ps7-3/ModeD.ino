//***************************Mode D斜走
void ModeD()
{
   for (byte i = 0; i < path_length; i++)
  { 
    if(i==1 ||i==2 || i==4 || i==12 ||i==13 ||i==25 ||i==28 )
    {
      maximum=modeBspeed;    
      startTime=millis();
      follow_segment2(i);
      arc_turn(path[i]);
    }
    maximum=modeBspeed;        
    startTime=millis();
    follow_segment2(i);
    follow_segment();
    run_turn(path[i]);
  }
}

