#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(8,9);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t id_origem = 04;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t id_destino = 01;   // Address of the other node in Octal format

struct payload_t {                 // Structure of our payload
  char id[100];
};

float tensao_lida = 0;
int SENSORTENSAO=1;
char *para_enviar;

void lerTensao(){
  float valor_lido_tensao = analogRead(SENSORTENSAO);
  tensao_lida=((valor_lido_tensao*0.004887586)*4.65);
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup(void)
{
  Serial.begin(57600);
  Serial.println("Receptor");
 
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  network.begin(/*channel*/ 90, /*node address*/ id_origem);
}

void loop(void){
  
  network.update();                  // Check the network regularly
  
  while ( network.available() ) {     // Is there anything ready for us?

    //recebe o que foi enviado do repetidor 2
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t recebido;
    network.read(header,&recebido,sizeof(recebido));
    //ENVIAR PARA GATEWAY
    String mensagem_recebida = recebido.id;
    payload_t enviar;
    //lerTensao();
    mensagem_recebida+=':';
    mensagem_recebida+=tensao_lida;
  
    char char_mensagem[100];
    mensagem_recebida.toCharArray(char_mensagem, 100);
    strcpy( enviar.id, char_mensagem);
    RF24NetworkHeader headerEnviar(id_destino);
    bool ok = network.write(headerEnviar,&enviar,sizeof(enviar));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");    
  }
}

