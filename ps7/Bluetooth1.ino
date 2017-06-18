//A t=>MA速度秒數 T=轉換後秒數
//B T=>MB當次用的衝刺秒數 t=>校正後的秒數
//C T=>MC 當次衝刺的秒數
void Bluetooth1()
{
  if(Serial.available())
  {
    char chr=Serial.read();
    if(chr=='T')
    {
      for(byte i=1;i<43;i++)
      {
        Serial.print(T[i-1]);
        if(i!=42)Serial.print(",");           
        if(i%10==0)Serial.println();          
       }
        Serial.println("");
     }
     else if(chr=='t')
     {
      for(byte i=1;i<43;i++)
      {
        Serial.print(T2[i-1]);
        if(i!=42)Serial.print(",");           
        if(i%10==0)Serial.println();          
       }
        Serial.println("");
     }        
   }
}

