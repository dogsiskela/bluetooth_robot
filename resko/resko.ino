#include "BluetoothSerial.h"

#define E1 14
#define E2 17

#define I1 27
#define I2 16
#define I3 25
#define I4 26

#define buzzerPin 12

#define trigPin 5
#define echoPin 13


const int TONE_PWM_CHANNEL = 0;
int playBuzzer = 0;
int goingBack = 0;
int onObstacle = 0;

long duration;
int distance;

BluetoothSerial ESP_BT;
int incoming;

void setup() {
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);

  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);
  pinMode(I3, OUTPUT);
  pinMode(I4, OUTPUT);

  Serial.begin(9600);
  ESP_BT.begin("RESKO_BT");
  Serial.println("Bluetooth Device is Ready to Pair");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ledcAttachPin(buzzerPin, TONE_PWM_CHANNEL);
}

void toneBack() {
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_D, 4);
}

void toneBeep() {
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_G, 5);
}

void noTone() {
  ledcWrite(TONE_PWM_CHANNEL, 0);
}

void forward() {
  digitalWrite(E1, HIGH);
  digitalWrite(E2, HIGH);

  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);

  ESP_BT.println("Moving forward.");
}

void back() {
  digitalWrite(E1, HIGH);
  digitalWrite(E2, HIGH);

  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  ESP_BT.println("Going back.");
}

void left() {
  digitalWrite(E1, HIGH);
  digitalWrite(E2, HIGH);

  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  ESP_BT.println("Turning left.");
}

void right() {
  digitalWrite(E1, HIGH);
  digitalWrite(E2, HIGH);

  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);

  ESP_BT.println("Turning right.");
}

void stop() {
  digitalWrite(E1, LOW);
  digitalWrite(E2, LOW);
  ESP_BT.println("Stopped");
}

void getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);
  if (distance < 10) {
    toneBeep();
    stop();
  }
}

void loop() {
  if (ESP_BT.available())
  {
    incoming = ESP_BT.read();
    Serial.print("Received:");
    Serial.println(incoming);
    if (incoming == 52) {
      stop();
      noTone();
    }
    if (incoming == 51) {
      //ocite
      noTone();
      right();
    }
    if (incoming == 50) {
      //ocite
      noTone();
      left();
    }
    if (incoming == 49)
    {
      noTone();
      forward();
      getDistance();
    }
    if (incoming == 48)
    {
      toneBack();
      back();
    }

  }
}
