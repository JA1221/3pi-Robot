//************************Mode B(純衝刺)************************
void ModeB()
{
    for (byte i = 0; i < path_length; i++)
  { 
    maximum=modeBspeed;        
    startTime=millis();
    follow_segment2(i);
    startTime=millis();
    follow_segment();
    t=millis()-startTime;
    if(t>150)T2[i]+=(t-150)*0.625;
    run_turn(path[i]);
  }
  maximum=modeBspeed;
  startTime=millis();
  follow_segment2(path_length);
  startTime=millis();
  follow_segment();
  t=millis()-startTime;
  if(t>150)T2[path_length]+=(t-150)*0.7;
}
