#include "LSM6DS3.h"
#include "Wire.h"

//Create a instance of class LSM6DS3
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

// Constant variables
const float scaleA = 0.061; // THESE ARE DIFFERENT FOR THIS BOARD MAYBE
const float scaleG = 4.375;
const float scaleM = 6842.0;
const float delta = 0.2;

// Sensor variables
float dt;
float ax, ay, az;
float ax0, ay0, az0;
float wx, wy, wz;
float mx, my, mz;
float magGlobalX, magGlobalY, magGlobalZ;
float magOffset[3], magGain[3];

// Angle Variables
int a, b, drift;
int roll, pitch, yaw;
int roll0, pitch0, yaw0;
float phiDot, thetaDot, psiDot;
float phi, theta, psi;
float phi_0, theta_0, psi_0;
float psiOffset;

// Matrices and Position Variables
float cPhi, sPhi, cTh, sTh, cPsi, sPsi;
float R[3][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0}
};
float T[3][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0}
};
float W[3][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0}
};
int A[3][1] = {
  {0},
  {0},
  {0}
};
float aT[3][1] = {
  {0},
  {0},
  {0}
};
float aG[3][1] = {
  {0},
  {0},
  {0}
};
float aCentrp[3][1] = {
  {0},
  {0},
  {0}
};
int a0[3][1] = {
  {0},
  {0},
  {0}
};
int v0[3][1] = {
  {0},
  {0},
  {0}
};
int x0[3][1] = {
  {0},
  {0},
  {0}
};
int s[3][1] = {
  {0},
  {0},
  {0}
};
int zero[3][1] = {
  {0},
  {0},
  {0}
};

int pop[30];
int avg = 0;

// delta variables
int counter = 0;
unsigned long oldTime = 0;

//________Gyro Offset Variables___________//
float gyro_pitch_cal = 0;
float gyro_roll_cal = 0;
float gyro_yaw_cal = 0;
float gyro_pitch = 0;
float gyro_roll = 0;
float gyro_yaw = 0;
int calib_cnt = 100;

//LP filter variables
const float alpha = 0.5; //bigger = not enough filtering, lower = too much filtering
double filtered_data[6] = {0, 0, 0, 0, 0, 0};
double data[3] = {0, 0, 0};


void setup() 
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial);
    //Call .begin() to configure the IMUs
    if (myIMU.begin() != 0) {
        Serial.println("Device error");
    } else {
        Serial.println("Device OK!");
    }
    
    phi = 0;
    theta = 0;
    psi = 0;
  
    phi_0 = 0;
    theta_0 = 0;
    psi_0 = 0;
  
    psiOffset = 0;

    //_____________Calib Mag_________________//
    magOffset[0] = -15845; //mxMin
    magOffset[1] = 4232;   //myMin
    magOffset[2] = -4223;  //mzMin
  
    magGain[0] = (-6822 - -15845);  //mxMax - mxMin
    magGain[1] = (5568 - 4232);     //myMax - myMin
    magGain[2] = (13157 - -4223);   //mzMax - mzMin
  
  //_____________Calib Gyro_________________//
    for(int i = 0; i < calib_cnt; i++)
    {
      //myIMU.readFloatGyroX();
      gyro_pitch_cal += myIMU.readFloatGyroX();;                              //Add pitch value to gyro_pitch_cal.
      gyro_roll_cal  += myIMU.readFloatGyroY();;                               //Add roll value to gyro_roll_cal.
      gyro_yaw_cal   += myIMU.readFloatGyroZ();;                                //Add yaw value to gyro_yaw_cal.
      delay(100); 
    }
  
    gyro_pitch_cal /= calib_cnt;
    gyro_roll_cal  /= calib_cnt;
    gyro_yaw_cal   /= calib_cnt;
  
    Serial.print("Gyro calib: ");
    Serial.print(gyro_roll_cal);
    Serial.print(" ");
    Serial.print(gyro_pitch_cal);
    Serial.print(" ");
    Serial.println(gyro_yaw_cal);
    delay(200);
}

