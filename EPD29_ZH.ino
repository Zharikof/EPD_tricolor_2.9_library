// Library for Electronic Parer Display 2.9' tree color White, Black, Red. SPI wire. (GDEM029C90) "Good Display Co."
// by Zharikov A. (icitici@gmail.com)
#include <Arduino.h>
#include <SPI.h>
#include "picture.h"
#define RST_PIN 5
#define DC_PIN 4
#define CS_PIN 7
#define BUSY_PIN 16
// SPI: SCL - 12, SDA - 11

void icitici_epd_SendCommand(unsigned char command) {
  digitalWrite(DC_PIN, LOW);
  SpiTransfer(command);
}

void icitici_epd_SendData(unsigned char data) {
  digitalWrite(DC_PIN, HIGH);
  SpiTransfer(data);
}

void icitici_epd_wait_busy(void) {
  while (digitalRead(BUSY_PIN) == HIGH) {  //LOW - pin is free, HIGH - it is busy
    delay(1000);
  }
}

void SpiTransfer(unsigned char data) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(data);
  digitalWrite(CS_PIN, HIGH);
}
void icitici_epd_reset(void) {
  digitalWrite(RST_PIN, LOW);  
  delay(100);
  digitalWrite(RST_PIN, HIGH);
  delay(100);
  icitici_epd_wait_busy();
}
void icitici_epd_Init(void) {
  pinMode(CS_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
  pinMode(BUSY_PIN, INPUT);
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  icitici_epd_reset();
  icitici_epd_wait_busy();
  delay(100);
  icitici_epd_SendCommand(0x12);  // soft reset
  icitici_epd_wait_busy();
  delay(100);
  icitici_epd_SendCommand(0x74);  //analog block control
  icitici_epd_SendData(0x54);
  icitici_epd_SendCommand(0x7E);  //digital block control
  icitici_epd_SendData(0x3B);
  icitici_epd_SendCommand(0x01);  //Driver output control
  icitici_epd_SendData(0x27);
  icitici_epd_SendData(0x01);
  icitici_epd_SendData(0x00);
  icitici_epd_SendCommand(0x11);  //data entry mode
  icitici_epd_SendData(0x03);     
  icitici_epd_SendCommand(0x3C);  //Border Wave from
  icitici_epd_SendData(0x05);     
  icitici_epd_SendCommand(0x18);
  icitici_epd_SendData(0x80);
  icitici_epd_SendCommand(0x21);  //  Display update control
  icitici_epd_SendData(0x00);
  icitici_epd_SendData(0x80);
  icitici_epd_SendCommand(0x44);
  icitici_epd_SendData(0x00);
  icitici_epd_SendData(127 / 8);
  icitici_epd_SendCommand(0x45);
  icitici_epd_SendData(0x00);
  icitici_epd_SendData(0x00);
  icitici_epd_SendData(296 % 256);
  icitici_epd_SendData(296 / 256);
  icitici_epd_SendCommand(0x4e);
  icitici_epd_SendData(0 / 8);
  icitici_epd_SendCommand(0x4f);
  icitici_epd_SendData(0 % 256);
  icitici_epd_SendData(0 / 256);
}
void icitici_show_image(const unsigned char *datas1, const unsigned char *datas2) {
  unsigned int i;
  icitici_epd_SendCommand(0x24);  //write RAM. Black(0x00), White (0XFF)
  for (int i = 0; i < 4736; i++) {
    icitici_epd_SendData(pgm_read_byte(&datas1[i]));
  }
  icitici_epd_SendCommand(0x26);  //write RAM. Red(0xFF), White (0x00)
  for (int i = 0; i < 4736; i++) {
    icitici_epd_SendData(pgm_read_byte(&datas2[i]));
  }
  icitici_epd_SendCommand(0x22);
  icitici_epd_SendData(0xF7);
  icitici_epd_SendCommand(0x20);
  icitici_epd_wait_busy();
  icitici_epd_sleep();
}

void icitici_epd_sleep(void) {
  icitici_epd_SendCommand(0x10);  
  icitici_epd_SendData(0x01);
}

void setup() {
  Serial.begin(115200);
  icitici_epd_Init();  
  icitici_show_image(gImage_picture_128x296, gImage_picture_128x296_red);
  icitici_epd_sleep();  
}

void loop() {
}
