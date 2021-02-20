

int buzzer=9;
int echoPin=12;
int trigPin=13;


void setup(){
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(buzzer,OUTPUT);

}

void loop(){
  digitalWrite(trigPin,LOW);
  digitalWrite(echoPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

 unsigned long duration = pulseIn(echoPin,HIGH);

 float distance=((float)duration)/58;

 Serial.print(distance);
 Serial.println("cm");

 if(distance<50)
 {
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(1);
 }
delay(1000);
}
