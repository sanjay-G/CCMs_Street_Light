#include "painlessMesh.h"
#include <Arduino_JSON.h>
#include <Ticker.h>
#define   MESH_PREFIX     "TeamStreetLight"
#define   MESH_PASSWORD   "Forged@123"
#define   MESH_PORT        5555
painlessMesh  mesh;
Scheduler userScheduler; // to control your personal task
Ticker timer;
int count = 1;
String NewDataReceived = "NO";
struct dataReceived 
{
  int NodeNumber;
  String VoltageData;
  String CurrentData;
};
dataReceived MeshData = {0,"0","0"};


void setup() {
  Serial.begin(115200);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  timer.attach(5.0, fetchData);
  
}

void loop() {
//  if(MeshData.NodeNumber==0){  // use interuppt for the task for 1 min once 
//    SendMessage(broadcastData (2,"ON",2,5));
//    mesh.update();}
   

}

void fetchData() {
       
        Serial.println("Iteruppt Fired");
        SendMessage(broadcastData(count, "ON", 0, 0)); 
        mesh.update();
        if(NewDataReceived=="YES"){
        Serial.println("New Data Received");
        //Passing the data to google sheets or UART
        count = count+1;
       }
       NewDataReceived = "NO";
       if(count==4){
        count=0;
       }
       
    
}

String broadcastData (int DataRequestNodeID ,String LightOnOffStatus,int LightNode , int BrightnessLevel) {  //Format for sending the data to broadcast
  JSONVar jsonReadings;
  jsonReadings["RequestNode"] = DataRequestNodeID;
  jsonReadings["LightStatus"] = LightOnOffStatus;
  jsonReadings["LightOnOffNode"] = LightNode;
  jsonReadings["BrigthnessPercentage"] = BrightnessLevel;
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
  NewDataReceived = "YES";
  JSONVar myObject = JSON.parse(msg.c_str());
  int node = myObject["Node"];
  String voltage = myObject["Volatge"];
  String current = myObject["Current"];
  MeshData.NodeNumber = node;
  MeshData.VoltageData =  voltage;
  MeshData.CurrentData =  current;
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
