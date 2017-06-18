//********************Mode B(衝刺 修正衝刺時間)**********************
void ModeB()
{
  for (byte i = 0; i < path_length; i++)
  {        
    maximum=modeBspeed;
    follow_segment2(i);    
    startTime=millis();    
    follow_segment();
    t=millis()-startTime;
    if(t>80)T[i]+=(t-80)*0.8;
    startTime=millis();
    run_turn(path[i]);
  }
   maximum=modeBspeed;
   follow_segment2(41);
   startTime=millis();
   follow_segment();
   t=t=millis()-startTime;
   if(t>100)T[41]+=t;
}

