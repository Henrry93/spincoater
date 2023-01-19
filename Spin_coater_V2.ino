#define LED1 7
#define LED2 13
char dato;
String cadena, d1, d2, d3, d4, d5, d6;
int p1, p2, p3, tiempo1, tiempo2, tiempo3;
int p4, p5, p6, velocidad1, velocidad2, velocidad3;

int K1 = 0;  // las memorias equivalentes a los Relés se declaran en estado inicial bajo o false.
int K2 = 0;
int K3 = 0;
int S0;       // Hará la función de STOP
int S1;       // Harà la función de START
int H1 = 7;   // Pin de salida del LED
int H2 = 13;  // Pin de salida del LED

///////////////
// Variables asociadas a "temp1".
int T1 = LOW;       // Bit asociado al temporizador 1
int activado1 = 0;  // Al principio no ha sido activado.
long inicio1, final1, actual1;

///

// Variables asociadas a "temp2".
int T2;             // Bit asociado al temporizador 2
int activado2 = 0;  // Al principio no ha sido activado.
long inicio2, final2, actual2;


// Variables asociadas a "temp3".
int T3;             // Bit asociado al temporizador 3
int activado3 = 0;  // Al principio no ha sido activado.
long inicio3, final3, actual3;

// Variables asociadad al setpoint de velocidad

float spp;
int PWM_salida = 6;  



void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(H1, LOW);
  digitalWrite(H2, LOW);
  pinMode (PWM_salida, OUTPUT);
  
}

void loop() {

  if (Serial.available() > 0) {
    dato = Serial.read();
    cadena = cadena + dato;
    if (dato == '*') {
      p1 = cadena.indexOf('|');
      p2 = cadena.indexOf('|', p1 + 1);
      p3 = cadena.indexOf('|', p2 + 1);
      p4 = cadena.indexOf('|', p3 + 1);
      p5 = cadena.indexOf('|', p4 + 1);
      p6 = cadena.indexOf('|', p5 + 1);


      
      d1 = cadena.substring(0, p1);
      d2 = cadena.substring(p1 + 1, p2);
      d3 = cadena.substring(p2 + 1, p3);
      d4 = cadena.substring(p3 + 1, p4);
      d5 = cadena.substring(p4 + 1, p5);
      d6 = cadena.substring(p5 + 1, p6);
      

      tiempo1 = d1.toInt();
      tiempo2 = d2.toInt();
      tiempo3 = d3.toInt();
      velocidad1 = d4.toInt();
      velocidad2 = d5.toInt();
      velocidad3 = d6.toInt();
      Serial.print(tiempo1);
      Serial.print(tiempo2);
      Serial.print(tiempo3);
      Serial.print(velocidad1);
      Serial.print(velocidad2);
      Serial.print(velocidad3);

      cadena = "";
    }
    if (dato == 'a') {

      S0 = 1;
    }
    if (dato == 'b') {

      S0 = 0;
    }
    if (dato == 'c') {

      S1 = 1;
    }
    if (dato == 'd') {

      S1 = 0;
    }
  }

  if ((S1 | K1) && (!S0) && (!K2) && (!K3)) {
    K1 = 1;
  } else {
    K1 = 0;
  }

  if (K1) {
    activetemp1(tiempo1);
  } else {
    desactivetemp1();
  }


  if ((T1 | K2) && (!S0) && (!K3)) {
    K2 = 1;
  } else {
    K2 = 0;
  }


  if (K2) {
    activetemp2(tiempo2);
  } else {
    desactivetemp2();
  }

  if ((T2 | K3) && (!S0) && (!T3)) {
    K3 = 1;
  } else {
    K3 = 0;
  }

  if (K3) {
    activetemp3(tiempo3);
  } else {
    desactivetemp3();
  }



  //////SALIDAS

  if (K1) {
    digitalWrite(H1, HIGH);
    spp = velocidad1; // Dato desde Nextion 30%
    analogWrite(PWM_salida,spp*255.0/100.0);
    
  } else {
    digitalWrite(H1, LOW);
    analogWrite(PWM_salida, 0);
  
  }

  if (K2) {
    digitalWrite(H2, HIGH);
    spp = velocidad2;  // Dato desde Nextion 75%
    analogWrite(PWM_salida,spp*255.0/100.0);
  } else {
    digitalWrite(H2, LOW);
    analogWrite(PWM_salida, 0);
  }

  if (K3) {
    digitalWrite(H1, HIGH);
    spp = velocidad3;  // Dato desde Nextion 100%
    analogWrite(PWM_salida,spp*255.0/100.0);
  } else {
    digitalWrite(H1, LOW);
    analogWrite(PWM_salida, 0);
  }
}


//SUBRUTINA TEMPORIZADOR 1
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void activetemp1(int t1) {
  if (K1 && activado1 == 0) {  // Si K1 esta activa y no ha sido activado=0 antes...
    activado1 = 1;             // marca activado=1 y guarda el tiempo de inicio.
    inicio1 = millis();
    final1 = inicio1 + t1;  // dato desde nextion
  }
  actual1 = millis();  // Consulta el tiempo actual.
  if (activado1 == 1 && (actual1 > final1)) {
    T1 = HIGH;
  } else {
    T1 = LOW;
  }
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void desactivetemp1() {
  T1 = LOW;
  activado1 = 0;
  inicio1 = 0;
  final1 = 0;
  actual1 = 0;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//SUBRUTINA TEMPORIZADOR 2
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void activetemp2(int t2) {
  if (K2 && activado2 == 0) {  // Si ha pulsado HIGH y no ha sido activado=0 antes...
    activado2 = 1;             // marca activado=1 y guarda el tiempo de inicio.
    inicio2 = millis();
    final2 = inicio2 + t2;  // dato desde nextion
  }
  actual2 = millis();  // Consulta el tiempo actual.
  if (activado2 == 1 && (actual2 > final2)) {
    T2 = HIGH;
  } else {
    T2 = LOW;
  }
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void desactivetemp2() {
  T2 = LOW;
  activado2 = 0;
  inicio2 = 0;
  final2 = 0;
  actual2 = 0;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//SUBRUTINA TEMPORIZADOR 3
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void activetemp3(int t3) {
  if (K3 && activado3 == 0) {  // Si ha pulsado HIGH y no ha sido activado=0 antes...
    activado3 = 1;             // marca activado=1 y guarda el tiempo de inicio.
    inicio3 = millis();
    final3 = inicio3 + t3;  // Dato desde nextion
  }
  actual3 = millis();  // Consulta el tiempo actual.
  if (activado3 == 1 && (actual3 > final3)) {
    T3 = HIGH;
  } else {
    T3 = LOW;
  }
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void desactivetemp3() {
  T3 = LOW;
  activado3 = 0;
  inicio3 = 0;
  final3 = 0;
  actual3 = 0;
}
