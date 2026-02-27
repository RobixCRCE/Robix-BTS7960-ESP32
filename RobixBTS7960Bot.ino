#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

// LEFT BTS
#define L_EN_L 25  // L_EN
#define L_EN_R 26  // R_EN  
#define L_PWM  27  // LPWM - Forward
#define R_PWM  14  // RPWM - Backward

// RIGHT BTS
#define L2_EN_L 33 // L_EN
#define L2_EN_R 32 // R_EN
#define L2_PWM 4   // LPWM - Forward  
#define R2_PWM 15  // RPWM - Backward

#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

int speedVal = 180; //CAN SET IT TO 255 BUT OUR BOT WAS VERY FAST

void setup() {
  Serial.begin(115200);
  SerialBT.begin("RobixBTS7960Bot");
  
  // Setup enable pins (digital HIGH = always enabled)
  pinMode(L_EN_L, OUTPUT); digitalWrite(L_EN_L, HIGH);
  pinMode(L_EN_R, OUTPUT); digitalWrite(L_EN_R, HIGH);
  pinMode(L2_EN_L, OUTPUT); digitalWrite(L2_EN_L, HIGH);
  pinMode(L2_EN_R, OUTPUT); digitalWrite(L2_EN_R, HIGH);
  
  // NEW ESP32 3.0+ LEDC API - NO ledcSetup/ledcAttachPin!
  ledcAttach(L_PWM, PWM_FREQ, PWM_RESOLUTION);   // Left forward
  ledcAttach(R_PWM, PWM_FREQ, PWM_RESOLUTION);   // Left backward
  ledcAttach(L2_PWM, PWM_FREQ, PWM_RESOLUTION);  // Right forward
  ledcAttach(R2_PWM, PWM_FREQ, PWM_RESOLUTION);  // Right backward
  
  stopAll();
  
  Serial.println("ROBIX BOT!");
  Serial.print("Speed: "); Serial.println(speedVal);
  Serial.println("F/B/L/R/S/+/-");
}

// Motor control (uses channels automatically)
void setLeft(int forwardPWM, int backwardPWM) {
  ledcWrite(L_PWM, forwardPWM);
  ledcWrite(R_PWM, backwardPWM);
}

void setRight(int forwardPWM, int backwardPWM) {
  ledcWrite(L2_PWM, forwardPWM);
  ledcWrite(R2_PWM, backwardPWM);
}

// Movements
void forward(int spd) {
  Serial.println("FORWARD");
  setLeft(spd, 0);
  setRight(spd, 0);
}

void backward(int spd) {
  Serial.println("BACKWARD");
  setLeft(0, spd);
  setRight(0, spd);
}

void turnLeft(int spd) {
  Serial.println("LEFT");
  setLeft(0, spd);
  setRight(spd, 0);
}

void turnRight(int spd) {
  Serial.println("RIGHT");
  setLeft(spd, 0);
  setRight(0, spd);
}

void stopAll() {
  Serial.println("STOP");
  setLeft(0, 0);
  setRight(0, 0);
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    Serial.print("Cmd: '"); Serial.print(cmd); Serial.println("'");
    
    switch(cmd) {
      case 'F': case 'f': forward(speedVal); break;
      case 'B': case 'b': backward(speedVal); break;
      case 'L': case 'l': turnLeft(speedVal); break;
      case 'R': case 'r': turnRight(speedVal); break;
      case 'S': case 's': stopAll(); break;
      case '+': 
        speedVal = min(255, speedVal + 25);
        Serial.print("Speed: "); Serial.println(speedVal);
        break;
      case '-': 
        speedVal = max(50, speedVal - 25);
        Serial.print("Speed: "); Serial.println(speedVal);
        break;
      default: stopAll(); break;
    }
  }
}

//made by robix but open to all
