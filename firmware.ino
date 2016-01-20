// -----------------
// Read temperature from multiple sensors and make the readings available to a google spread sheet
// Lutz Lab UW 2015 www.lutzlab.com
// -----------------

//int numReadings;
int NumSensors =4;
// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
double readings1[10];       //
double readings2[10];       //
double readings3[10];       //
double readings4[10]; 

//Variables associated with calculations
int j[4] = {0,0,0,0};                  // the index of the current reading; same for both
double total[4] = {0.0,0.0,0.0,0.0};                  // the running total
double TempAverage[4] = {0.0,0.0,0.0,0.0};
double tempSample[4] = {0.0,0.0,0.0,0.0};
int rawreading[4] = {0,0,0,0}; 
double voltage[4] = {0.0,0.0,0.0,0.0};


//Pin declarations
int TSensorPin[4] = {A0,A1,A2,A3};


//Spark variable JSON object
char resultstr[64];


//Prelimcalculations
int numReadings=arraySize(readings1); //or 2 both should be same

void setup()
{
  // Register a Spark variable here
  Particle.variable("tempstr", resultstr); 
  // Connect the temperature sensors to A7 and A6 and configure it
  // to be an input
   for (int k = 0; k < NumSensors; k++) {
  pinMode(TSensorPin[k], INPUT);}
  
  // initialize all the readings to 0:
  for (int i = 0; i < numReadings; i++)
     readings1[i] = 0;   
  
  for (int i = 0; i < numReadings; i++)
     readings2[i] = 0;   

  for (int i = 0; i < numReadings; i++)
     readings3[i] = 0;   
  
  for (int i = 0; i < numReadings; i++)
     readings4[i] = 0;   
}

void loop()
{
 
 for (int k = 0; k < NumSensors; k++) {

  // subtract the last reading:
  if (k==0) total[k]= total[k] - readings1[j[k]]; 
  if (k==1) total[k]= total[k] - readings2[j[k]]; 
  if (k==2) total[k]= total[k] - readings3[j[k]]; 
  if (k==3) total[k]= total[k] - readings4[j[k]]; 
  
  // read from the sensor: 
  rawreading[k] = analogRead(TSensorPin[k]);
  voltage[k] = (rawreading[k] * 3.3) / 4095;
  tempSample[k] = (voltage[k] - 0.5) * 100;
  if (k==0) readings1[j[k]] = tempSample[k];
  if (k==1) readings2[j[k]] = tempSample[k];
  if (k==2) readings3[j[k]] = tempSample[k];
  if (k==3) readings4[j[k]] = tempSample[k];
  
  // add the reading to the total:
  if (k==0) total[k]= total[k] + readings1[j[k]]; 
  if (k==1) total[k]= total[k] + readings2[j[k]];
  if (k==2) total[k]= total[k] + readings3[j[k]]; 
  if (k==3) total[k]= total[k] + readings4[j[k]];
  
  // advance to the next position in the array:  
  j[k] = j[k] + 1;                    
  // if we're at the end of the array...// ...wrap around to the beginning:
  if (j[k] >= numReadings) j[k] = 0;                           

  // calculate the average:
  TempAverage[k] = total[k] / numReadings;         
  //Pause between readings
  delay(50);
 }
  //for JSON 
  sprintf(resultstr, "{\"temperature1\":%f,\"temperature2\":%f,\"temperature3\":%f,\"temperature4\":%f}", TempAverage[0], TempAverage[1],TempAverage[2], TempAverage[3]); 
  
  delay(50);        // delay in between read cycles for stability

}
