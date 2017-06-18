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
   }
}

