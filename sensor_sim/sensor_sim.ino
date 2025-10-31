/**
 * @file sensor_sim.ino
 * @brief Simulador de sensores para envío por puerto serial
 */

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  // Simular temperatura (20.0 a 50.0 grados)
  float temperatura = random(200, 500) / 10.0;
  Serial.print("TEMP:");
  Serial.println(temperatura);
  
  delay(2000);
  
  // Simular presión (50 a 150 unidades)
  int presion = random(50, 150);
  Serial.print("PRES:");
  Serial.println(presion);
  
  delay(2000);
}