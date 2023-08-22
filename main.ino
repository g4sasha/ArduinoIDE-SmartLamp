// Имена для всех элементов
#define _lamp     3
#define _light    A0
#define _pot      A1
#define _btn      10
#define _led      5

// Сегменты экрана
#define M1  11
#define M2  4
#define M3  12
#define M4  2
#define M5  13
#define M6  A4
#define M7  A5

// Переменные по умолчанию
boolean btn_flag = 0;
int state_number = 0;
float i = 0.00;

void setup()
{
  // Устанавливаем режим выхода кнопке и светильникам
  pinMode(_lamp, OUTPUT);
  pinMode(_led, OUTPUT);
  pinMode(_btn, INPUT_PULLUP);

  // Устанавливаем режим выхода сегментам экрана
  // Кроме M6 и M7, т.к они установлены на A4 и A5
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  pinMode(M5, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  // Обновляемые переменные
  int light = analogRead(_light);
  int pot = analogRead(_pot);
  boolean btn = !digitalRead(_btn);

  // Проверка состояния "0"
  // Выводит на экран цифру 0
  // В будущем проверяется для выключения схемы
  if(state_number == 0)
  {
    digitalWrite(M1, 1);
    digitalWrite(M2, 1);
    digitalWrite(M3, 1);
    digitalWrite(M4, 0);
    digitalWrite(M5, 1);
    analogWrite(M6, 256);
    analogWrite(M7, 256);
  }

  // Проверка состояния "1"
  // Выводит на экран цифру 1
  // В будущем используется для первого решима с включением в темноте
  if(state_number == 1)
  {
    digitalWrite(M1, 0);
    digitalWrite(M2, 0);
    digitalWrite(M3, 1);
    digitalWrite(M4, 0);
    digitalWrite(M5, 0);
    analogWrite(M6, 256);
    analogWrite(M7, 0);
  }

  // Проверка состояния "2"
  // Выводит на экран цифру 2
  // В будущем используется для второго режима с включением при свете
  if(state_number == 2)
  {
    digitalWrite(M1, 1);
    digitalWrite(M2, 0);
    digitalWrite(M3, 1);
    digitalWrite(M4, 1);
    digitalWrite(M5, 1);
    analogWrite(M6, 0);
    analogWrite(M7, 256);
  }

  // Проверка состояния "3"
  // Выводит на экран цифру 3
  // В будущем используется для третьего режима "Плавное мигание"
  if(state_number == 3)
  {
    digitalWrite(M1, 1);
    digitalWrite(M2, 0);
    digitalWrite(M3, 1);
    digitalWrite(M4, 1);
    digitalWrite(M5, 0);
    analogWrite(M6, 256);
    analogWrite(M7, 256);
  }

  // При нажатии на кнопку:
  if(btn == 1 and btn_flag == 0)
  {
    btn_flag = 1;                               // Переменная, чтобы нажатие кнопки не фиксировалась каждый оборот в цикле
    state_number += 1;                          // Переключаем на следующее состояние
    if(state_number > 3) { state_number = 0; }  // Не даём переключиться на состояние, которого не существует
    delay(50);                                  // Задержка для избежания дребезгов кнопки
  }

  // Когда кнопку отпустили:
  if(btn == 0 and btn_flag == 1)
  {
    btn_flag = 0;                                // Переменная, чтобы нажатие кнопки не фиксировалась каждый оборот в цикле
    delay(50);                                   // Задержка для избежания дребезгов кнопки
  }

  // Первое состояние (свет выключается когда светло)
  if(state_number == 1)
  {
    if(pot >= light)
    {
      digitalWrite(_lamp, 1);
    }
  
    else
    {
      digitalWrite(_lamp, 0);
    }
  }

  // Второе состояние (свет выключается когда темно)
  if(state_number == 2)
  {
    if(pot <= light)
    {
      digitalWrite(_lamp, 1);
    }
  
    else
    {
      digitalWrite(_lamp, 0);
    }
  }

  // Третье состояние (плавное мигание света)
  if(state_number == 3)
  {
    analogWrite(_lamp, (sin(i)+1)/2*256);
    i += 0.0005;
    if(i >= 10000) { i = 0; }
    // Строчка выше нужна, чтобы при долгой работе переменная не уходила далеко
  }

  // Состояние выключенной схемы
  if(state_number == 0)
  {
    digitalWrite(_lamp, 0);
  }

  // Синий светодиод для красоты. Горит всегда, кроме состояния "0"
  digitalWrite(_led, !state_number == 0);

  // Serial.println(String() + " <=> ");
}
