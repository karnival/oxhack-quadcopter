// RC motor controller

int signalPinLB = 3; // Set PWM outputs.
int signalPinRB = 9;
int signalPinLF = 10;
int signalPinRF = 11;

int lowDuty  = 32; // Lowest duty cycle is 12.5%, scaled to 0-255 for analogWrite().
int highDuty = 64; // Highest duty cycle is 25%, scaled to 0-255 for analogWrite().
int medDuty = 40;

boolean rotorsPowered = false; // Check rotors' state.

void setup(){
  TCCR2B = (TCCR2B & 0xF8) | 6; // Set clock to 122 Hz for pins 11 and 3
  TCCR1B = (TCCR1B & 0xF8) | 4; // And for pins 9 and 10
  
  Serial.begin(9600);
  Serial.println("Serial begun");
  
  pinMode(signalPinLB, OUTPUT);
  pinMode(signalPinRB, OUTPUT);
  pinMode(signalPinLF, OUTPUT);
  pinMode(signalPinRF, OUTPUT);
  
  pinMode(13, OUTPUT);
  
  powerUpRotor();
  Serial.println("Rotors powered up");
  rotorsPowered = true;
}

void loop(){
    if (rotorsPowered == true){ 
      Serial.println("Reaches main loop");
    }

    else{
      Serial.println("Rotors lost power?");
    }
    
    /*analogWrite(signalPinLB, highDuty);
    delay(5000);
    analogWrite(signalPinLB, lowDuty);
    delay(5000);*/
    
}

void powerUpRotor() { // Power up the rotors, then idle them at low duty cycle. Our ESCs, we believe, go between duty cycle 5% and 10%. Frequency is believed to be 500Hz, which is analogWrite() default.
  long startTime = millis(); // Initialise time at which rotors have started to undergo powerup sequence. Might need to change this to look at start time for individual rotors.
  
  analogWrite(signalPinLB, lowDuty); // Set 12.5% duty cycle PWM wave for rotors.
  analogWrite(signalPinRB, lowDuty);
  analogWrite(signalPinLF, lowDuty);
  analogWrite(signalPinRF, lowDuty);
  
  digitalWrite(13, LOW);
  
  long curTime = millis();
  while(curTime-startTime < 5000){ // Wait 5s at 12.5%.
    curTime = millis();
  }

  digitalWrite(13, LOW);

  analogWrite(signalPinLB, medDuty); // Set 25% duty cycle PWM wave for rotors.
  analogWrite(signalPinRB, medDuty);
  analogWrite(signalPinLF, medDuty);
  analogWrite(signalPinRF, medDuty);

  
  curTime = millis();
  while(curTime-startTime < 10000){ // Wait 5s at 25%.
    curTime = millis();
  }


  /*analogWrite(signalPinLB, lowDuty); // Set 12.5% duty cycle PWM wave for rotors. They will idle at this until we enter the main loop.
  analogWrite(signalPinRB, lowDuty);
  analogWrite(signalPinLF, lowDuty);
  analogWrite(signalPinRF, lowDuty);
  */
  digitalWrite(13, HIGH);

}
