#include <JeeLib.h>
#include <RF24Network.h>
#include <SPI.h>
#include <DHT.h>
#include <RF24.h>

#define DHTPIN 1       // Pino DATA do Sensor DHT.
#define DHTTYPE DHT22   // Define o tipo do sensor utilizado DHT 11
#define IDCOLMEIA "Colmeia2" //ID da Colmeia monitorada
#define TEMPOENTRECADALEITURA 55000 //Tempo entre cada leitura 

DHT dht(DHTPIN, DHTTYPE); //Objeto do sensor de temperatura

RF24 radio(8, 9);                   // nRF24L01(+) radio attached using Getting Started board
RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format

struct paraenviar {                  // Structure of our payload
  char id[30];
  float temperatura;
  float umidade;
  float co2;
  float som;
};
float temperatura_lida = 0;
float umidade_lida = 0;
float co2_lido = 0;
float som_lido = 0;
int led = 13;

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void lerDHT(){
  if (isnan(dht.readTemperature())) {                   
    temperatura_lida = 0;
  }
  else {
    temperatura_lida = dht.readTemperature();
  }
  if (isnan(dht.readHumidity())) {             
    umidade_lida = 0;
  }
  else {                                              
    umidade_lida = dht.readHumidity();
  }
}
void lerMQandKy(){
  som_lido = analogRead(A0);
  co2_lido = 0;
}
void setup(void)
{
  Serial.begin(57600);
  digitalWrite(led, LOW);
  dht.begin();                           // Inicializa a classe do sensor

  SPI.begin();
  radio.begin();
  radio.stopListening();
  radio.setPALevel(RF24_PA_MAX); 
  //The possibilities are: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() {

  network.update();
  lerDHT();   
  lerMQandKy();                 
  struct paraenviar payload;
  strcpy( payload.id, IDCOLMEIA);
  payload.temperatura = temperatura_lida;
  payload.umidade = umidade_lida;
  payload.som = som_lido;
  payload.co2 = co2_lido;

  RF24NetworkHeader header(/*to node*/ other_node);
  bool ok = network.write(header, &payload, sizeof(payload));
  if (ok)
    Serial.println("ok.");
  else
    Serial.println("failed.");
  Sleepy::loseSomeTime(TEMPOENTRECADALEITURA);
}
