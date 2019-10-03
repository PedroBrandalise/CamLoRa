#include <SPI.h>  //responsável pela comunicação serial
#include <LoRa.h>  //responsável pela comunicação com o WIFI Lora
#include <Wire.h>  //responsável pela comunicação i2c
//#include <SSD1306.h> //responsável pela comunicação com o display
//#include "images.h" //contém o logo para usarmos ao iniciar o display


// Definição dos pinos 
#define SCK 5 // GPIO5 -- SX127x's SCK
#define MISO 19 // GPIO19 -- SX127x's MISO
#define MOSI 27 // GPIO27 -- SX127x's MOSI
#define SS 18 // GPIO18 -- SX127x's CS
#define RST 14 // GPIO14 -- SX127x's RESET
#define DI00 26 // GPIO26 -- SX127x's IRQ (Interrupt Request)
 
#define BAND 915E6 //Frequencia do radio - podemos utilizar ainda : 433E6, 868E6, 915E6
#define PABOOST true

unsigned int counter = 0;
 
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void setup(){
    Serial.begin(9600);
    SPI.begin(SCK,MISO,MOSI,SS); //inicia a comunicação serial com o Lora
    LoRa.setPins(SS,RST,DI00); //configura os pinos que serão utlizados pela biblioteca (deve ser chamado antes do LoRa.begin)
    //inicializa o Lora com a frequencia específica.
    if(!LoRa.begin(BAND)){
      Serial.println("erro ao iniciar Lora");
      while(1);
    }
    Serial.println("sucesso ao iniciar Lora");
    delay(1000);
}
void loop(){
  int aux;
  //beginPacket : abre um pacote para adicionarmos os dados para envio
  LoRa.beginPacket();
  //print: adiciona os dados no pacote
  LoRa.print("hello ");
  LoRa.print((String) counter);
  //endPacket : fecha o pacote e envia
  Serial.println(counter);
  aux =LoRa.endPacket(); //retorno= 1:sucesso | 0: falha
  if(aux==1){
    Serial.print("DEU BOA\n");
  }
   counter++;
     
  digitalWrite(25, HIGH); // liga o LED indicativo
  delay(500); // aguarda 500ms
  digitalWrite(25, LOW); // desliga o LED indicativo
  delay(500); // aguarda 500ms
  
}
