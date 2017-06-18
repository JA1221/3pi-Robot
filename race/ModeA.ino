//*********************Mode A(安全模式+讀秒轉換加速時間)**************************
void ModeA()
{
  for (byte i = 0; i < path_length; i++)
  {    
    startTime=millis();
    follow_segment();
    t=millis()-startTime;
    if(fix>0)
    {
      t-=fix;
      fix=0;
    }
    T[i]=t*AtoB-as;
    run_turn(path[i]);
   }
   startTime=millis();
   follow_segment();
   t=millis()-startTime;
   T[path_length]=t*AtoB;    
}

