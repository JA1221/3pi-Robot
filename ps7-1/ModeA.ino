//*********************Mode A(安全模式+讀秒轉換加速時間)**************************
void ModeA()
{
  for (a = 0; a < path_length; a++)
  {    
    startTime=millis();
    follow_segment();
    t=millis()-startTime;
    if(fix>0)//修正S直走多耗的時間
    {
      t-=fix;
      t=0;
    }
    T[a]=t*AtoB-80;
    run_turn(path[a]);
    T2[a]=t;
   }
   startTime=millis();
   follow_segment();
   t=millis()-startTime;
   T[41]=t*AtoB;
   T2[41]=t;     
}

