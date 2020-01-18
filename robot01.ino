/****************************/
/*    PIN assignments       */
/****************************/
const int SPEED_PIN_A  = 3;
const int DIR_PIN_A = 2;

const int SPEED_PIN_B = 10;
const int DIR_PIN_B = 12;

const int LIGHT_SENSE_1 = A0;
const int LIGHT_SENSE_2 = A1;

/****************************/
/*    Motor part            */
/****************************/
const char SPEED_FULL   = 0xFF;
const char SPEED_MEDIUM = 0x10;
const char SPEED_LOW    = 0x08;
const char SPEED_STOP   = 0x00;

const bool DIR_FORWARD   = HIGH;
const bool DIR_BACKWARDS = LOW;

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

#define FORWARD(M) go((M), SPEED_FULL, DIR_FORWARD)
#define BACKWARDS(M) go((M), SPEED_FULL, DIR_BACKWARDS)
#define STOP(M) go((M), SPEED_STOP, DIR_BACKWARDS)

#define STOP_A() STOP(MOTOR_A)
#define STOP_B() STOP(MOTOR_B)

#define FORWARD_A() FORWARD(MOTOR_A)
#define FORWARD_B() FORWARD(MOTOR_B)

#define BACKWARDS_A() BACKWARDS(MOTOR_A)
#define BACKWARDS_B() BACKWARDS(MOTOR_B)

dir_t current_direction;
dir_t new_direction;

void go(int motor, uint8_t speed, bool direction) {
  /* We need the motor to stop first before changing direction */
  analogWrite(motors[motor].speed_pin, 0); 
  delay(50);
  
  digitalWrite(motors[motor].dir_pin, direction);  
  analogWrite(motors[motor].speed_pin, speed);  //set motor to run at (100/255 = 39)% duty cycle (slow)
}

void set_motors_to_new_direction()
{
  if(new_direction != current_direction)
  {
    current_direction = new_direction;
    switch(new_direction){
      case STRAIGHT:
      {
        FORWARD_B();
        FORWARD_A();
      }
      break;
      case LEFT:
      {
        STOP_A();
        FORWARD_B();
      }
      break;
      case RIGHT:
      {
        STOP_B();
        FORWARD_A();
      }
      break;
    }
  }
}

void print_current_direction()
{
  switch(new_direction){
    case STRAIGHT:
    {
      Serial.println("\t Go Straight");
    }
    break;
    case LEFT:
    {
      Serial.println("\t Turn direction A");
    }
    break;
    case RIGHT:
    {
      Serial.println("\t Turn direction B");
    }
    break;
  }
}
/****************************/
/*    LDR sensor part       */
/****************************/

const int hysteresis = 30;

int sensor1Value;
int sensor2Value;

void read_light_sensors()
{
  sensor1Value = analogRead(LIGHT_SENSE_1);
  sensor2Value = analogRead(LIGHT_SENSE_2);
}

void print_light_values_to_serial()
{
  Serial.print("sensor1 = ");
  Serial.print(sensor1Value);
  
  Serial.print("\t sensor2 = ");
  Serial.println(sensor2Value);
}

void decide_new_direction()
{
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
}

void wait_a_little()
{
  delay(100);
}

void init_globals()
{
  sensor1Value = 0;
  sensor2Value = 0;
  current_direction = STRAIGHT;
  new_direction = STRAIGHT;
}

void init_pins()
{
  pinMode(SPEED_PIN_A, OUTPUT);
  pinMode(DIR_PIN_A, OUTPUT);

  pinMode(SPEED_PIN_B, OUTPUT);
  pinMode(DIR_PIN_B, OUTPUT);

  Serial.begin(9600);
  
  pinMode(LIGHT_SENSE_1, INPUT);
  pinMode(LIGHT_SENSE_2, INPUT); 
}

void setup()
{
  init_pins();
  init_globals();
}

void loop()
{
  read_light_sensors();
  
  print_light_values_to_serial();
  
  decide_new_direction();

  set_motors_to_new_direction();

  print_current_direction();
  
  wait_a_little();
}
