
// Configuração inicial OOK
int n_entradas_ook = 3; // Numero de bits no campo de enderecamento;
int n_bits_campo_dados = 8; // Numero de bits que serah transmitido no campo de dados;
int pinos_saida_ook[] = {2, 3, 4, 5, 7, 8, 12, 13};

// Configuração inicial CSK
int n_saidas_csk = 4;
int pinos_saida_csk[] = {6, 9, 10, 11};

// Configuracao quadro de Sincronismo
int sinc_iniciar = 3; // diz quantos quadros ook/csk devem passar ateh transmisir o quadro de sincrinismo
int sinc_contador = 1; // contador para saber quantos quadros devem passar para ativar o quadro de sincronismo
int time_sinc = 3000; // tempo em que o bit de sincronismo eh transmitido

// Ativação OOK, CSK e quadro sincronismo
bool ativar_ook = true;
bool ativar_csk = true;
bool csk4 = false;
bool csk8 = true;
bool sinc_ativar = true;

// Funcao que converte um numero binario em numero decimal
int converte_binario_em_decimal(int *n_entradas_ook, int enderecamento_ook[]){
  float num_decimal = 0.0;
  for (int i = 0; i < *n_entradas_ook ; i++)
    num_decimal += (float)enderecamento_ook[i]*pow(2.0, (float)(*n_entradas_ook - 1 - i));
  num_decimal += 0.1;// Arruma o erro da função pow()
  return (int) num_decimal; 
}

// Funcao que calcula o numero de saidas OOk a serem utilizadas baseado no numero de entradas de enderecamento
int calcula_n_saidas_ook(int *n_entradas_ook){
  return (int)(pow(2.0, (float)*n_entradas_ook) + 0.1);
}

void setup()
{
  Serial.begin(9600); // Inicialização do monitor serial

  if (ativar_ook)
  {
    // Declara os pinos de saída ook
    const int n_saidas_ook = calcula_n_saidas_ook(&n_entradas_ook);
    for (int i = 0; i < n_saidas_ook; i++)
      pinMode(pinos_saida_ook[i], OUTPUT);
  } // Final ativar ook

  if (ativar_csk)
  {
    // Declara os pinos de saída CSK
    for (int i = 0; i < n_saidas_csk; i++)
      pinMode(pinos_saida_csk[i], OUTPUT);
  } // Final ativar csk
  
}

