// RC motor controller

int rotorPin1 = 3; // Set PWM outputs.
int rotorPin2 = 9;
int rotorPin3 = 10;
int rotorPin4 = 11;

int motorsCheckPin = 7;

int lowDuty  = 32; // Lowest duty cycle is 12.5%, scaled to 0-255 for analogWrite().
int highDuty = 64; // Highest duty cycle is 25%, scaled to 0-255 for analogWrite().

boolean ESCsPoweredAtStart = false; // Check if rotors are powered at the start
boolean ESCsPowered = false; // Used to check rotors' state.
boolean startupSuccessful = false;

void setup() {
  TCCR2B = (TCCR2B & 0xF8) | 6; // Set clock to 122 Hz for pins 11 and 3
  TCCR1B = (TCCR1B & 0xF8) | 4; // And for pins 9 and 10
  
  Serial.begin(9600);
  Serial.println("Serial begun");
  
  pinMode(rotorPin1, OUTPUT);
  pinMode(rotorPin2, OUTPUT);
  pinMode(rotorPin3, OUTPUT);
  pinMode(rotorPin4, OUTPUT);
  
  pinMode(motorsCheckPin, INPUT);
  
  if (digitalRead(motorsCheckPin) == HIGH) { // Check if ESCs are already powered at Arduino startup. If they are, we can't start them properly.
    ESCsPoweredAtStart = true;
  }
  
  setRotor(0, 0); // Set pins to lowest duty cycle, ready for rotors to be powered up
  
}

void loop() {
  //Serial.println(ESCsPoweredAtStart);
  
  if (ESCsPoweredAtStart == false) { // If we didn't have power at the start, we have the possibility of starting the motors.
    if (digitalRead(motorsCheckPin) == HIGH) { // Check if ESCs are powered now
      ESCsPowered = true;
    }
    else {
      ESCsPowered = false;
    }
    
    if (ESCsPowered == true && startupSuccessful == true) {  // If we have power and have started the motors, enter main flying loop.
      Serial.println("Reaches main loop");
    }

    else if (ESCsPowered == true && startupSuccessful == false) { // If we have power and haven't started the motors, we have gotten power *just now*.

      long startTime = millis();
      long curTime = millis();
      
      while(curTime-startTime < 5000) { // Wait 5s at low duty cycle
        curTime = millis();
      }
      
      setRotor(0, 0.1); // Set rotors to a small idle value
      
      startupSuccessful = true;
      
      Serial.println("Startup time!");
    }
  }
}

void powerUpRotor() { // Power up the rotors, then idle them at 0.5 effort.
  long startTime = millis(); // Initialise time at which rotors have started to undergo powerup sequence. Might need to change this to look at start time for individual rotors.
  
  setRotor(0, 0.0); // Set all rotors to minimal effort
  
  digitalWrite(13, LOW);
  
  long curTime = millis();
  while(curTime-startTime < 5000) { // Wait 5s at 12.5%.
    curTime = millis();
  }

  digitalWrite(13, LOW);
  
  setRotor(0, 0.5); // Set all rotors to half effort

  digitalWrite(13, HIGH);

}

void setRotor(int rotor, float effort) {
  if (effort < 0.0) {
    effort = 0.0;
  }
  if (effort > 1.0) {
    effort = 1.0;
  }
  
  if (0 <= effort <= 1) {
    int duty = lowDuty + (highDuty-lowDuty)*effort; // Duty cycle, scaled to 0-255 for analogWrite()
    
    Serial.print("effort is");
    Serial.println(effort);
    
    Serial.print("duty is");
    Serial.println(duty);
    
    if      (rotor == 1) {
      analogWrite(rotorPin1, duty);
    }
    else if (rotor == 2) {
      analogWrite(rotorPin2, duty);
    }
    else if (rotor == 3) {
      analogWrite(rotorPin3, duty);
    }
    else if (rotor == 4) {
      analogWrite(rotorPin4, duty);
    }
    else if (rotor == 0) {
      analogWrite(rotorPin1, duty);
      analogWrite(rotorPin2, duty);
      analogWrite(rotorPin3, duty);
      analogWrite(rotorPin4, duty);
    }
  }
}
