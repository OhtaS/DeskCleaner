#define M1_S 5 // motor1 speed pin
#define M1_D 4 // motor1 direction pin
#define M2_S 6 // motor2 speed pin
#define M2_D 7 // motor2 direction pin
#define CLEANER 8 // cleaner pin
#define SENSOR_B 11 // back sensor analog pin
#define SENSOR_R 12 // right sensor analog pin
#define SENSOR_L 13 // left sensor analog pin

enum State {Forward, Backward, Rotate, Warning, Stop} state;

void check_state();
void rotate_right(int analog_speed);
void rotate_left(int analog_speed);
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
  pinMode(CLEANER, OUTPUT);
  pinMode(SENSOR_B, INPUT);
  pinMode(SENSOR_R, INPUT);
  pinMode(SENSOR_L, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(CLEANER, HIGH);
  check_state();
  switch (state) {
    case Forward:
      Serial.print("run forward : ");
      //      run_forward(28);
      run_forward(38);
      break;

    case Backward:
      Serial.print("run backward : ");
      //      run_backward(28);
      run_backward(38);

      is_return = true;
      break;

    case Rotate:
      Serial.print("rotate : ");
      //      rotate_right(35);
      rotate_right(45);
      break;

    case Warning:
      Serial.print("WARNING ZONE : ");
      //      move_to_safe_zone(120);
      move_to_safe_zone(120);
      break;

    case Stop:

      break;
  }

  Serial.print("R : ");
  Serial.print(digitalRead(SENSOR_R));
  Serial.print(", L : ");
  Serial.println(digitalRead(SENSOR_L));
  delay(10);
}

void rotate_right(int analog_speed) {
  static int count = 0;
  turn_motor1(analog_speed + 2, HIGH);
  turn_motor2(analog_speed, LOW);
  count++;
  if (count > 40) {
    is_return = false;
    is_rotate = false;
    count = 0;
  }
}

void rotate_left(int analog_speed) {
  static int count = 0;
  turn_motor1(analog_speed, LOW);
  turn_motor2(analog_speed, HIGH);
  count++;
  if (count > 40) {
    is_return = false;
    is_rotate = false;
    count = 0;
  }
}

void move_to_safe_zone(int analog_speed) {
  static int count = 0;
  turn_motor1(analog_speed, LOW);
  turn_motor2(analog_speed, LOW);
}

void run_forward(int analog_speed) {
  static int count = 0;
  turn_motor1(analog_speed, HIGH);
  turn_motor2(analog_speed + 2, HIGH);
  count++;
  if (count > 280) {
    is_return = false;
    is_rotate = true;
    count = 0;
  }
}

void run_backward(int analog_speed) {
  static int count = 0;
  turn_motor1(analog_speed, LOW);
  turn_motor2(analog_speed, LOW);
  count++;
  if (count > 100 || digitalRead(SENSOR_B) == LOW) {
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

void check_state() {
  static boolean right_sensor = LOW;
  static boolean left_sensor = LOW;
  right_sensor = digitalRead(SENSOR_R);
  left_sensor = digitalRead(SENSOR_L);

  if (is_rotate == true || state == Backward) {
    state = Rotate;
  } else if (right_sensor == LOW || left_sensor == LOW ) {
    state = Warning;
    is_return = true;
  } else if (right_sensor == HIGH && left_sensor == HIGH && is_return == true) {
    state = Backward;
  } else if (right_sensor == HIGH && left_sensor == HIGH && is_return == false || digitalRead(SENSOR_B) == LOW) {
    state = Forward;
  }
}
