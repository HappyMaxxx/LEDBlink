const int buttonpin1 = 6;
const int buttonpin2 = 7;
const int ledpin = 8;

int val = 0;
bool laststate = false;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
const unsigned long blinkInterval1 = 200;
const unsigned long blinkInterval2 = 400;
bool ledState = false;

String inputString = "";

void setup() {
  Serial.begin(9600);
  pinMode(buttonpin1, INPUT_PULLUP);
  pinMode(buttonpin2, INPUT_PULLUP);
  pinMode(ledpin, OUTPUT);
}

void read_python() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputString.trim(); // очистити пробіли
      // Очікуємо "0", "1", "2", "3"
      if (inputString == "0" || inputString == "1" || inputString == "2" || inputString == "3") {
        val = inputString.toInt();
        ledState = false;
        digitalWrite(ledpin, LOW);
        previousMillis1 = millis();
        previousMillis2 = millis();
      } else if (inputString == "+") {
        val_plus();
      } else if (inputString == "-") {
        val_minus();
      }
      
      inputString = ""; // скидаємо буфер
    } else {
      inputString += c;
    }
  }
}

void val_plus() {
  val = (val + 1) % 4;
  ledState = false;
  digitalWrite(ledpin, LOW);
  previousMillis1 = millis();
  previousMillis2 = millis();
}

void val_minus() {
  if (not ((val-1) < 0)) {
    val = (val - 1) % 4;
    } else {
    val = 3;
  }
  ledState = false;
  digitalWrite(ledpin, LOW);
  previousMillis1 = millis();
  previousMillis2 = millis();
  delay(200);
}

void loop() {
  // --- 1. Читання даних з комп'ютера ---
  read_python();

  // --- 2. Обробка кнопки ---
  bool currentState1 = digitalRead(buttonpin1) == LOW;
  bool currentState2 = digitalRead(buttonpin2) == LOW;
  
  if (currentState1 && !laststate) {
    val_plus();
    delay(200);
  }

  if (currentState2 && !laststate) {
    val_minus();
    delay(200);
  }
  
  laststate = max(currentState1, currentState2);

  // --- 3. Режими ---
  unsigned long currentMillis = millis();

  switch (val) {
    case 0:
      digitalWrite(ledpin, LOW);
      break;
    case 1:
      digitalWrite(ledpin, HIGH);
      break;
    case 2:
      if (currentMillis - previousMillis1 >= blinkInterval1) {
        previousMillis1 = currentMillis;
        ledState = !ledState;
        digitalWrite(ledpin, ledState);
      }
      break;
    case 3:
      if (currentMillis - previousMillis2 >= blinkInterval2) {
        previousMillis2 = currentMillis;
        ledState = !ledState;
        digitalWrite(ledpin, ledState);
      }
      break;
  }

  Serial.println(val);
}
