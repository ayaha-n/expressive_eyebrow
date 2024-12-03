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

int delay_time = 1500;

// サーボを指定された角度に移動させる関数
void moveServo(int angle1, int angle2, int angle3) {
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

void onReceive(int inNumOfRecvBytes) {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("onReceive called");

  const int numOfAngles = 3;
  const int bytesPerDouble = 8; // float64 is 8 bytes
  unsigned char buffer[numOfAngles * bytesPerDouble];

  // Check if the received bytes are sufficient
  if (Wire.available() >= numOfAngles * bytesPerDouble) {
    for (int i = 0; i < numOfAngles * bytesPerDouble; i++) {
      buffer[i] = Wire.read();
    }

    double angles[numOfAngles];
    for (int i = 0; i < numOfAngles; i++) {
      // Combine 8 bytes into a double
      memcpy(&angles[i], &buffer[i * bytesPerDouble], bytesPerDouble);
    }

    double angle1 = angles[0];
    double angle2 = angles[1];
    double angle3 = angles[2];

    Serial.print("Angle1: ");
    Serial.println(angle1, 6); // Print with precision
    Serial.print("Angle2: ");
    Serial.println(angle2, 6);
    Serial.print("Angle3: ");
    Serial.println(angle3, 6);

    if ((angle1 >= 83.0 && angle1 <= 124.0) &&
        (angle2 >= 72.0 && angle2 <= 132.0) &&
        (angle3 >= 0.0 && angle3 <= 180.0)) {
      delayAndMoveServo(delay_time, angle1, angle2, angle3);
    } else {
      Serial.println("Angles are out of range.");
    }
  } else {
    Serial.println("Insufficient data received.");
  }

  Serial.println("onReceive end");
  digitalWrite(LED_BUILTIN, LOW);
}


// setup()関数
void setup() {
  Serial.begin(9600); // serial.print用

  pinMode(LED_BUILTIN, OUTPUT);

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
  delay(1);  // 1ミリ秒の遅延
}