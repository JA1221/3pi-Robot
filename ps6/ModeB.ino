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
    if(t>275)T2[i]+=(t-275)*0.93;
    startTime=millis();
    run_turn(path[i]);
    t=millis()-startTime;
    if(t>250)T2[i]-=(t-250);    
  }
   startTime=millis();
   maximum=modeBspeed;
   follow_segment2(41);
   startTime=millis();
   follow_segment();
   t=t=millis()-startTime;
   if(t>150)T2[41]+=(t-150)*0.9;
}

