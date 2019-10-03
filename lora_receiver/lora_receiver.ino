#include <SPI.h>  //responsável pela comunicação serial
//#include <Serial.h>
#include <LoRa.h>  //responsável pela comunicação com o WIFI Lora
#include <Wire.h>  //responsável pela comunicação i2c
//#include <Adafruit_SSD1306.h> //responsável pela comunicação com o display
#include "SSD1306.h"
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

SSD1306 display(0x3c, 4, 15);
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void setup(){
  Serial.begin(9600);
  //configura os pinos como saida
  pinMode(16,OUTPUT); //RST do oled
  pinMode(25,OUTPUT);
  digitalWrite(16, LOW); // reseta o OLED
  delay(50); 
  digitalWrite(16, HIGH); // enquanto o OLED estiver ligado, GPIO16 deve estar HIGH
   
  display.init(); //inicializa o display
  display.flipScreenVertically(); 
  display.setFont(ArialMT_Plain_10); //configura a fonte para um tamanho maior
   
  //imprime o logo na tela
//  logo();
   
  delay(1500);
  display.clear();
  int status = LoRa.begin(BAND);
  while(!status) {
    display.clear();
    delay(50);
    display.drawString(0, 0, "erro ao iniciar LoRa");
    Serial.println(".");
    display.display();
    status = LoRa.begin(BAND);
  }

  display.drawString(0, 0, "LoRa Initial success!");
  display.drawString(0, 10, "Wait for incomm data...");
  display.display();
  delay(1000);

  Serial.println("sucesso inicial!!");
  LoRa.receive();
  
}

void loop() {
  
  //parsePacket: checa se um pacote foi recebido
  //retorno: tamanho do pacote em bytes. Se retornar 0 (ZERO) nenhum pacote foi recebido
  int packetSize = LoRa.parsePacket();
  //caso tenha recebido pacote chama a função para configurar os dados que serão mostrados em tela
  if (packetSize) { 
    cbk(packetSize); //função responsável por recuperar o conteúdo do pacote recebido
    delay(10);
  }
  Serial.println("-->");
  Serial.println(packet);
}
void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC); //transforma o tamanho do pacote em String para imprimirmos
  for (int i = 0; i < packetSize; i++) { 
    packet += (char) LoRa.read(); //recupera o dado recebido e concatena na variável "packet"
  }
  
  rssi = "RSSI=  " + String(LoRa.packetRssi(), DEC)+ "dB"; //configura a String de Intensidade de Sinal (RSSI)
  //mostrar dados em tela
  
  loraData();
}

//função responsável por configurar os dadosque serão exibidos em tela.
//RSSI : primeira linha
//RX packSize : segunda linha
//packet : terceira linha
void loraData(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 , 18 , "Rx "+ packSize + " bytes");
  Serial.println(packet);
  display.drawStringMaxWidth(0 , 39 , 128, packet);
  display.drawString(0, 0, rssi);  
  display.display();
}
