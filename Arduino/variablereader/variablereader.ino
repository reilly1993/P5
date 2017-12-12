void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

//VÆRDIER for VARIABLE
//2,5 = 510
//0,5 = 89
//1 = 168
//1,7 = 346

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A0));
  Serial.println((int) map(analogRead(A5), 0, 1023, 0, 1000)/100); //0-5 (aktør)
  delay(500);a
}
