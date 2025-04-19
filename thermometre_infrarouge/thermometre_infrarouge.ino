#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

const int bouton = 9;

void setup() {
  pinMode(bouton, INPUT);
  Wire.begin();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Init...");
  mlx.begin();
  delay(1000);
  ShowTemp(-1);
}

void loop() {
  static bool lastState = LOW;
  bool state = digitalRead(bouton);
  if (state == HIGH && lastState == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mesure...");
    float temperature = GetTemp();
    ShowTemp(temperature);
  }

  lastState = state;
}

float GetTemp() {
  float temp = 0;
  int compteur = 0;
  for (int i = 0; i < 25; i++) {
    float t = mlx.readObjectTempC();
    if (!isnan(t)){
      temp += t;
      compteur += 1;
      delay(50);
    }
  }
  return temp / compteur;
}

void ShowTemp(float temperature) {
  lcd.clear();
  if (temperature == -1) {
    lcd.setCursor(0, 0);
    lcd.print("Pret !");
  } else if (temperature == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Erreur !");
    delay(3000);
    ShowTemp(-1);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Temp :");
    lcd.setCursor(0, 1);
    lcd.print(temperature, 1);
    lcd.print(" C");
    delay(3000);
    ShowTemp(-1);
  }
}