void loop() 
{
  //gyroAcc.read();
  //mag.read();

  dt = float(CalculateDeltaTime()) / 1000.0;
  // Serial.println(dt, 3); // debugging delta

//________Gyro data and conversion_______//
  wx = myIMU.readFloatGyroX();
  wy = myIMU.readFloatGyroY();
  wz = myIMU.readFloatGyroZ();

  wy -= gyro_pitch_cal; // apply calibration
  wx -= gyro_roll_cal;
  wz -= gyro_yaw_cal; 

  wy *= M_PI/180.0; // scaleG / 1000.0 * // convert to deg/s then rads
  wx *= M_PI/180.0; // scaleG / 1000.0 * 
  wz *= M_PI/180.0; // scaleG / 1000.0 *

//    Serial.print("Gyro: ");
//    Serial.print(wx);
//    Serial.print(" ");
//    Serial.print(wy);
//    Serial.print(" ");
//    Serial.println(wz);
  
//________________Acc data (cm/s^2)_______________________//
  ax = myIMU.readFloatAccelX() * scaleA / 100.0;
  ay = myIMU.readFloatAccelY() * scaleA / 100.0;
  az = myIMU.readFloatAccelZ() * scaleA / 100.0;

//  data[0] = ax;
//  data[1] = ay;
//  data[2] = az; 
//
//  filtered_data[3] = alpha * data[0] + (1 - alpha) * filtered_data[0];
//  filtered_data[4] = alpha * data[1] + (1 - alpha) * filtered_data[1];
//  filtered_data[5] = alpha * data[2] + (1 - alpha) * filtered_data[2];
//
//  filtered_data[0] = filtered_data[3];
//  filtered_data[1] = filtered_data[4];
//  filtered_data[2] = filtered_data[5];
//
//  ax = filtered_data[3];
//  ay = filtered_data[4];
//  az = filtered_data[5];

  //  Serial.print("Acc: ");
  //  Serial.print(ax);
  //  Serial.print(" ");
  //  Serial.print(ay);
  //  Serial.print(" ");
  //  Serial.println(az);
  
//________________Mag data (uT)__________________________//

//  mx = ((mag.m.x  - magOffset[0]) / magGain[0]) / scaleM * -1;
//  my = ((mag.m.y  - magOffset[1]) / magGain[1]) / scaleM * -1;
//  mz = ((mag.m.z  - magOffset[2]) / magGain[2]) / scaleM * -1;

  //  Serial.print("Mag: ");
  //  Serial.print(mx);
  //  Serial.print(" ");
  //  Serial.print(my);
  //  Serial.print(" ");
  //  Serial.println(mz);

  // Gyro angular velocity
  phiDot = (wx + (wz*cos(phi) + wy*sin(phi))*tan(theta));
  thetaDot = (wy*cos(phi) - wz*sin(phi));
  psiDot = ((wz*cos(phi) + wy*sin(phi))/cos(theta));

  // Gyro numerical integration
  phi_0 = phiDot*dt + phi_0;
  theta_0 = thetaDot*dt + theta_0;
  psi_0 = psiDot*delta + psi_0;

  // Drift compensation
  // a = atan2(mz, mx);
  // b = atan2(magGlobalZ, magGlobalX);
  // drift = (a-b) * 180.0 / M_PI;
  
  // Accel and Mag angles
  phi = (atan2(-ay, -az));                                              // converting to degs
  theta = (atan2(ax, sqrt(ay*ay + az*az)));
//  psi = ((atan2(mx*cos(theta) + my*sin(theta)*sin(phi) + mz*sin(theta)*cos(phi), my*cos(phi) - mz*sin(phi))));
  
  // mx = mag.m.x * cos(theta) - mag.m.y * sin(phi) * sin(theta) + mag.m.z * cos(phi) * sin(theta); // second way of calculating psi
  // my = mag.m.y * cos(phi) + mag.m.z * sin(phi);
  // psi = atan2(my, mx);
  // Serial.println(psi);

  // Complementary Filtering 
  roll = (0.02*phi_0 + 0.98*phi) * 180.0/M_PI;
  pitch = (0.02*theta_0 + 0.98*theta) * 180.0/M_PI;
  //yaw = (0.98*psi_0 + 0.02*psi) * 180.0/M_PI;
  yaw = psi_0 * 180.0/M_PI;

  
  if(counter < 1) // Setting ICs
  {
    roll0 = roll;
    pitch0 = pitch;
    yaw0  = yaw;
    counter = 1000;
    ax0 = ax * -1;
    ay0 = ay * -1;
    az0 = az * -1;
    psi_0 = psi;
    
  }

  roll = roll - roll0; //x
  pitch = pitch - pitch0; //y
  yaw = yaw - yaw0; //z

//_________________________Print Statements___________________________//

  Serial.print("Angles: ");
  Serial.print(roll);
  Serial.print(" ");
  Serial.print(pitch);
  Serial.print(" ");
  Serial.println(yaw);

  delay(100);
}

