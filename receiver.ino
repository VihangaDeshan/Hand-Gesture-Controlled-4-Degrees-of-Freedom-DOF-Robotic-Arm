#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

const uint64_t pipeIn = 0xF9E8F0F0E8LL;
RF24 radio(9,8);  // CE, CSN pins
Servo xServo;     // Define a servo object for the x-axis servo
Servo yServo;     // Define a servo object for the y-axis servo
Servo claw;       // Define a servo object for the claw servo
Servo flexServo;  // Define a servo object for the flex sensor

void setup() {
  Serial.begin(115200);

  // Initialize the NRF24L01 radio module
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();

  // Attach servos to respective pins
  xServo.attach(7);    // Attach the x-axis servo to pin 7
  yServo.attach(2);    // Attach the y-axis servo to pin 2
  claw.attach(4);      // Attach the claw servo to pin 4
  flexServo.attach(5); // Attach the flex sensor servo to pin 5

  // Set initial positions of the servos
  xServo.write(50);   // X-axis servo start position
  yServo.write(90);   // Y-axis servo start position
  claw.write(75);     // Claw servo start position
  flexServo.write(80); // Flex sensor servo start position
}

void loop() {
  if (radio.available()) {
    // Receive the message wirelessly
    int data[4] = {0};

    // Read the data from the radio
    radio.read(&data, sizeof(data));

    // Assign received values to variables
    int16_t xValue = data[0];
    int16_t yValue = data[1];
    int flexValue = data[2];
    int buttonState = data[3];

    // Print received values to Serial Monitor
    Serial.print("X: ");
    Serial.print(xValue);
    Serial.print("\tY: ");
    Serial.print(yValue);
    Serial.print("\tFlex: ");
    Serial.print(flexValue);
    Serial.print("\tButton State: ");
    Serial.println(buttonState == HIGH ? "HIGH" : "LOW");

    // Map and constrain values for servo control
    int yMapped = map(yValue, -1000, 1000, 180, 0);
    yMapped = constrain(yMapped, 0, 180);

    int xMapped = map(xValue, -600, -1300, 120, 90);
    xMapped = constrain(xMapped, 90, 120);

    int flexMapped = map(flexValue, 22000, 18000, 80, 170);
    flexMapped = constrain(flexMapped, 80, 170);

    // Control the servos based on received values
    yServo.write(yMapped);
    if (buttonState == HIGH) {
      claw.write(140);   // Set claw position when button state is HIGH
    } else {
      claw.write(75);    // Set claw position when button state is LOW
    }
    xServo.write(xMapped);
    flexServo.write(flexMapped);
  }
}
