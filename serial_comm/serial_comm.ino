// serial communication test
int led = 13;
int incoming;

void setup(){
  Serial.begin(38400);
  //Serial.println("Connected to Pi.");
  pinMode(led, OUTPUT);
}

void loop(){
  if (Serial.available()){
    incoming=Serial.read();
    Serial.println(incoming);
    
  if (incoming==49){  // ascii(49)=1
  flash_fast();
    Serial.println("receive 1");
    return;
  }
  else if (incoming ==50){
    flash_slow();
    Serial.println("receive 2");
    return;
  }
  }
}

void flash_fast(){
  Serial.println("please wait...");
  for(int i=0;i<10;i++){
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
  }
}
void flash_slow(){
  Serial.println("please wait...");
  for(int i=0;i<5;i++){
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
  }
}
