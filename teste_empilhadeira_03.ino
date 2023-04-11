/*
  Congraf april 2023
  fork-lift joystck control code. 
  made by Cesar Costa
*/
// variables
int potentiometerX = A0;
int potentiometerY = A1;
int button = A2;
int valueX = 0;
int valueY = 0;
int valueButton = 0;
int valueX_before = 0;
int valueY_before = 0;
int valueButton_before = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillisTime = 0;
// constants
const long interval = 500;
const long time_to_read = 200;  // Time to updated if value change
const int sensitivity = 2;      // lower value is 1
const long value_of_half = (1023 / 2) / sensitivity;
const int tolerance_of_half = 70;  // fine tuning the tolerance in the middle

// ------------***********-------------//

void setup() {
  pinMode(potentiometerX, INPUT_PULLUP);
  pinMode(potentiometerY, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // initialize serial:
  Serial.begin(9600);
}  // end setup

// ------------***********-------------//
void loop() {

  valueButton = digitalRead(button);
  valueButton_before = valueButton;

  valueX = 0;
  valueY = 0;

  // noise reduction
  for (int i = 0; i < 10; i++) {
    valueX += analogRead(potentiometerX);
    valueY += analogRead(potentiometerY);
    delay(1);
  }

  // average of read values
  valueX = valueX / 10;
  valueY = valueY / 10;

  valueX = valueX / sensitivity;
  valueY = valueY / sensitivity;

  // stabilize time between readings
  if (valueX != valueX_before) {
    if (millis() - previousMillis1 >= time_to_read) {
      previousMillis1 = millis();
      valueX_before = valueX;
    }
  } else previousMillis1 = millis();

  if (valueY != valueY_before) {
    if (millis() - previousMillis2 >= time_to_read) {
      previousMillis2 = millis();
      valueY_before = valueY;
    }
  } else previousMillis2 = millis();

  // checks if the joystick is in the middle on the axis X
  if (valueX_before >= value_of_half - tolerance_of_half && valueX_before <= value_of_half + tolerance_of_half) {
    valueX_before = value_of_half;  // adjusts the value to the center
  }

  // checks if the joystick is in the middle on the axis Y
  if (valueY_before >= value_of_half - tolerance_of_half && valueY_before <= value_of_half + tolerance_of_half) {
    valueY_before = value_of_half;  // adjusts the value to the center
  }
  //print values
  if (millis() - previousMillisTime >= interval) {
    previousMillisTime = millis();
    Serial.print(valueX_before, DEC);
    Serial.print(", ");
    Serial.print(valueY_before, DEC);
    Serial.print(", ");
    Serial.print(valueButton_before, DEC);

    if (valueX_before == value_of_half) {
      Serial.print(", ");
      Serial.print("PARADA");
    } else if (valueX_before > value_of_half && valueY_before == value_of_half) {
      Serial.print(", ");
      Serial.print("POS- 1");
    } else if (valueX_before < value_of_half && valueY_before == value_of_half) {
      Serial.print(", ");
      Serial.print("POS- 2");
    } else if (valueX_before > value_of_half && valueY_before > value_of_half) {
      Serial.print(", ");
      Serial.print("POS- 5");
    } else if (valueX_before < value_of_half && valueY_before > value_of_half) {
      Serial.print(", ");
      Serial.print("POS- 6");
    } else if (valueX_before > value_of_half && valueY_before < value_of_half) {
      Serial.print(", ");
      Serial.print("POS- 7");
    } else if (valueX_before < value_of_half && valueY_before < value_of_half) {
      Serial.print(", ");
      Serial.print("POS- 8");
    }
    if (valueButton_before == 0) Serial.print("  .  button press  . ");

    Serial.println(" ");

  }  // end if previusMillisTempo
}  // end loop
