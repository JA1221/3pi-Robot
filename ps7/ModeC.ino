//************************Mode C(純衝刺)************************
void ModeC()
{
  for (byte i = 0; i < path_length; i++)
  { 
    maximum=modeBspeed;        
    Serial.print(i+1);
    Serial.print(".");
    startTime=millis();
    follow_segment2(i);
    Serial.print(millis()-startTime);
    Serial.print(",");
    startTime=millis();     
    follow_segment();
    Serial.print(millis()-startTime);
    Serial.print(",");
    startTime=millis();
    run_turn(path[i]);
    Serial.print(millis()-startTime);
    Serial.println("");
  }
  maximum=modeBspeed;
  startTime=millis();
  follow_segment2(41);
  Serial.print("42.");
  Serial.print(millis()-startTime);
  Serial.print(",");
  startTime=millis();
  follow_segment();
  Serial.print(millis()-startTime);
  Serial.println("");
}

