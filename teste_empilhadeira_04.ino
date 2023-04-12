/*
  Congraf april 2023
  fork-lift joystck control code. 
  made by Cesar Costa
*/
// variables

int verical_axis_output = 5;  // wire #32  980hz pin 5 and 6 - 490hz pin 2, 3, 10 and 11
int horizontal_axis_output = 6; // wire #34
int outputY;
int outputX;
int potentiometerY = A0;
int potentiometerX = A1;
int button = A2;
int valueY = 0;
int valueX = 0;
int valueButton = 0;
int valueY_before = 0;
int valueX_before = 0;
int valueButton_before = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillisTime = 0;
// constants
const long interval = 100;      // time to print value
const long time_to_read = 150;  // Time to updated if value change
const int sensitivity = 1;      // lower value is 1
const long value_of_half = (1023 / 2) / sensitivity;
const int tolerance_of_half = 70;  // fine tuning the tolerance in the middle
const int output_middle=127; //middle of PWM 


// ------------***********-------------//

void setup() {
  pinMode(potentiometerY, INPUT_PULLUP);
  pinMode(potentiometerX, INPUT_PULLUP);
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

  valueY = 0;
  valueX = 0;

  // noise reduction
  for (int i = 0; i < 10; i++) {
    valueY += analogRead(potentiometerY);
    valueX += analogRead(potentiometerX);
    delay(1);
  }

  // average of read values
  valueY = valueY / 10;
  valueX = valueX / 10;

  valueY = valueY / sensitivity;
  valueX = valueX / sensitivity;

  // stabilize time between readings
  if (valueY != valueY_before) {
    if (millis() - previousMillis1 >= time_to_read) {
      previousMillis1 = millis();
      valueY_before = valueY;
    }
  } else previousMillis1 = millis();

  if (valueX != valueX_before) {
    if (millis() - previousMillis2 >= time_to_read) {
      previousMillis2 = millis();
      valueX_before = valueX;
    }
  } else previousMillis2 = millis();

  // checks if the joystick is in the middle on the axis Y
  if (valueY_before >= value_of_half - tolerance_of_half && valueY_before <= value_of_half + tolerance_of_half) {
    valueY_before = value_of_half;  // adjusts the value to the center
  }
  // map the value to PWM range axis Y
  outputY= map(valueY_before, 0, 1023 / sensitivity, 0, 255);
  // checks if the joystick is in the middle on the axis X
  if (valueX_before >= value_of_half - tolerance_of_half && valueX_before <= value_of_half + tolerance_of_half) {
    valueX_before = value_of_half;  // adjusts the value to the center
  }
  //map the value to PWM range axis X
  outputX = map(valueX_before, 0, 1023 / sensitivity, 0, 255);

//write PWM
 analogWrite(verical_axis_output,outputY);
 analogWrite(horizontal_axis_output,outputX);

    //print values
    if (millis() - previousMillisTime >= interval) {
    previousMillisTime = millis();
    Serial.print(outputY, DEC);
    Serial.print(", ");
    Serial.print(outputX, DEC);
    Serial.print(", ");
    Serial.print(valueButton_before, DEC);

    if (outputY == output_middle) {
      Serial.print(", ");
      Serial.print("PARADA");
    } else if (outputY > output_middle && outputX == output_middle) {
      Serial.print(", ");
      Serial.print("POS- 1");
    } else if (outputY < output_middle && outputX == output_middle) {
      Serial.print(", ");
      Serial.print("POS- 2");
    } else if (outputY > output_middle && outputX > output_middle) {
      Serial.print(", ");
      Serial.print("POS- 5");
    } else if (outputY < output_middle && outputX > output_middle) {
      Serial.print(", ");
      Serial.print("POS- 6");
    } else if (outputY > output_middle && outputX < output_middle) {
      Serial.print(", ");
      Serial.print("POS- 7");
    } else if (outputY < output_middle && outputX < output_middle) {
      Serial.print(", ");
      Serial.print("POS- 8");
    }
    if (valueButton_before == 0) Serial.print("  .  button press  . ");

    Serial.println(" ");

  }  // end if previusMillisTempo
}  // end loop
