
#define SPEED_PIN_A 3
#define DIR_PIN_A 2

#define SPEED_FULL   0xFF
#define SPEED_MEDIUM 0x10
#define SPEED_LOW    0x08
#define SPEED_STOP   0x00


#define DIR_FORWARD   HIGH
#define DIR_BACKWARDS LOW

#define FORWARD() go(SPEED_FULL, DIR_FORWARD)
#define BACKWARDS() go(SPEED_FULL, DIR_BACKWARDS)
#define STOP() go(SPEED_STOP, DIR_BACKWARDS)

char cmd;

void go(uint8_t speed, bool direction) {
  /* We need to stop the motor to stop first before changing direction */
  analogWrite(SPEED_PIN_A, 0); 
  delay(50);
  
  digitalWrite(DIR_PIN_A, direction);  
  analogWrite(SPEED_PIN_A, speed);  //set motor to run at (100/255 = 39)% duty cycle (slow)
}


void setup()
{
  pinMode(SPEED_PIN_A, OUTPUT);  //Set control pins to be outputs
  pinMode(DIR_PIN_A, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  while (!Serial.available()) ;
  cmd = Serial.read();        // read next available byte


  if( cmd == '1' ) {
    FORWARD();
    cmd = '\0';
  } else if( cmd == '2' ) {
    BACKWARDS();
    cmd = '\0';
  } else if( cmd == '3' ) {
    STOP();
    cmd = '\0';
  }
  //delay(1000);
  
}


