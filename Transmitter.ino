#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <MPU6050.h>
MPU6050 mpu;

const uint64_t pipeOut = 0xF9E8F0F0E8LL;
RF24 radio(9,8);  // CE, CSN pins

void setup() {
   Serial.begin(115200);

  // Initialize the NRF24L01 radio module
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  Wire.begin();

  // Initialize the MPU6050 sensor
  mpu.initialize();
}

void loop() {
  // Read MPU6050 data
  int16_t xValue = mpu.getAccelerationX() * 0.1;
  int16_t yValue = mpu.getAccelerationY() * 0.1;

  // Read flex sensor value and push button state
  int flexValue = analogRead(A0) * 50;  
  int buttonState = digitalRead(2);

  // Prepare data array
  int data[4] = {xValue, yValue, flexValue, buttonState};

  // Send data wirelessly
  radio.write(&data, sizeof(data));

  // Print data to Serial Monitor for debugging
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print("\tY: ");
  Serial.print(yValue);
  Serial.print("\tFlex: ");
  Serial.print(flexValue);
  Serial.print("\tButton: ");
  Serial.println(buttonState);

  // Adjust delay as needed
  delay(0);
}
