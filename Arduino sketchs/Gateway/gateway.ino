#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(8,9);                // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;   // Address of the other node in Octal format

struct payload_t {                 // Structure of our payload
  char id[30];
  float temperatura;
  float umidade;
  float co2;
  float som;
  float tensaocolmeia;
  float tensaorepetidor;

};
void setup(void)
{
  Serial.begin(57600); 
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX); 
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  
}

void loop(void){
  
  network.update();                  // Check the network regularly
  while ( network.available() ) {     // Is there anything ready for us?
    RF24NetworkHeader header;        // If so, grab it and print it out
    struct payload_t payload;
    network.read(header,&payload,sizeof(payload));
    //Serial.print("Colmeia: ");
    Serial.print(payload.id);
    Serial.print(" ");
    //Serial.print("Temperatura: ");
    Serial.print(payload.temperatura);
    Serial.print(" ");
    //Serial.print("Umidade: ");
    Serial.print(payload.umidade);
    Serial.print(" ");
    //Serial.print("Som: ");
    Serial.print(payload.som);
     Serial.print(" ");
    //Serial.print("Co2: ");
    Serial.println(payload.co2);
    //Serial.print("TensaoColmeia: ");
    Serial.println(payload.tensaocolmeia);
    //Serial.print("TensaoRepetidor: ");
    Serial.println(payload.tensaorepetidor);
  }
}
