#include <Arduino.h>
#include <Servo.h>
#include <ros.h>
#include <sensor_msgs/JointState.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// ros::NodeHandle nh;
// ros::NodeHandle_<ArduinoHardware, 25, 25, 4096, 4096> nh;
ros::NodeHandle_<ArduinoHardware, 10, 10, 2048, 2048> nh;


void set(int angle1, int angle2, int angle3) {
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
  servo4.write(227 - angle1);
  servo5.write(214 - angle2);
  servo6.write(179 - angle3);

}

void jointStateCallback(const sensor_msgs::JointState &msg) {
  nh.loginfo("jointStateCallback called");

  // msg.position_length を使って配列のサイズを確認
  if (msg.position_length >= 3) {
    // Serial.print("Received Angles: ");
    // Serial.print(msg.position[0]);
    // Serial.print(", ");
    // Serial.print(msg.position[1]);
    // Serial.print(", ");
    // Serial.println(msg.position[2]);

    int angle1 = static_cast<int>(msg.position[0]);
    int angle2 = static_cast<int>(msg.position[1]);
    int angle3 = static_cast<int>(msg.position[2]);

    if ((angle1 >= 83 && angle1 <= 124) && (angle2 >= 72 && angle2 <= 132) && (angle3 >= 0 && angle3 <= 180)) {
      set(angle1, angle2, angle3);
    }
  } else {
    nh.loginfo("Insufficient data in position array.");
  }
  nh.loginfo("jointStateCallback end");
}


ros::Subscriber<sensor_msgs::JointState> sub("eye_brow/joint_states", &jointStateCallback);
// ros::Subscriber<sensor_msgs::JointState> sub("joint_states", &jointStateCallback);

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);  // power on
  
  servo1.attach(9);
  servo2.attach(8);
  servo3.attach(12);
  servo4.attach(10);
  servo5.attach(7);
  servo6.attach(11);

  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);

  // Serial.begin(57600); // Debugging output
}

void loop() {
  nh.spinOnce();
  //nh.loginfo("nh.spinOnce()");
  delay(1);
}
