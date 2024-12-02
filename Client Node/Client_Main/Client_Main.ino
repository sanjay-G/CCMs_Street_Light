#define lightOnOff 13
#define lightPwm   23
#define voltageSesnorPin 26
#define currentSensorPin 27

void setup() {
  Serial.begin(115200);
  pinMode(lightOnOff,OUTPUT);
  pinMode(lightPwm,OUTPUT);

}

void loop() {

 LightOnOff("ON"); //ON/OFF
 lightIntensity(10); // Percentage of light Intensity
 Voltage(); //Voltage in V
 Current(); //Current in mA
 

}


float Current(){
  int sensitivity = 66;
  int adcValue= 0;
  int offsetVoltage = 2500;
  double adcVoltage = 0;
  double currentValue = 0;
  adcValue = analogRead(currentSensorPin);
  adcVoltage = (adcValue / 1024.0) * 5200;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
  return currentValue;
}

float Voltage(){
    float adc_voltage = 0.0;
    float in_voltage = 0.0;
    float R1 = 30000.0;
    float R2 = 75000.0;
    float ref_voltage = 5.0;
    int adc_value = 0;

    Serial.println("Getting Voltage Sensor Data");
    adc_value = analogRead(voltageSesnorPin);
    adc_voltage  = (adc_value * ref_voltage) / 1024.0;
    in_voltage = ((adc_voltage/ (R2/(R1+R2)))-7.0);
    return in_voltage;
}

void lightIntensity(int percentage){
  float lightIntensity = map(percentage,0,100,0,255);
  analogWrite(lightPwm,lightIntensity);
}

void LightOnOff(String Input){
 if(Input=="ON"){
   digitalWrite(lightOnOff,HIGH);
 }
 else{
   digitalWrite(lightOnOff,LOW);
 }
}
