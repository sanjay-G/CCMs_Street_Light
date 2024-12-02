#include "painlessMesh.h"
#include <Arduino_JSON.h>
#define   MESH_PREFIX     "TeamStreetLight"
#define   MESH_PASSWORD   "Forged@123"
#define   MESH_PORT        5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
int ThisNodeID = 2;

struct dataReceived 
{
  int NodeNumber;
  String VoltageData;
  String CurrentData;
  int  OnOffData;
};

dataReceived MeshData = {0,"0","0",0};

void setup() {
  Serial.begin(115200);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);

}

void loop() {
 
 mesh.update();
 SendMessage(broadcastData (ThisNodeID,"5.5","0.125"));
 Serial.println(MeshData.CurrentData);

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
  int lightOnOffCommand = myObject["LightStatus"];
  MeshData.NodeNumber = node;
  MeshData.VoltageData =  voltage;
  MeshData.CurrentData =  current;
  MeshData.OnOffData =  lightOnOffCommand;
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
