void setup() {
  Serial.begin(9600);  // Débuter la communication série
  delay(5000);  // Éviter des bugs avec le Arduino

  while (Serial.available() == 0) { delay(100); }  // Attendre la première commande
}

void loop() {

}
