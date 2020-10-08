void setup() {
 Serial.begin(9600);
}

void loop() { 
  int vetor[] = {1, 2, 3, 5};

  for(int i = 0; i < 4; i++){
    Serial.print(vetor[i]);
    delay(1000);
  }
}
