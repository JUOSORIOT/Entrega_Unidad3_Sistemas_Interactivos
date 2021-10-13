unsigned int uiCrc16Cal(unsigned char const *pucY, unsigned char ucX)
{
  const uint16_t PRESET_VALUE = 0xFFFF;
  const uint16_t POLYNOMIAL = 0x8408;
  unsigned char ucI, ucJ;
  unsigned short int uiCrcValue = PRESET_VALUE;
  for (ucI = 0; ucI < ucX; ucI++)
  {
    uiCrcValue = uiCrcValue ^ *(pucY + ucI);
    for (ucJ = 0; ucJ < 8; ucJ++)
    {
      if (uiCrcValue & 0x0001)
      {
        uiCrcValue = (uiCrcValue >> 1) ^ POLYNOMIAL;
      }
      else
      {
        uiCrcValue = (uiCrcValue >> 1);
      }
    }
  }
  return uiCrcValue;
}
float getFloat(byte packet[],byte i)
{
  union u_tag{
    byte bin[4];
    float num;
  } u;
  u.bin[0] = packet[i];
  u.bin[1] = packet[i+1];
  u.bin[2] = packet[i+2];
  u.bin[3] = packet[i+3];
  return u.num;
}
void taskCom() {
  static uint8_t state = 1;
  static float numero_1 = 0.0;
  static float numero_2 = 0.0;
  static float resultado = 0.0;
  static uint8_t arrayResultado[6] = {0};
  static uint8_t bufferRx[11] = {0};
  static uint8_t dataCounter = 0;
  static uint8_t arrayCheck[2] ={0};
  static uint8_t arrayCheckr[2] ={0};  

  switch (state) {
  case 1:
    while(Serial.available())
    {
      uint8_t dataRx = Serial.read();
       
      if (dataCounter >= 10) {
          bufferRx[dataCounter] = dataRx;
          dataCounter = 0;
          for (int i = 0; i < 11; i++) 
          {
            //Serial.write(bufferRx[i]);
          }
          //Serial.write(uiCrc16Cal(bufferRx,9));
          uint16_t checkp = uiCrc16Cal(bufferRx,9);
          memcpy(arrayCheck,(uint8_t *)&checkp,2);
          //Serial.write(arrayCheck,2);
          if(arrayCheck[0]==bufferRx[9] and arrayCheck[1]==bufferRx[10])
          {
            state = 2;            
          }
         
      }
      else
      {
          bufferRx[dataCounter] = dataRx;
          dataCounter++;              
      }     
    }
    break;
  case 2:
    numero_1 = getFloat(bufferRx, 0);
    numero_2 = getFloat(bufferRx, 4);    
    //Serial.println(numero_1);
    //Serial.println(numero_2);
    //Serial.println((char)bufferRx[8]);
    if(((char)bufferRx[8])== '+')
    {
      resultado = numero_1 + numero_2;
    }
    else if(((char)bufferRx[8])== '-')
    {
      resultado = numero_1 - numero_2;
    }
    else if(((char)bufferRx[8])== '*')
    {
      resultado = numero_1 * numero_2;
    }
    else if(((char)bufferRx[8])== '/')
    {
      resultado = numero_1 / numero_2;
    }

    //Serial.println(resultado);
    //Serial.println("Llegamos al estado 2, Felicidades");
    state = 3;
    break;
  case 3:
    memcpy(arrayResultado, (uint8_t *)&resultado,4);
    //arrayResultado[4] = uiCrc16Cal(arrayResultado,4);
    uint16_t checkr = uiCrc16Cal(arrayResultado,4);
    memcpy(arrayCheckr,(uint8_t *)&checkr,2);
    arrayResultado[4] = arrayCheckr[0];
    arrayResultado[5] = arrayCheckr[1];
    Serial.write(arrayResultado,6);
    state = 1;
    break;
  default:
    //Serial.print("Si estas aqui posees problemas");
    break;
  }  
}
void setup() {
  Serial.begin(115200);
}
void loop() {
  taskCom();
}
