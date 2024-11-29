void setup() {
  pinMode(13,OUTPUT);
  pinMode(23,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(13,HIGH);
  for(int i=0;i<=255;i++){
     analogWrite(23,i);
     delay(200);
  }
   for(int i=255;i<=0;i--){
     analogWrite(23,i);
     delay(200);
  }
 

}
