/****************************/
/*    PIN assignments       */
/****************************/
#define SPEED_PIN_A 3
#define DIR_PIN_A 2

#define SPEED_PIN_B 10
#define DIR_PIN_B 12

#define LIGHT_SENSE_1 A0
#define LIGHT_SENSE_2 A1

/****************************/
/*    Motor part            */
/****************************/
#define SPEED_FULL   0xFF
#define SPEED_MEDIUM 0x10
#define SPEED_LOW    0x08
#define SPEED_STOP   0x00

#define DIR_FORWARD   HIGH
#define DIR_BACKWARDS LOW

#define FORWARD(M) go((M), SPEED_FULL, DIR_FORWARD)
#define BACKWARDS(M) go((M), SPEED_FULL, DIR_BACKWARDS)
#define STOP(M) go((M), SPEED_STOP, DIR_BACKWARDS)

typedef struct {
  int dir_pin;
  int speed_pin;
}motor_t;

motor_t motors[2] = {
  { DIR_PIN_A, SPEED_PIN_A },
  { DIR_PIN_B, SPEED_PIN_B }  
};

enum {
  MOTOR_A,
  MOTOR_B
};

typedef enum {
  STRAIGHT,
  LEFT,
  RIGHT
}dir_t;

#define STOP_A() STOP(MOTOR_A)
#define STOP_B() STOP(MOTOR_B)

#define FORWARD_A() FORWARD(MOTOR_A)
#define FORWARD_B() FORWARD(MOTOR_B)

#define BACKWARDS_A() BACKWARDS(MOTOR_A)
#define BACKWARDS_B() BACKWARDS(MOTOR_B)

void go(int motor, uint8_t speed, bool direction) {
  /* We need the motor to stop first before changing direction */
  analogWrite(motors[motor].speed_pin, 0); 
  delay(50);
  
  digitalWrite(motors[motor].dir_pin, direction);  
  analogWrite(motors[motor].speed_pin, speed);  //set motor to run at (100/255 = 39)% duty cycle (slow)
}

/****************************/
/*    LDR sensor part       */
/****************************/

const int hysteresis = 30;

int sensor1Value;
int sensor2Value;
dir_t current_direction = STRAIGHT;
dir_t new_direction = STRAIGHT;

void setup()
{
  pinMode(SPEED_PIN_A, OUTPUT);
  pinMode(DIR_PIN_A, OUTPUT);

  pinMode(SPEED_PIN_B, OUTPUT);
  pinMode(DIR_PIN_B, OUTPUT);

  Serial.begin(9600);
  
  pinMode(LIGHT_SENSE_1, INPUT);
  pinMode(LIGHT_SENSE_2, INPUT);
  sensor1Value = 0;
  sensor2Value = 0;
}

void loop()
{
  sensor1Value = analogRead(LIGHT_SENSE_1);
  sensor2Value = analogRead(LIGHT_SENSE_2);
  
  // print the results to the Serial Monitor:
  Serial.print("sensor1 = ");
  Serial.print(sensor1Value);
  
  Serial.print("\t sensor2 = ");
  Serial.print(sensor2Value);
  int sensorDiff = sensor1Value - sensor2Value;

  if(sensorDiff < -hysteresis)
  {
    new_direction = RIGHT;
  }
  else if(sensorDiff > hysteresis)
  {
    new_direction = LEFT;
  }
  else
  {
    new_direction = STRAIGHT;
  }

  if(new_direction != current_direction)
  {
    current_direction = new_direction;
    switch(new_direction){
      case STRAIGHT:
      {
        FORWARD_B();
        FORWARD_A();
        Serial.print("\t Go Straight");
      }
      break;
      case LEFT:
      {
        STOP_A();
        FORWARD_B();
        Serial.print("\t Turn direction A");
      }
      break;
      case RIGHT:
      {
        STOP_B();
        FORWARD_A();
        Serial.print("\t Turn direction B");
      }
      break;
    }
  }
  Serial.println(".");
  delay(100);
}
