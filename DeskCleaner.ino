#define M1_S 5 // motor1 speed
#define M1_D 4 // motor1 direction
#define M2_S 6 // motor2 speed
#define M2_D 7 // motor2 direction
#define SENSOR_1 0 // distance sensor

void rotate(int analog_speed);
void run_forward(int analog_speed);
void run_backward(int analog_speed);
void turn_motor1(int analog_speed, boolean is_positive);
void turn_motor2(int analog_speed, boolean is_positive);

boolean is_return = false;
boolean is_rotate = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(M1_S, OUTPUT); // 速度 (Analog)
  pinMode(M1_D, OUTPUT); // 方向 (HIGH/LOW)
  pinMode(M2_S, OUTPUT); // 速度 (Analog)
  pinMode(M2_D, OUTPUT); // 方向 (HIGH/LOW)
  pinMode(SENSOR_1, INPUT);
  Serial.begin(9600);
}

void loop() {
  static int sensor_val = 0;
  sensor_val = analogRead(SENSOR_1);
  if (is_rotate == true) {
    rotate(30);
  } else if (sensor_val > 500 && is_return == false) {
    run_forward(30);
  } else {
    run_backward(30);
    is_return = true;
  }
  //  if (is_rotate == true) {
  //    rotate_count++;
  //    if (rotate_count > 50) {
  //      is_rotate = false;
  //      rotate_count = 0;
  //    }
  //  }

  //  if (is_return == true) {
  //    count++;
  //    if (count > 80) {
  //      is_return = false;
  //      is_rotate = true;
  //      count = 0;
  //    }
  //}

  delay(10);
  Serial.println(sensor_val);
}

void rotate(int analog_speed) {
  static int count = 0;
  turn_motor1(analog_speed, HIGH);
  turn_motor2(analog_speed, LOW);
  count++;
  if (count > 80) {
    is_rotate = false;
    count = 0;
  }
}

void run_forward(int analog_speed) {
  turn_motor1(analog_speed, LOW);
  turn_motor2(analog_speed, LOW);
}

void run_backward(int analog_speed) {
  static int count = 0;
  turn_motor1(analog_speed, HIGH);
  turn_motor2(analog_speed, HIGH);
  count++;
  if (count > 80) {
    is_return = false;
    is_rotate = true;
    count = 0;
  }
}

void turn_motor1(int analog_speed, boolean is_positive) {
  digitalWrite(M1_D, is_positive);
  analogWrite(M1_S, analog_speed);
}

void turn_motor2(int analog_speed, boolean is_positive) {
  digitalWrite(M2_D, is_positive);
  analogWrite(M2_S, analog_speed);
}
