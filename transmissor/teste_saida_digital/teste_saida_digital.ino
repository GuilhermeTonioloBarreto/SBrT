// Checa se todas as saídas digitais estão funcionando corretamente
// As saídas piscam de 1 em 1 segundo

int saidaDigital[] = {2, 3, 4, 5, 7, 8, 12, 13}; //devem ser colocados manualmente

void setup() {
  for (int i = 0; i < 8; i++)
    pinMode(saidaDigital[i], OUTPUT);
}

void loop() { 
  for (int i = 0; i < 8; i++)
    digitalWrite(saidaDigital[i], HIGH);
    
  delay(1000);
  
  for (int i = 0; i < 8; i++)
    digitalWrite(saidaDigital[i], LOW);    
  
  delay(1000);
}
