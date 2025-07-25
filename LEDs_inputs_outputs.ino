// Переменные для кнопки
int switchState = 0;
int lastSwitchState = 0;
bool isSequenceMode = false; // false - мигание синего, true - последовательность

// Переменные для мигания синего LED
unsigned long previousMillis = 0;
const long blinkInterval = 500;
bool blueLedState = false;

// Переменные для затухания
unsigned long fadeMillis = 0;
int fadeValue = 0;
int fadeStep = 5;
bool increasing = true;

int currentLED = 0; // 0 - желтый, 1 - красный, 2 - голубой
const long fadeInterval = 10;

void setup() {
  pinMode(3, OUTPUT); // Желтый
  pinMode(4, OUTPUT); // Красный
  pinMode(5, OUTPUT); // Голубой
  pinMode(2, INPUT);  // Кнопка
}

void loop() {
  switchState = digitalRead(2);

  // Детект нажатия кнопки (только при переходе с LOW на HIGH)
  if (switchState == HIGH && lastSwitchState == LOW) {
    isSequenceMode = !isSequenceMode; // Переключаем режим
    // Сброс переменных
    previousMillis = millis();
    fadeMillis = millis();
    fadeValue = 0;
    increasing = true;
    currentLED = 0;

    // Гасим все LED
    analogWrite(3, 0);
    analogWrite(4, 0);
    analogWrite(5, 0);

    delay(50); // Антидребезг
  }

  lastSwitchState = switchState;

  if (!isSequenceMode) {
    // Режим: мигание синего LED
    unsigned long currentMillis = millis();

    // Гасим другие LED
    analogWrite(3, 0);
    analogWrite(4, 0);

    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      blueLedState = !blueLedState;
      digitalWrite(5, blueLedState ? HIGH : LOW);
    }
  } else {
    // Режим: плавное затухание LED по очереди
    unsigned long currentMillis = millis();

    // Гасим синий LED
    digitalWrite(5, LOW);

    if (currentMillis - fadeMillis >= fadeInterval) {
      fadeMillis = currentMillis;

      int ledPins[3] = {3, 4, 5};
      int activePin = ledPins[currentLED];

      if (increasing) {
        fadeValue += fadeStep;
        if (fadeValue >= 255) {
          fadeValue = 255;
          increasing = false; // Начинаем затухать
        }
      } else {
        fadeValue -= fadeStep;
        if (fadeValue <= 0) {
          fadeValue = 0;
          increasing = true;

          // Переход к следующему LED
          currentLED++;
          if (currentLED > 2) {
            currentLED = 0;
          }
        }
      }

      // Гасим все
      analogWrite(3, 0);
      analogWrite(4, 0);
      analogWrite(5, 0);

      // Управляем текущим LED
      analogWrite(activePin, fadeValue);
    }
  }
}