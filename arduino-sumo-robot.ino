// Pins from L298N
#define IN1 5
#define IN2 4
#define IN3 3
#define IN4 2
#define ENA 10
#define ENB 11
// Pins from tcs3200 
#define SOS2 6 
#define S1S3 7 
#define tcsout1 8
#define tcsout2 9
// Pins from HC-SR04
#define trig1 A0
#define echo1 A1
#define trig2 A2
#define echo2 A3

// variables from tcs3200
int color1 = 0;
int color2 = 0;

// variables from HC-SR04
long duration1, distance1, duration2, distance2;

//variable for the search algorithm
int searchcycle = 0;

void setup() {
  // declaring pinModes
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(SOS2, OUTPUT);
  pinMode(S1S3, OUTPUT);
  pinMode(tcsout1, INPUT);
  pinMode(tcsout2, INPUT);

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);  

  // Setting frequency-scaling to 20% and setting clear photodiode to be read on the tcs3200
  digitalWrite(SOS2, HIGH);
  digitalWrite(S1S3, LOW);

  // Set motor's speed to 200/255
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void loop() {
  // TCS3200 *****
  // Reading tcs3200 output frequencies (colors)
  color1 = pulseIn(tcsout1, LOW);
  color2 = pulseIn(tcsout2, LOW);
 
  if (color2 < 20) { // If light is sensed in the back, go FORWARD
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);    
    
  }
  else if (color1 < 20) { // If light is sensed in the front, go BACKWARD
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);    
  }
  // HC-SR04 *****
  else { 
    // Calculate distance of sensor 1
    digitalWrite(trig1, LOW);
    delayMicroseconds(2);
    digitalWrite(trig1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig1, LOW);
    duration1 = pulseIn(echo1, HIGH);
    distance1 = duration1 / 58.2;

    // Calculate distance of sensor 2
    digitalWrite(trig2, LOW);
    delayMicroseconds(2);
    digitalWrite(trig2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig2, LOW);
    duration2 = pulseIn(echo2, HIGH);
    distance2 = duration2 / 58.2;  

    if (distance1 < 40) { // If distance from front sensor is less than 50, go FORWARD
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);    
    }
    else if (distance2 < 40) { // If distance from front sensor is less than 50, go BACKWARD
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);    
    }
    // SEARCH 
    else { 
      if (searchcycle < 15) { // Go forward 15 cycles (around 20 cycles in a sec)
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);   
      }
      else if (searchcycle <= 20) { // Rotate right 5 cycles
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH); 
      }
      if (searchcycle < 20) {
        searchcycle += 1;    
      }
      else if (searchcycle == 20) {
        searchcycle = 0;
      }      
    } 
  }
  delay(50); // This + the time that the above takes is a cycle.
}



