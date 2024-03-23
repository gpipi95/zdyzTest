#include <Arduino.h>
#include <Dabble.h>

#define PWMB A2
#define BIN1 PB6
#define BIN2 PB7
#define PWMA A5
#define AIN1 PG6
#define AIN2 PG7

#define DabbleTx PB10
#define DabbleRx PB11

#define DEBUG
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

HardwareSerial Serial3(PB11, PB10);

uint8_t buf[20];

void setup()
{
  pinMode(LED_GREEN, OUTPUT);
  pinMode(PF9, OUTPUT);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  Dabble.begin(115200, DabbleRx, DabbleTx);

  Serial.begin(115200);
  buf[0] = 20;
  buf[1] = 1;
}
void dir(int in1, int in2, int d)
{
  switch (d)
  {
  case 0:
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    break;
  case 1:
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    break;
  default: // stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    break;
  }
}
void loop()
{
  // digitalWrite(LED_GREEN, HIGH); // turn the LED on (HIGH is the voltage level)
  // delay(800);                    // wait for a second
  // digitalWrite(LED_GREEN, LOW);  // turn the LED off by making the voltage LOW
  // delay(300);
  // digitalWrite(PF9, HIGH); // turn the LED on (HIGH is the voltage level)
  // delay(400);              // wait for a second
  // digitalWrite(PF9, LOW);  // turn the LED off by making the voltage LOW
  // delay(600);
  Dabble.processInput();

  Serial.print("Angle:");
  Serial.print(GamePad.getAngle());
  Serial.print('\t');
  Serial.print("Radius:"); // 0 if led is Off. 1 if led is On.
  Serial.print(GamePad.getRadius());
  Serial.print('\t');
  Serial.print("Start?:");
  Serial.println(GamePad.isStartPressed());

  // analogWrite(PWMB, 10);
  // Serial.write("Hello World\r\n");
  int size = Serial.available();
  if (size)
  {
    Serial.readBytes(buf, size);
    Serial.write(buf, size);
    if (size > 1)
    {
      analogWrite(PWMB, buf[0]);
      dir(BIN1, BIN2, buf[1]);
    }
    if (size > 3)
    {
      analogWrite(PWMA, buf[2]);
      dir(AIN1, AIN2, buf[3]);
    }
  }
}
