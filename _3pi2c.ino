 
#include <Wire.h> 
 #include <I2C_Anything.h>
float amps,acs,vrms,arms,watt,watt1,realwatt, voltage;
float energydc=0;
float energyac=0;float energydc1=0;

    long lastMillis = 0;
long loops = 0;
long time1, lastConnectionTime;
int lan;
volatile double t[6], x[6], y[6],z[6];
#define PAYLOAD_SIZE 32 // how many bytes to expect from each I2C salve node
#define NODE_MAX 6 // maximum number of slave nodes (I2C addresses) to probe
#define START_NODE 2 // The starting I2C address of slave nodes
#define NODE_READ_DELAY 1000 // Some delay between I2C node reads
 union T {byte b[32]; double d;} T;
    union X {byte b[32]; double d;} X;
    union Y {byte b[32]; double d;} Y;
  union Z {byte b[32]; double d;} Z;
  const byte SLAVE_ADDRESS = 42;

void setup()
{//
  Serial.begin(9600);  
   
  Serial.println("MASTER READER NODE");
  Serial.print("Maximum Slave Nodes: ");
  Serial.println(NODE_MAX);
  Serial.print("Payload size: ");
  Serial.println(PAYLOAD_SIZE);
  Serial.println("***********************");
  
  Wire.begin();  
   delay(300);
}
void(* resetFunc) (void) = 0;
void loop()
{// RTC.readClock();
  // RTC.getFormatted(formatted);
  for (int nodeAddress = START_NODE; nodeAddress <= NODE_MAX; nodeAddress++)
  { // we are starting from Node address 2
    Wire.requestFrom(nodeAddress, PAYLOAD_SIZE);    // request data from node#
     if (Wire.requestFrom(nodeAddress, PAYLOAD_SIZE) == PAYLOAD_SIZE)
      {
        for (byte i = 0; i < 32; i++)
        {
          T.b[i] = Wire.read();
          X.b[i] = Wire.read();
          Y.b[i] = Wire.read();
            Z.b[i] = Wire.read();
        }
        t[nodeAddress-1] = T.d;
        x[nodeAddress-1] = X.d;
        y[nodeAddress-1] = Y.d;
        z[nodeAddress-1] = Z.d;
     
      Serial.print(nodeAddress-1); 
      Serial.print("   ");
        Serial.print(t[nodeAddress-1]); 
        Serial.print(","); Serial.print(x[nodeAddress-1]); 
        Serial.print(",");Serial.print(y[nodeAddress-1]);
         Serial.print(",");Serial.println(z[nodeAddress-1]);
     
      }
      else
      {
        while (Wire.available()) {byte del = Wire.read();} // Delete any data on wire.
      } 
    }
   acs=z[2];// A do qua acs
  Serial.print(acs);Serial.print(" Adc  ");
   lan=x[3]; // so lan nhay
   Serial.print(lan);Serial.print(" lan  ");
   voltage=z[1]; // Dien ap DC
    Serial.print(voltage);Serial.print("V dc   ");
   amps=t[1]; // DOng DC qua kep
   Serial.print(amps);Serial.print(" A Dc - kep   ");
   realwatt=x[1];
    Serial.print(realwatt);Serial.print(" W ac   ");
   arms=y[1];
    Serial.print(arms);Serial.print(" A Ac  ");
   watt=voltage*acs;
   watt1=voltage*amps;
    Serial.print(watt);Serial.println(" W dc  ");
long currentMillis = millis();
  loops++;
  
  // Serial.println(formatted);    
   time1= (currentMillis - lastMillis )/1000;
    
    lastMillis = currentMillis;
    loops = 0;
   {energydc=energydc+(watt*time1/3600000);}//kwh
{energyac=energyac+(realwatt*time1/3600000);}//kwh
{energydc1=energydc1+(watt*time1/3600000);}//kwh
     Wire.beginTransmission (SLAVE_ADDRESS);
  I2C_writeAnything (realwatt);
  I2C_writeAnything (lan);
   I2C_writeAnything (energydc1);
   
    I2C_writeAnything (voltage);
     I2C_writeAnything (acs);
      I2C_writeAnything (amps);
    
          I2C_writeAnything (energydc);
          I2C_writeAnything (energyac);
         //  I2C_writeAnything (energyhq);
  Wire.endTransmission ();
    delay(1000);
}
