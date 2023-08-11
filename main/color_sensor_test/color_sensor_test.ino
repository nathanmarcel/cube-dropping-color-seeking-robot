Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);
int red_var;
int green_var;
int blue_var;

int blue_top_thresh = 0;
int blue_bottom_thresh = 0;
int green_top_thresh = 0;
int green_bottom_thresh = 0;

char color_sensor_test() {

  char color = '';
  uint16_t lux = tcs.getRGB(&red_var, &green_var, &blue_var);

  if ((red_var > 240) && (blue_var < 10) && (green_var < 10)) {
    color = 'R';
  }
  else if ((blue_var > 240) && (red_var < 10) && (green_var < 10)) {
    color = 'B';
  }
  else if ((blue_var < blue_top_thresh) && (blue_var > blue_bottom_thresh) && (green_var < green_top_thresh) && (green_var > green_bottom_thresh)) {
    color = 'Y';
  }
  else {
    color = 'N';
  }

  return color;
  
}


void sendStartFrame() 
{
 SPI.transfer(B00000000);
 SPI.transfer(B00000000);
 SPI.transfer(B00000000);
 SPI.transfer(B00000000);
}

void sendEndFrame() 
{
 SPI.transfer(B11111111);
 SPI.transfer(B11111111);
 SPI.transfer(B11111111);
 SPI.transfer(B11111111); 
}

void sendColorFrame(byte brightness, byte B, byte G, byte R) 
{
  brightness = brightness | B11100000;
  SPI.transfer(brightness);
  SPI.transfer(B);
  SPI.transfer(G);
  SPI.transfer(R);
}