void loop() 
{
  // Configurações OOK
  int saida_ook[n_bits_campo_dados]; // Vetor que armazena os bits de dados a serem transmitidos
  const int n_saidas_ook = calcula_n_saidas_ook(&n_entradas_ook); // Calcula novamente o número de saídas OOK que serão utilizadas 
  int n_entradas_ook_decimal = 0; // Eh o numero de entradas OOk no formato decimal
  int enderecamento_ook[n_entradas_ook];// Entradas de enderecamento ook
  bool transmissao_dados_ook = false;
  
  //Configurações CSK
  int b_csk; // Recebe cada byte da entrada serial
  int saida_csk[] = {0, 0, 0, 0}; // Array com o valor das saídas CSK
  

  // leitura dos dados OOK
  if (ativar_ook)
  {
    int b_ook = 0; // Recebe cada byte da entrada serial
    int j = 0; // incrementa-se cada vez que o valor de b eh armazenado em enderecamento_ook[]
    bool leitura_dados_ook = false;
    
    while(leitura_dados_ook == false)
    {
      // Leitura da entrada serial
      if (Serial.available() > 0)
      {
        b_ook = Serial.read() - 48; // Leitura serial
  
        // Atribuir os primeiros bits em enderecamento_ook[] e os últimos bits em saida_ook[]
        if (j >= 0 && j < n_entradas_ook)
          enderecamento_ook[j] = b_ook;
        else if (j >= n_entradas_ook && j < n_entradas_ook + n_bits_campo_dados)
          saida_ook[j - n_entradas_ook] = b_ook;
        
        j = j + 1;// Incrementa o valor de j
  
        // Sai do loop de leitura de dados se todos os bits foram lidos
        if (j == n_entradas_ook + n_bits_campo_dados)
          leitura_dados_ook = true;

      } // Final leitura de entrada serial 
    } // Final leitura dos dados OOK
  } // Final ativar ook
  
 // Leitura de dados CSK
  if (ativar_csk)
  {
    bool leitura_dados_csk = false;
    while (leitura_dados_csk == false)
    {
      // Leitura da entrada serial
      if (Serial.available() > 0)
      {
        b_csk = Serial.read() - 48;

        // Utilizando a modulação 4-csk
        if (csk4 == true)
        {
          if (b_csk == 0)
            saida_csk[0] = 255;
        
          else if (b_csk == 1)
            saida_csk[1] = 255;
        
          else if (b_csk == 2)
            saida_csk[3] = 255;
        
          else if (b_csk == 3)
            saida_csk[2] = 255;  
        } // Final csk-4

        // Utilizando a modulação 8-csk
        if (csk8 == true)
        {
          if (b_csk == 0)
            saida_csk[0] = 255;
        
          else if (b_csk == 1)
          {
            saida_csk[0] = 128;
            saida_csk[1] = 128;
          }
        
          else if (b_csk == 2)
          {
            saida_csk[1] = 128;
            saida_csk[2] = 128;
          }
      
          else if (b_csk == 3)
            saida_csk[1] = 255;
      
          else if (b_csk == 4)
          {
            saida_csk[0] = 128;
            saida_csk[3] = 128;
          }
        
          else if (b_csk == 5)
            saida_csk[3] = 255;
        
          else if (b_csk == 6)
            saida_csk[2] = 255;
        
          else if (b_csk == 7)
          {
            saida_csk[2] = 128;
            saida_csk[3] = 128;
          }
        } // Final csk-8
      
        leitura_dados_csk = true;
    
      } // Final leitura de entrada serial
    } // Fim leitura de dados CSK
  } // Final Ativar CSK

  // Transmissão de dados CSK
  if (ativar_csk)
    // Atribui valores à saídas PWM
    for (int i = 0; i < n_saidas_csk; i++)
      analogWrite(pinos_saida_csk[i], saida_csk[i]);
  

  if (ativar_ook)
  {    
    n_entradas_ook_decimal = converte_binario_em_decimal(&n_entradas_ook, enderecamento_ook);   
 
    int saida_escolhida = 0; // Armazena a saida a transmitir os bits do campo OOK
    
    // Atribui HIGH ou LOW para as saídas Digitais nao escolhidas
    for (int i = 0; i < n_saidas_ook; i++)
      if (i == n_entradas_ook_decimal)
        saida_escolhida = i;
      else
        digitalWrite(pinos_saida_ook[i], LOW);

     // Transmite os bits do campo de dados na saida escolhida
     for(int i = 0; i < n_bits_campo_dados; i++){
      if (saida_ook[i] == 1)
        digitalWrite(pinos_saida_ook[saida_escolhida], HIGH);
      else
        digitalWrite(pinos_saida_ook[saida_escolhida], LOW);
     
     //Delay provisorio para ver se funciona
     delay(500);
     } // Final Laco For para transmissao dos bits do campo de enderecamento     
  } // Final ativar OOK

  if(sinc_ativar)
    if(sinc_contador < sinc_iniciar)
      sinc_contador++;
    else{
      // Todos os LEDs OOK transmitem nivel logico alto
      for (int i = 0; i < n_saidas_ook; i++)
        digitalWrite(pinos_saida_ook[i], HIGH);

      // Todos os LEDs CSK transmitem nivel logico baixo
      for (int i = 0; i < n_saidas_csk; i++)
        analogWrite(pinos_saida_csk[i], LOW);

      delay(time_sinc);
      sinc_contador = 1;
      
    }// Final loop else
} // Final void loop
