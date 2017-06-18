//************************Mode B(純衝刺)************************
//void ModeB()//比賽用
//{
//    for (byte i = 0; i < path_length; i++)
//  { 
//    maximum=modeBspeed;        
//    startTime=millis();
//    follow_segment2(i);
//    follow_segment();
//    run_turn(path[i]);
//  }
//  maximum=modeBspeed;
//  startTime=millis();
//  follow_segment2(path_length);
//  follow_segment();
//}



void ModeB()//藍芽測試用
{
    for (byte i = 0; i < path_length; i++)
  { 
    maximum=modeBspeed;        
    Serial.print(i+1);
    Serial.print(".");
    startTime=millis();
    follow_segment2(i);
    t=millis()-startTime;
    Serial.print(t);
    Serial.print(",");
    startTime=millis();     
    follow_segment();
    t=millis()-startTime;
    Serial.print(t);
    Serial.print(",");
    startTime=millis();
    run_turn(path[i]);
    t=millis()-startTime;
    Serial.print(t);
    Serial.println("");
  }
  maximum=modeBspeed;
  startTime=millis();
  follow_segment2(path_length);
  Serial.print("25.");
  t=millis()-startTime;
  Serial.print(t);
  Serial.print(",");
  startTime=millis();
  follow_segment();
  Serial.print(millis()-startTime);
  Serial.println("");
     for (byte i = 0; i < path_length; i++)
   {
    if(i==1 ||i==2 || i==5 || i==8 || i==9 || i==10 || i==11 || i==12 ||i==13||i==14||i==23 )
    T[i]=st;     
   }
}

