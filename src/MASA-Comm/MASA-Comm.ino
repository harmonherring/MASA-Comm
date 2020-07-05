#include <LoRa_STM32.h>

int counter = 0;

void setup() {
  pinMode(PC13, OUTPUT);
  LoRa.setSpreadingFactor(12);
  if (!LoRa.begin(433E6)) {
    while (1) {
      digitalWrite(PC13, LOW);
      delay(500);
      digitalWrite(PC13, HIGH);
      delay(500);
    }
  }
}

void loop() {
  // send packet

  LoRa.beginPacket();
  LoRa.print("    ");
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  digitalWrite(PC13, LOW);
  delay(100);
  digitalWrite(PC13, HIGH);

  delay(2000);
}
