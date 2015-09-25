const bool DEBUG = false;

// Strategy:
// We read a value once a second
// If the value has dropped significantly from a second ago,
// tell the computer

const int dropThreshold = 30;

const int readings = 16;
int previousReading = -1;

/*=============== Crystal Ball sensor ====================*/
const int crystalBallPin = 1; //analog pin 0

void setup() {
}

void read_crystal_ball_sensor() {
  int cb_read = 0;
  
  for(int i = 0; i < readings; i++) {
    cb_read += analogRead(crystalBallPin);
    delay(10);
    
  }

  if (DEBUG) {
    Serial.print("reading = ");
    Serial.println(cb_read);
    Serial.print("previous reading = ");
    Serial.println(previousReading);
  }

  if (previousReading != -1 && cb_read < previousReading) {
    int scaled = map(previousReading - cb_read, 0, 1023*readings/8, 0, 255);
    if (DEBUG) {
      Serial.print("scaled difference = ");
      Serial.println(scaled);
    } else {
      Serial.write(scaled);
    }
  } else {
    Serial.write(0);
  }

  previousReading = cb_read;

  delay(1000);
}

void loop() {
  read_crystal_ball_sensor();
}
