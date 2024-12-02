#include "painlessMesh.h"
#include <Arduino_JSON.h>
#define lightOnOff 13
#define lightPwm   23
#define voltageSesnorPin 26
#define currentSensorPin 27
#define   MESH_PREFIX     "TeamStreetLight"
#define   MESH_PASSWORD   "Forged@123"
#define   MESH_PORT        5555
painlessMesh  mesh;
Scheduler userScheduler; // to control your personal task
int ThisNodeID = 1;

struct dataReceived 
{
  int NodeNumber;
  String VoltageData;
  String CurrentData;
  String  OnOffData;
  int LightOnOffNode;
  int GatewayRequest;
  int brightnessData;
};
dataReceived MeshData = {0,"0","0","ON",0,0,0};
String LightOnOffVerify = "ON";
unsigned long lastRequestTime = 0; // Tracks the last time a request was processed
const unsigned long resetInterval = 500; 

void setup() {
  Serial.begin(115200);
  pinMode(lightOnOff,OUTPUT);
  pinMode(lightPwm,OUTPUT);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);

}

void loop() {
 mesh.update();

//  Voltage(); //Voltage in V
//  Current(); //Current in mA
 if(MeshData.LightOnOffNode == ThisNodeID && MeshData.OnOffData == "ON" ){  //checking weather the command for This node else this loop will ignore the command
   LightOnOffVerify = "ON";}
 else if(MeshData.LightOnOffNode == ThisNodeID && MeshData.OnOffData == "OFF"){
   LightOnOffVerify = "OFF";
 }
  
 if (MeshData.GatewayRequest ==ThisNodeID && millis() - lastRequestTime > resetInterval ){
   SendMessage(broadcastData (ThisNodeID, String(Voltage()),String(Current())));
   lastRequestTime = millis();
   MeshData.GatewayRequest = 0;
   }

LightOnOff(LightOnOffVerify); //ON/OFF
lightIntensity(MeshData.brightnessData); // Percentage of light Intensity
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

String broadcastData (int NodeID,String Volt,String mAmps) {  //Format for sending the data to broadcast
  JSONVar jsonReadings;
  jsonReadings["Node"] = ThisNodeID;
  jsonReadings["Volatge"] = Volt;
  jsonReadings["Current"] = mAmps;
  String readings = JSON.stringify(jsonReadings);
  return readings;
}

int getNodeID()
{
 int NodeId = mesh.getNodeId();
 return NodeId;
}

void SendMessage(String Message)
{
  mesh.sendBroadcast(Message);
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse(msg.c_str());
  int node = myObject["Node"];
  String voltage = myObject["Volatge"];
  String current = myObject["Current"];
  String lightOnOffCommand = myObject["LightStatus"];
  int lightOnOffNode = myObject["LightOnOffNode"];
  int RequestFromMaster = myObject["RequestNode"];
  int LightBrightness = myObject["BrigthnessPercentage"];
  MeshData.NodeNumber = node;
  MeshData.VoltageData =  voltage;
  MeshData.CurrentData =  current;
  MeshData.OnOffData =  lightOnOffCommand;
  MeshData.LightOnOffNode =  lightOnOffNode;
  MeshData.GatewayRequest =  RequestFromMaster;
  MeshData.brightnessData =  LightBrightness;
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
