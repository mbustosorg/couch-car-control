/*
 
 Copyright (C) 2019 Mauricio Bustos (m@bustos.org)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

#include <Arduino.h>

const int FORWARD_ANALOG_PIN = 0;
const int LATERAL_ANALOG_PIN = 1;
const int RIGHT_MOTOR_CONTROLLER = 2;
const int LEFT_MOTOR_CONTROLLER = 3;

int x, y = 0;

// Digital Pots
const int POT_COUNT = 2;
int SCKO = 9;
int CS0 = 10;
int DOUT = 11;
byte potValues[POT_COUNT];

void bitBang(int data, int dataPin, int clockPin) {
  int newData = data;
  for (int i = 0; i < 8; i++) {
    digitalWrite(dataPin, bitRead(newData, 7)); digitalWrite(clockPin, HIGH); digitalWrite(clockPin, LOW);
    newData = newData << 1;
  }
}

void updatePots() {
  digitalWrite(CS0, LOW);
  bitBang(B00010001, DOUT, SCKO);
  bitBang(potValues[1], DOUT, SCKO);
  digitalWrite(CS0, HIGH);
  digitalWrite(CS0, LOW);
  bitBang(B00010010, DOUT, SCKO);
  bitBang(potValues[0], DOUT, SCKO);
  digitalWrite(CS0, HIGH);
}

void setup() {
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode (CS0, OUTPUT);
  digitalWrite(CS0, HIGH);
  pinMode (SCKO, OUTPUT);
  pinMode (DOUT, OUTPUT);
  //SPI.begin();

  for (int i = 0; i < POT_COUNT; i++) {
    potValues[i] = 128;
  }
  updatePots();
}

void loop() {

  x = analogRead(FORWARD_ANALOG_PIN);
  y = analogRead(LATERAL_ANALOG_PIN);

  // 0 motor is left
  potValues[1] = x / 4;
  potValues[0] = y / 4;

  potValues[1] = 122 + (potValues[1] - 122) * 0.4;
  potValues[0] = 122 + (potValues[0] - 122) * 0.4;

  updatePots();

  Serial.print(potValues[0]);
  Serial.print(" ");
  Serial.print(potValues[1]);
  Serial.print(" ");
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
}