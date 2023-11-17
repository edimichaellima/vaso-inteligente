#include <LiquidCrystal.h>
#include <dht.h>

//Pino Sensor de Umidade
const int sensor_umidade = 0;

//Pino Bomba D'água 
const int motor = 13;

int ValorLDR; //Armazenar a leitura do sensor LDR
int IntensidadeLuz; //Transforma a leitura em uma escala de 0 a 100
float pinoLDR = A1; //PINO ANALÓGICO utilizado para ler o LDR

//LCD
const int rs = 3, en = 2, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int LM35 = A2; //define o pino que lera a saida do LM35

dht DHT;

#define DHT11_PIN 8

void setup() {
  Serial.begin(9600);
  
  //Definir pinos do motor
  pinMode(motor, OUTPUT);
  
  //Definir LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INICIANDO...");
  delay (2000);
  lcd.clear(); 
  
  //Pino de temperatura
  pinMode(LM35,INPUT);
  	
  pinMode(pinoLDR, INPUT); //Define o pino que o LDR está ligado como entrada de dados

}

void loop() {
	
  //Ler do sinal do sensor
  float sinal = analogRead(sensor_umidade);
  
  //Transformar o valor analógico recebido em porcentagem
  int umidade = map(sinal, 0, 1023, 0, 100);
  
  Serial.print("Umidade do solo (%): ");
  Serial.println(umidade);
  
  //Saida para o LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UMIDADE: ");
  lcd.print(umidade);
  lcd.print("%");
  lcd.setCursor(0, 1);
  
  if(umidade < 80) { 
    //Desligar sistema de irrigação
  	digitalWrite(motor, LOW);
    Serial.println("Motor desligado");

    //Saida para o LCD
    lcd.print("SOLO UMIDO");   
  } 
  else {	
    //Acionar sistema de irrigação
    digitalWrite(motor, HIGH);
    Serial.println("Motor ligado");
    Serial.println("Irrigando...");
    
    //Saida para o LCD
    lcd.print("IRRIGANDO...");
    delay(2000);
    digitalWrite(motor, LOW);
    Serial.println("Motor desligado");
  }
  delay(2000);

  //Itencidade de luz
  ValorLDR = analogRead(pinoLDR); //Faz a leitura do sensor, em um valor que pode variar de 0 a 1023
  IntensidadeLuz = map(ValorLDR, 0, 1023, 0, 100); //Converte o valor para uma escala de 0 a 100
  Serial.print("Intensidade de Luz = "); //Imprime texto “Intensidade de Luz = ” na tela
  Serial.println(IntensidadeLuz); //Imprime o valor lido na tela após o sinal de igual do texto acima
  
  //Mostrando a intensidade de luz no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("QUANT. DE LUZ: ");
  lcd.setCursor(0,1);

  //Se a intensidade de luz for menor que 60, então a luz esta boa, senão está pouca
  if(IntensidadeLuz < 60){
  	lcd.print("ALTA");
  } else {
    lcd.print("BAIXA");
  }
  delay(2000);

  //Sensor de temperatura e umidade do ambiente
  int chk = DHT.read11(DHT11_PIN);

  Serial.print("Temperatura = ");
  Serial.println(DHT.temperature);
  Serial.print("Umidade do ar = ");
  Serial.println(DHT.humidity);

  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("TEMPERATURA: ");
  lcd.setCursor(0,1);
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("UMIDADE DO AR: ");
  lcd.setCursor(0,1);
  lcd.print(DHT.humidity);
  lcd.print("%");
  
  Serial.println("-----------");

  delay(1000);  
}
