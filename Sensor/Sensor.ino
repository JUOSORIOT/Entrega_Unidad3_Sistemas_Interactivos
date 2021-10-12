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



void taskCom() {
  unsigned char ab[1]={0x38};
  static uint8_t numero_1 = 0.0f;
  
  if(Serial.available())
  {
    Serial.read();

    Serial.write(uiCrc16Cal(ab,1));
  }
}
void setup() {
  Serial.begin(115200);
}



void loop() {
  taskCom();
}
