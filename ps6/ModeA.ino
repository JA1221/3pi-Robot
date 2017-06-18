//*********************Mode A(安全模式+讀秒轉換加速時間)**************************
void ModeA()
{
  for (byte i = 0; i < path_length; i++)
  {    
    startTime=millis();
    follow_segment();
    t=millis()-startTime;
    if(t>200)T[i]=(t-200)*AtoB;
    run_turn(path[i]);
   }
   startTime=millis();
   follow_segment();
   t=millis()-startTime;
   T[41]=t*AtoB;        
}