//__________________Functions_____________________________//
void transform(float accX, float accY, float accZ, float matrix[3][3], String acc )
{
  if(acc == "linear"){
  aT[0][0] = matrix[0][0]*accX + matrix[0][1]*accY + matrix[0][2]*accZ;
  aT[1][0] = matrix[1][0]*accX + matrix[1][1]*accY + matrix[1][2]*accZ;
  aT[2][0] = matrix[2][0]*accX + matrix[2][1]*accY + matrix[2][2]*accZ;
  }

  if(acc == "gravity"){
  aG[0][0] = matrix[0][0]*accX + matrix[0][1]*accY + matrix[0][2]*accZ;
  aG[1][0] = matrix[1][0]*accX + matrix[1][1]*accY + matrix[1][2]*accZ;
  aG[2][0] = matrix[2][0]*accX + matrix[2][1]*accY + matrix[2][2]*accZ;
  }

  if(acc == "centripetal"){
  aCentrp[0][0] = matrix[0][0]*accX + matrix[0][1]*accY + matrix[0][2]*accZ;
  aCentrp[1][0] = matrix[1][0]*accX + matrix[1][1]*accY + matrix[1][2]*accZ;
  aCentrp[2][0] = matrix[2][0]*accX + matrix[2][1]*accY + matrix[2][2]*accZ;
  }
  
  return;
}

void transpose(float matrix[3][3])
{

  T[0][0] = matrix[0][0];
  T[0][1] = matrix[1][0];
  T[0][2] = matrix[2][0];
  T[1][0] = matrix[0][1];
  T[1][1] = matrix[1][1];
  T[1][2] = matrix[2][1];
  T[2][0] = matrix[0][2];
  T[2][1] = matrix[1][2];
  T[2][2] = matrix[2][2];
  
  return;
}

void euler(int sDdot[3][1], int sdot[3][1], int s0[3][1])
{
  
  s[0][0] = 0.5 * sDdot[0][0] * dt*dt + sdot[0][0]*dt + s0[0][0];
  s[1][0] = 0.5 * sDdot[1][0] * dt*dt + sdot[1][0]*dt + s0[1][0];
  s[2][0] = 0.5 * sDdot[2][0] * dt*dt + sdot[2][0]*dt + s0[2][0];

//  s[0][0] = sdot[0][0]*dt + s0[0][0];
//  s[1][0] = sdot[1][0]*dt + s0[1][0];
//  s[2][0] = sdot[2][0]*dt + s0[2][0];
  
  return;
}

void simpsons()
{
  return;
}

void trapezoidal(int s1[3][1], int s0[3][1], int ic[3][1])
{

  s[0][0] = (s1[0][0] + s0[0][0]) * dt * 0.5 + ic[0][0];
  s[1][0] = (s1[1][0] + s0[1][0]) * dt * 0.5 + ic[1][0];
  s[2][0] = (s1[2][0] + s0[2][0]) * dt * 0.5 + ic[2][0];
  
  return;
}

void popAvg(int newData)
{

  for(int j = 3; j > 1; j--)
  {
    pop[j] = pop[j-1];
  }

  pop[0] = newData;
  
  for(int i = 0; i < 3; i++)
  {
    avg += pop[i];
  }

  avg = avg/3;
  
  return;
}

unsigned long CalculateDeltaTime()
{
  unsigned long currentTime = millis();
  unsigned long deltaTime = (currentTime - oldTime);
  oldTime = currentTime;
  return deltaTime;
}
