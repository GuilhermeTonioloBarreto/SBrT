
// Configuração inicial OOK
int n_entradas_ook = 3; // Numero de bits no campo de enderecamento;
int n_bits_campo_dados = 8; // Numero de bits que serah transmitido no campo de dados;
int pinos_saida_ook[] = {2, 3, 4, 5, 7, 8, 12, 13};

// Configuração inicial CSK
int n_saidas_csk = 4;
int pinos_saida_csk[] = {6, 9, 10, 11};

// Ativação OOK e CSK
bool ativar_ook = true;
bool ativar_csk = true;
bool csk4 = false;
bool csk8 = true;

void setup()
{
  Serial.begin(9600); // Inicialização do monitor serial

  if (ativar_ook)
  {
    // Declara os pinos de saída ook
    const int n_saidas_ook = (int)(pow(2.0, (float)n_entradas_ook) + 0.1);
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
  int b_ook; // Recebe cada byte da entrada serial
  int j = 0; // incrementa-se cada vez que o valor de b eh armazenado em enderecamento_ook[]
  int saida_ook[n_bits_campo_dados]; // Vetor que armazena os bits de dados a serem transmitidos
  const int n_saidas_ook = (int)(pow(2.0, (float)n_entradas_ook) + 0.1); // Calcula novamente o número de saídas OOK que serão utilizadas 
  float num_decimal = 0.0; // Utilizado para converter o valor do demux em número decimal
  int enderecamento_ook[n_entradas_ook];// Entradas de enderecamento ook
  bool leitura_dados_ook = false;
  bool transmissao_dados_ook = false;
  
  //Configurações CSK
  int b_csk; // Recebe cada byte da entrada serial
  int saida_csk[] = {0, 0, 0, 0}; // Array com o valor das saídas CSK
  bool leitura_dados_csk = false;
  bool transmissao_dados_csk = false;  

  // leitura dos dados OOK
  if (ativar_ook)
  {
    while(leitura_dados_ook == false)
    {
      // Leitura da entrada serial
      if (Serial.available() > 0)
      {
        b_ook = Serial.read() - 48; // Leitura serial
  
        // Atribuir os primeiros bits em controle[] e o último na saída digital
        if (j >= 0 && j < n_entradas_ook)
          enderecamento_ook[j] = b_ook;
        else if (j >= n_entradas_ook && j < n_entradas_ook + n_bits_campo_dados)
          saida_ook[j - n_entradas_ook] = b_ook;
        
        j = j + 1;// Incrementa o valor de j
  
        // Laço de Decisão que ocorre quando todos os bits de endereçamento e o de saída OOK forem lidos
        if (j == n_entradas_ook + n_bits_campo_dados)
          leitura_dados_ook = true;

      } // Final leitura de entrada serial 
    } // Final leitura dos dados OOK
  } // Final ativar ook
  
 // Leitura de dados CSK
  if (ativar_csk)
  {
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

  if (ativar_csk)
  {
    // Transmissão de dados CSK
    if (transmissao_dados_csk == false)
    {     
      // Atribui saídas PWM
      for (int i = 0; i < n_saidas_csk; i++)
        analogWrite(pinos_saida_csk[i], saida_csk[i]);

      transmissao_dados_csk = true;
    } // Final transmissão de dados csk
  } // Final Ativar CSK
  

  if (ativar_ook)
  {    
    // Converte a entrada de controle do demux em número decimal
    for (int i = 0; i < n_entradas_ook ; i++)
      num_decimal += (float)enderecamento_ook[i]*pow(2.0, (float)(n_entradas_ook - 1 - i));
    num_decimal += 0.1;// Arruma o erro da função pow()
    
    // Preparar Transmissão de dados OOK
    if (transmissao_dados_ook == false)
    {
      int saida_escolhida = 0; // Armazena a saida a transmitir os bits do campo OOK
      // Atribui HIGH ou LOW para a saída Digital
      for (int i = 0; i < n_saidas_ook; i++)
        if (i == (int)num_decimal)
          saida_escolhida = i;
        else
          digitalWrite(pinos_saida_ook[i], LOW);
       
       for(int i = 0; i < n_bits_campo_dados; i++){
        if (saida_ook[i] == 1)
          digitalWrite(pinos_saida_ook[saida_escolhida], HIGH);
        else
          digitalWrite(pinos_saida_ook[saida_escolhida], LOW);
       
       //Delay provisorio para ver se funciona
       delay(500);
       }
      
       transmissao_dados_ook = true;

    } // Final transmissão de dados OOK 
  } // Final ativar OOK  
} // Final void loop
