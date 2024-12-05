#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

const int I2C_SLAVE_ADDRESS = 0x62;

// サーボオブジェクトの作成
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

float servo_now1 = 100;
float servo_now2 = 80;
float servo_now3 = 45;

uint8_t angle1;
uint8_t angle2;
uint8_t angle3;
bool newAnglesReceived = false;

int delay_time = 1500;

// サーボを指定された角度に移動させる関数
void moveServo(int angle1, int angle2, int angle3) {
  digitalWrite(2, LOW);
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
  servo4.write(227 - angle1);
  servo5.write(214 - angle2);
  servo6.write(179 - angle3);
}

// サーボを指定された角度に時間をかけて移動させる関数
void delayAndMoveServo(int delay_time, int angle1, int angle2, int angle3) {
  // 1回あたりの遅延時間を10ミリ秒に設定
  int steps = delay_time / 10;  // delay_time(ミリ秒) / 10ミリ秒
  float step_angle1 = ((float)angle1 - servo_now1) / steps;
  float step_angle2 = ((float)angle2 - servo_now2) / steps;
  float step_angle3 = ((float)angle3 - servo_now3) / steps;

  for (int i = 0; i < steps; i++) {
    servo_now1 += step_angle1;
    servo_now2 += step_angle2;
    servo_now3 += step_angle3;
    moveServo((int)servo_now1, (int)servo_now2, (int)servo_now3);
    delay(10);  // 10ミリ秒ごとに指令を送る
  }
}

static char gBuf[255];

void onReceive(int inNumOfRecvBytes) {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("onReceive called");

  const int numOfAngles = 3;
  unsigned char buffer[numOfAngles];

  // Check if the received bytes are sufficient
  unsigned char packetType = Wire.read();
  sprintf(gBuf,"%02X ", packetType);
  Serial.println(gBuf);

  if (packetType != 1) {
    Serial.println("invalid Packet Type");
    digitalWrite(LED_BUILTIN, LOW);
    while (Wire.available()) {
      Wire.read();
    }
    return;
  }

  if (Wire.available() >= numOfAngles) {
    for (int i = 0; i < numOfAngles; i++) {
      buffer[i] = Wire.read();
    }
    while (Wire.available()) {
      Wire.read();
    }
    angle1 = buffer[0];
    angle2 = buffer[1];
    angle3 = buffer[2];
    Serial.print("Angle1: ");
    Serial.println(angle1);
    Serial.print("Angle2: ");
    Serial.println(angle2);
    Serial.print("Angle3: ");
    Serial.println(angle3);
    if ((angle1 >= 83 && angle1 <= 124) &&
        (angle2 >= 72 && angle2 <= 132) &&
        (angle3 >= 0 && angle3 <= 180)) {
      newAnglesReceived = true;
    } else {
      Serial.println("Angles are out of range.");
    }
  } else {
    Serial.println("Insufficient data received.");
  }
  Serial.println("onReceive end");
  digitalWrite(LED_BUILTIN, HIGH);
}


// setup()関数
void setup() {
  Serial.begin(9600); // serial.print用

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);  // servo off.

  // サーボのピンに接続
  servo1.attach(9);
  servo2.attach(8);
  servo3.attach(12);
  servo4.attach(10);
  servo5.attach(7);
  servo6.attach(11);

  // I2C
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(onReceive);
}

// loop()関数
void loop() {
  if (newAnglesReceived) {
    // digitalWrite(2, LOW);
    delayAndMoveServo(delay_time, angle1, angle2, angle3);
    digitalWrite(2, HIGH);
    newAnglesReceived = false;
  }
  delay(1);  // 1ミリ秒の遅延
}
