#include <Servo.h>
Servo servo;

#define STATE_IDLE 0
#define STATE_EXTENDING 2
#define STATE_RESETTING 3

int extended_pos = 30;
int retracted_pos = 150;
int servo_pos = extended_pos;

int state = STATE_IDLE;
int animation_number = 0;
unsigned long resetting_cooldown_timer = 0;

// Set servo position and remember where the servo was 
void set_servo_pos(int pos) {
  servo_pos = pos;
  servo.write(pos);
}

// Sweep servo to a position with a given step size and step delay
void sweep_servo_at_speed(int target_pos, int step_size, int step_delay) {
  if (servo_pos <= target_pos) {
    for (int i = servo_pos; i <= target_pos; i += step_size) {
      servo.write(i);
      delay(step_delay);
    }
  } else {
    for (int i = servo_pos; i >= target_pos; i -= step_size) {
      servo.write(i);
      delay(step_delay);
    }
  }
  servo.write(target_pos);
  servo_pos = target_pos;
}

void setup() {
  pinMode(9, OUTPUT);
  pinMode(8, INPUT_PULLUP);
  servo.attach(9);
  Serial.begin(9600);
}

void loop() {
  bool switchVal = digitalRead(8);
  Serial.println(switchVal);

  switch (state) {
    case STATE_IDLE: {
      if (switchVal) {
        // If button is pressed, choose an animation and go into the extending state
        state = STATE_EXTENDING;
        animation_number = random(0, 5);
      }
      set_servo_pos(retracted_pos);
      break;
    }
    case STATE_EXTENDING: {
      // Execute the animation that has been randomly chosen
      switch (animation_number) {
        case 0: {
          // Normal open -> hit switch -> close
          delay(500);
          sweep_servo_at_speed(extended_pos, 3, 25);
          delay(100);
          break;
        }
        case 1: {
          // short delay after open
          delay(500);
          sweep_servo_at_speed(extended_pos, 3, 25);
          delay(500);
          break;
        }
        case 2: {
          // long delay after open
          delay(500);
          sweep_servo_at_speed(extended_pos, 3, 25);
          delay(1000);
          break;
        }
        case 3: {
          // fast 
          delay(200);
          sweep_servo_at_speed(extended_pos, 10, 30);
          delay(100);
          break;
        }
        case 4: {
          // wait a bit before starting 
          delay(4000);
          sweep_servo_at_speed(extended_pos, 3, 25);
          delay(100);
          break;
        }
      }
      state = STATE_RESETTING;
      break;
    }
    case STATE_RESETTING: {
      sweep_servo_at_speed(retracted_pos, 10, 40);
      state = STATE_IDLE;
      break;
    }
  }
  delay(10);
}
