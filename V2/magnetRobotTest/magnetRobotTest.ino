/*
The sensor outputs provided by the library are the raw 16-bit values
obtained by concatenating the 8-bit high and low magnetometer data registers.
They can be converted to units of gauss using the
conversion factors specified in the datasheet for your particular
device and full scale setting (gain).

Example: An LIS3MDL gives a magnetometer X axis reading of 1292 with its
default full scale setting of +/- 4 gauss. The GN specification
in the LIS3MDL datasheet (page 8) states a conversion factor of 6842
LSB/gauss (where LSB means least significant bit) at this FS setting, so the raw
reading of 1292 corresponds to 1292 / 6842 = 0.1888 gauss.
*/

#include <Wire.h>
#include <LIS3MDL.h>

LIS3MDL mag;

const int mAv = 20;

int sumX=0, sumY=0, sumZ=0;
int avgX=0, avgY=0, avgZ=0;
int mx[mAv] = {0, 0, 0, 0, 0, 0, 0};
int my[mAv] = {0, 0, 0, 0, 0, 0, 0};
int mz[mAv] = {0, 0, 0, 0, 0, 0, 0};
int movX, movY, movZ;
int magX, magY, magZ;

char report[80];

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  
  if (!mag.init())
  {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1);
  }

  
  mag.enableDefault();
  // change fsr +/- 16: doesn't affect much
  // mag.writeReg(LIS3MDL::CTRL_REG2, 0x60);
  // change operation mode
  //mag.writeReg(LIS3MDL::CTRL_REG1, 0x40);
  for (int i = 0; i < 1000; i++)
  {
    mag.read();
    sumX += mag.m.x; 
    sumY += mag.m.y; 
    sumZ += mag.m.z; 
  }
  avgX = sumX/1000; 
  avgY = sumY/1000; 
  avgZ = sumZ/1000; 
}

void loop()
{
  mag.read();

  // Moving average
  mx[19] = mx[18];
  mx[18] = mx[17];
  mx[17] = mx[16];
  mx[16] = mx[15];
  mx[15] = mx[14];
  mx[14] = mx[13];
  mx[13] = mx[12];
  mx[12] = mx[11];
  mx[11] = mx[10];
  mx[10] = mx[9];
  mx[9] = mx[8];
  mx[8] = mx[7];
  mx[7] = mx[6];
  mx[6] = mx[5];
  mx[5] = mx[4];
  mx[4] = mx[3];
  mx[3] = mx[2];
  mx[2] = mx[1];
  mx[1] = mx[0];
  mx[0] = mag.m.x - avgX;
  
  movX = 0;
  
  for(int j=0; j<mAv; j++)
  {
   movX += mx[j];
  }
  
  magX = movX/mAv;

  my[0] = mag.m.y - avgY;
  my[1] = my[0];
  my[2] = my[1];
  my[3] = my[2];
  my[4] = my[3];
  my[5] = my[4];
  my[6] = my[5];

  movY = 0;
  
  for(int k=0; k<7; k++)
  {
   movY += my[k];
  }
  
  magY = movY/7;

  mz[0] = mag.m.z - avgZ;
  mz[1] = mz[0];
  mz[2] = mz[1];
  mz[3] = mz[2];
  mz[4] = mz[3];
  mz[5] = mz[4];
  mz[6] = mz[5];

  movZ = 0;
  
  for(int l=0; l<7; l++)
  {
   movZ += mz[l];
  }
  
  magZ = movZ/7;
  
  snprintf(report, sizeof(report), "M: %6d %6d %6d",
    magX, magY, magZ);
  Serial.println(report);

  delay(100);
}
