#define TRIGGER_A 19
#define TRIGGER_B 18
#define TRIGGER_C 2
#define TRIGGER_D 3
#define ECHO_A    4
#define ECHO_B    5
#define ECHO_C    6
#define ECHO_D    7
#define SALIDA_A  8
#define SALIDA_B  9
#define SALIDA_C  10
#define SALIDA_D  11
#define BUZZER    12
//#define SERVO     13
#define DIST_MIN  7
#define NIVEL_MAX 100

int melodia[ ] = {262, 196, 196, 220, 196, 0, 247, 262};
int duracionNotas[ ] = {4, 8, 8, 4, 4, 4, 4, 4};
int nivelActual = 1;
int velocidad = 500;
int secuencia[NIVEL_MAX];
int secuenciaUsuario[NIVEL_MAX];
int tono_A = 200;
int tono_B = 300;
int tono_C = 400;
int tono_D = 500;
int delayTimeBuzzer;

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER_A, OUTPUT);
  pinMode(TRIGGER_B, OUTPUT);
  pinMode(TRIGGER_C, OUTPUT);
  pinMode(TRIGGER_D, OUTPUT);
  pinMode(ECHO_A, INPUT);
  pinMode(ECHO_B, INPUT);
  pinMode(ECHO_C, INPUT);
  pinMode(ECHO_D, INPUT);
  pinMode(SALIDA_A, OUTPUT);
  pinMode(SALIDA_B, OUTPUT);
  pinMode(SALIDA_C, OUTPUT);
  pinMode(SALIDA_D, OUTPUT);
  digitalWrite(SALIDA_A, LOW);
  digitalWrite(SALIDA_B, LOW);
  digitalWrite(SALIDA_C, LOW);
  digitalWrite(SALIDA_D, LOW);
}

void loop() {
  if(nivelActual == 1) generaSecuencia();
  muestraSecuencia();
  leeSecuencia();
}

void generaSecuencia(){
  randomSeed(millis());
  for(int i = 0; i < NIVEL_MAX; i++) secuencia[i] = random(8,12);
}

void muestraSecuencia(){
  digitalWrite(SALIDA_A, LOW);
  digitalWrite(SALIDA_B, LOW);
  digitalWrite(SALIDA_C, LOW);
  digitalWrite(SALIDA_D, LOW);
  delayTimeBuzzer = 200;
  for(int i = 0; i < nivelActual; i++){
    if(secuencia[i] == SALIDA_A) sonarBuzzer(tono_A, delayTimeBuzzer);
    if(secuencia[i] == SALIDA_B) sonarBuzzer(tono_B, delayTimeBuzzer);
    if(secuencia[i] == SALIDA_C) sonarBuzzer(tono_C, delayTimeBuzzer);
    if(secuencia[i] == SALIDA_D) sonarBuzzer(tono_D, delayTimeBuzzer);
    digitalWrite(secuencia[i], HIGH);
    delay(velocidad);
    digitalWrite(secuencia[i], LOW);
    delay(200);
  }
}

void leeSecuencia(){
  int flag = 0;
  delayTimeBuzzer = 300;
  for(int i = 0; i < nivelActual; i++){
    flag = 0;
    while(flag == 0){
      printDist();
      if(distancia(TRIGGER_A, ECHO_A) <= DIST_MIN){
        if(leer(i, SALIDA_A, tono_A, delayTimeBuzzer)) return;
        flag = 1;
      }
      if(distancia(TRIGGER_B, ECHO_B) <= DIST_MIN){
        if(leer(i, SALIDA_B, tono_B, delayTimeBuzzer)) return;
        flag = 1;
      }
      if(distancia(TRIGGER_C, ECHO_C) <= DIST_MIN){
        if(leer(i, SALIDA_C, tono_C, delayTimeBuzzer)) return;
        flag = 1;
      }
      if(distancia(TRIGGER_D, ECHO_D) <= DIST_MIN){
        if(leer(i, SALIDA_D, tono_D, delayTimeBuzzer)) return;
        flag = 1;
      }
    }
  }
  secuenciaCorrecta();
}

float distancia(int trigger, int echo) {
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  return (pulseIn(echo,HIGH) / 2) / 29.15;
}

boolean leer(int i, int salida, int tono, int delayTimeBuzzer) {
  digitalWrite(salida, HIGH);
  sonarBuzzer(tono, delayTimeBuzzer);
  secuenciaUsuario[i] = salida;
  delay(300);
  if(secuenciaUsuario[i] != secuencia[i]){
    secuenciaError();
    return true;
  }
  digitalWrite(salida, LOW);
  return false;
}

void sonarBuzzer(int tono, int delayTimeBuzzer) {
  tone(BUZZER, tono);
  delay(delayTimeBuzzer);
  noTone(BUZZER);
}

void secuenciaError(){
  digitalWrite(SALIDA_A, HIGH);
  digitalWrite(SALIDA_B, HIGH);
  digitalWrite(SALIDA_C, HIGH);
  digitalWrite(SALIDA_D, HIGH);
  delay(250);
  digitalWrite(SALIDA_A, LOW);
  digitalWrite(SALIDA_B, LOW);
  digitalWrite(SALIDA_C, LOW);
  digitalWrite(SALIDA_D, LOW);
  melodiaError();
  nivelActual = 1;
  velocidad = 500;
}

void melodiaError(){
  for(int i = 0; i < 8; i++){
    int duracionNota = 1000/duracionNotas[i];
    tone(BUZZER, melodia[i],duracionNotas);
    int pausaEntreNotas = duracionNota * 1.30;
    delay(pausaEntreNotas);
    noTone(BUZZER);
  }
}

void secuenciaCorrecta(){
  if(nivelActual < NIVEL_MAX) nivelActual++;
  if(velocidad >= 100) velocidad -= 50;
  delay(200);
}

void printDist(){
  Serial.print(DIST_MIN);
  Serial.print("\tPing A: ");
  Serial.print(distancia(TRIGGER_A, ECHO_A)); 
  Serial.print(" cm");
  Serial.print("\tPing B: ");
  Serial.print(distancia(TRIGGER_B, ECHO_B)); 
  Serial.print(" cm");
  Serial.print("\tPing C: ");
  Serial.print(distancia(TRIGGER_C, ECHO_C)); 
  Serial.print(" cm");
  Serial.print("\tPing D: ");
  Serial.print(distancia(TRIGGER_D, ECHO_D)); 
  Serial.println(" cm");
}
