#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <printf.h>

//Definicao do NRF24L01
RF24 radio(7,8);    //CE CSN            // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;   // Address of the other node in Octal format

struct payload_recebido{                 // Structure of our payload
  char colmeia;
  float temperatura;
  float umidade;
  float co2;
  float som;
};

void setup(void)
{
  Serial.begin(57600);
  Serial.println("Iniciando Gateway...[ok]");
  Serial.println("Iniciando SPI...[ok]");
  SPI.begin();
  Serial.println("Iniciando radio...[ok]");
  radio.begin();
  Serial.println("Iniciando network...[ok]");
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void){
  
  network.update();                  // Check the network regularly

  
  while ( network.available() ) {     // Is there anything ready for us?
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_recebido payload;
    network.read(header,&payload,sizeof(payload));
    Serial.println("##############################");
    Serial.print("Colmeia: ");
    Serial.println(payload.colmeia);
    
    Serial.print("Temperatura: ");
    Serial.println(payload.temperatura);
    
    Serial.print("Umidade: ");
    Serial.println(payload.umidade);
    
    Serial.print("Co2: ");
    Serial.println(payload.co2);

    Serial.print("Som: ");
    Serial.println(payload.som);
    Serial.println("##############################");
  }
}

