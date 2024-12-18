#define PIN_RELAY 3 // Определяем пин, используемый для подключения реле
#define PIN_POT A6 // A0 определеяем пин для потенциометра 
#define PIN_TEMP A1 // A1 определеяем пин для термистора 
#define PIN_BUT 12 // 8 определеяем пин для кнопки

#define PIN_LED_1 4
#define PIN_LED_2 7
#define PIN_LED_3 9
#define PIN_LED_4 11

#define B 3950 // ntc коэффициент
#define RESISTOR 100000 // сопротивление резистора в омах
#define THERMISTOR 10000 // сопротивление термистора в омах
#define NOMINAL 25 // номинальная температура


void setup()
{
  Serial.begin(9600);
  pinMode(PIN_RELAY, OUTPUT); // Объявляем пин реле как выход
  pinMode(PIN_POT, INPUT); // Объявляем пин потенциометра как вход
  pinMode(PIN_TEMP, INPUT); // Объявляем пин термистора как вход
  pinMode(PIN_BUT, INPUT); // Объявляем пин кнопки как вход

  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);

  digitalWrite(PIN_RELAY, HIGH); // Выключаем реле - посылаем высокий сигнал
}


float signal, temp_cr;
int state = 0;



void loop()
{
  // ищем какая температура критическая
  signal = analogRead(PIN_POT);
  temp_cr = map(signal, 0, 1023, 20, 80);

  int buttonState = digitalRead(PIN_BUT);  

  if (buttonState == HIGH) {   
    if (state == 0){
      Serial.println("button");
      state = 1;
      digitalWrite(PIN_RELAY, LOW);
    }
    else{
      state = 0;
      digitalWrite(PIN_RELAY, HIGH);
    }
  }

  // контроль температуры стакана
  int t = analogRead(PIN_TEMP); // считываем напряжение
  float tr = 1023.0 / t - 1;
  tr = RESISTOR / tr;
 
  float steinhart;
  steinhart = tr / THERMISTOR;
  steinhart = log(steinhart);
  steinhart /= B;
  steinhart += 1.0 / (NOMINAL + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15; 
  // Serial.println(steinhart);
  
  if (steinhart >= 90 || steinhart > (temp_cr + 10)){
    digitalWrite(PIN_RELAY, HIGH);
    state = 0;
  }
  
  //Serial.println(steinhart);
  if (steinhart < 40){
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, LOW);
    digitalWrite(PIN_LED_3, LOW);
    digitalWrite(PIN_LED_4, LOW);
  }
  else if (steinhart >= 40 && steinhart < 55){
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, LOW);
    digitalWrite(PIN_LED_3, LOW);
    digitalWrite(PIN_LED_4, LOW);
  }
  else if (steinhart >= 55 && steinhart < 70){
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
    digitalWrite(PIN_LED_3, LOW);
    digitalWrite(PIN_LED_4, LOW);
  }
  else if (steinhart >= 70 && steinhart < 80){
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
    digitalWrite(PIN_LED_3, HIGH);
    digitalWrite(PIN_LED_4, LOW);
  }
  else{
    Serial.println("here");
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
    digitalWrite(PIN_LED_3, HIGH);
    digitalWrite(PIN_LED_4, HIGH);
  }

  Serial.println(buttonState);
  
  delay(100); // Проверяем значения 10 раз в секунду.

}
