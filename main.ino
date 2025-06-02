#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define col 16
#define lin 2
#define ende 0x27 // Endereço I2C do LCD
#define trigger 5 // Pino Trigger do sensor ultrassônico
#define echo 4    // Pino Echo do sensor ultrassônico
#define ledG 6    // LED Verde
#define ledY 7    // LED Amarelo
#define ledR 8    // LED Vermelho
#define buzzer 2  // Buzzer

LiquidCrystal_I2C lcd(ende, col, lin);

// Variáveis globais para os dados
float distanciaAtual = 0;
String mensagemAlerta = "";

// Variáveis para controle do tempo com millis()
unsigned long previousMillisLeitura = 0;
const long intervaloLeitura = 500; // Reduzido para leituras mais frequentes, ajuste se necessário

unsigned long previousMillisDisplay = 0;
const long intervaloDisplay = 2500; // Intervalo para trocar a tela do LCD (ms)
int estadoDisplay = 0;              // 0 para tela Distancia, 1 para tela Alerta

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Serial.begin(9600);

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("AquaGuard");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(1000); 
}

void loop() {
  unsigned long currentMillis = millis();

  // Bloco para Leitura do Sensor e Lógica de Alerta
  if (currentMillis - previousMillisLeitura >= intervaloLeitura) {
    previousMillisLeitura = currentMillis;

    // 1. Medir a distância
    digitalWrite(trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    
    distanciaAtual = pulseIn(echo, HIGH);
    distanciaAtual = distanciaAtual / 58.0; // converter para cm

    // 2. Determinar o nível de alerta e controlar LEDs/Buzzer
    digitalWrite(buzzer, LOW); 
    if (distanciaAtual >= 100) {
      mensagemAlerta = "Normal";
      digitalWrite(ledG, HIGH); 
      digitalWrite(ledY, LOW); 
      digitalWrite(ledR, LOW);
    } else if (distanciaAtual >= 90) {
      mensagemAlerta = "Cuidado";
      digitalWrite(ledG, LOW); 
      digitalWrite(ledY, HIGH); 
      digitalWrite(ledR, LOW);
    } else if (distanciaAtual >= 80) {
      mensagemAlerta = "Perigo";
      digitalWrite(ledG, LOW); 
      digitalWrite(ledY, LOW); 
      digitalWrite(ledR, HIGH);
    } else { 
      mensagemAlerta = "PERIGO MAXIMO!";
      digitalWrite(ledG, LOW); 
      digitalWrite(ledY, LOW); 
      digitalWrite(ledR, HIGH);
      digitalWrite(buzzer, HIGH);
    }
  }


  // Bloco para Atualização e Alternância do Display LCD
  if (currentMillis - previousMillisDisplay >= intervaloDisplay) {
    previousMillisDisplay = currentMillis;
    estadoDisplay = 1 - estadoDisplay; // Alterna entre 0 e 1

    lcd.clear(); 

    if (estadoDisplay == 0) {
      lcd.setCursor(0,0);
      lcd.print("Distancia: ");
      lcd.setCursor(0,1);
      lcd.print(distanciaAtual, 2);
      lcd.print(" cm");

    } else {
      lcd.setCursor(0, 0);
      lcd.print("Nivel de alerta:");
      
      lcd.setCursor(0, 1);
      lcd.print(mensagemAlerta);
    }
  }
}