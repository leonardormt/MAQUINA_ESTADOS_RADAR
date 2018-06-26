#include "StateMACH.h"

Target b;



void enter_GL_State() {
  switch (currentState)
  {
    case setupRadar: setup_Radar(); break;
    case setupMotor: setup_Motor(); break;
    case setupComs: setup_Coms(); break;
    case Standby: StandbyF(); break;
    case working: workingF(); break;
    case error: errorF(); break;
  }
}
 
void setup_Radar()
{

  
  if (inicializar_radar())
    nextState = setupMotor;
  else
  {
    nextState = setupMotor;
    radar_error = true;
    Serial.println("ERROR INICIAL DE RADAR");
  }
}

void setup_Motor()
{
  Serial.println("Iniciando MOTOR TRI");
  nextState = setupComs;


}

void setup_Coms()
{
  int flag = 0;
  int present;
  int past;
  Serial.println("Iniciando comunicaciones");

  if (!digitalRead(pinState)) {

    setupCOMMS();
    enviar_data_error();
    while (flag != 2)
    {
      if (digitalRead(pinState) && flag == 0)
      {
        flag = 1;
        past = millis();

        //sendMSG("$C01;");

        Serial.println("YA ME HA llegado");
      }

      if (flag == 1)
      {

        present = millis();
        /*if ((present - past) > tiempoEspera )
        {
          Serial.println(present - past);
          sendMSG("$E001;");
          flag = 2;
          nextState = Standby;
        }*/
        if (!digitalRead(pinState))
        {
          Serial.print((present - past));
          if (((present - past) < (tiempoEspera + time_tol)) && ((present - past) > (tiempoEspera - time_tol)) )
          {
            sendMSG("$C001;");
            flag = 2;
            nextState = Standby;
            Serial.print("VOY A Standby");
          }
          else
          {
            Serial.println(present - past);
            sendMSG("$E001;");
            flag = 2;
            nextState = Standby;
          }

        }

      }



    }


  }
}


void StandbyF() {
  //   moverMotor(0);
  if (digitalRead(pinState) == HIGH)
  {
    nextState = working;
    Serial.print("VOY A WORKING");
  }
  else
    nextState = Standby;


b.ID = 1;
  CheckRST();

  if (CheckSendData())
    enviar_data_radar(b);

     printTargets();

}

void workingF() {
  switch (state)
  {
    case activo_SIN_OBJETIVO: activo_sin(); break;

    case activo_CON_OBJETIVO: activo_con(); break;


  }
  CheckRST();
  if (CheckSendData())
    enviar_data_radar(b);

  if (digitalRead(pinState) == LOW)
  {
    nextState = Standby;
    Serial.print("VOY A standby");
    mstate = acercandose;
    state = activo_SIN_OBJETIVO;
  }

 

}

void errorF() {
  CheckRST();
  //Serial.print("AS");

}

void update_GL_State() {

  currentState = nextState;
}


void acercandoseF() {

b.ID = 3;

  Serial.println("Objetivo detectado");

  VEL_GIRO = sacar_Velocidad();
  //  motor.moverMotor();

  if (closest_target.distancia <= Distacia_minima)
  {
    Serial.println("Objetivo va a aterrizar");
    mstate = aterrizando;
    aux_target = closest_target;
    VEL_GIRO = aux_target.velocidad;
    motor.moverMotor();
    return;
  }
  if (!VEL_GIRO)
    state = activo_SIN_OBJETIVO;


  else
    motor.moverMotor(); /////////EL motor se queda fijado si le envio velocidad 0?

    
}

void aterrizandoF() {

b.ID = 4;
  Serial.print(" Aterrizando ");
  if (!aterrizaje_flag)
  {
    aterrizaje_t1 = millis();
    aterrizaje_flag = true;
  }
  int aterrizaje_t2=millis();
  if ( (aterrizaje_flag == true) && (aterrizaje_t2 - aterrizaje_t1) >= tiempo_aterrizaje)
  {
    
    Serial.println(aterrizaje_t2 - aterrizaje_t1);
    Serial.println("Objetivo aterrizado");
    
    int t1,a;
    state = activo_SIN_OBJETIVO;
    mstate = acercandose;
    aterrizaje_flag = false;
    
    motor.frenado();  //funcion para frenar

//comunicacion con MEGA
    digitalWrite(pinAterrizaje,HIGH);
    enviar_aterrizaje();

    
    a=1;
    t1=millis();
    
    while(a==1)
    {
      int b= millis();
      if(digitalRead(pinState)==LOW)// en realidad es LOW pero para las prueba vale
      {
        Serial.println("VOY A standby");
        a=0;
        nextState=Standby;
        digitalWrite(pinAterrizaje,LOW);
      }
      if((b-t1)>=tiempo_espera_postAterrizaje)
      {
        Serial.println(b-t1);
        Serial.println("HA TARDADO DEMASIADO; NOS VAMOS A ERROR ");
        a=2;
        enviar_error_state();
        nextState=error;
        digitalWrite(pinAterrizaje,LOW);
      }
    }
  }


}

void activo_con() {

  switch (mstate)
  {
    case acercandose: acercandoseF(); break;

    case aterrizando: aterrizandoF(); break;


  }

}

void activo_sin() {

 // Serial.print("ESTOY TRABAJNDO PERO NO DETECTO");
  //  moverMotor();
  int vel = sacar_Velocidad();

  //Serial.println(vel);
  if (vel > 0)
    {state = activo_CON_OBJETIVO; Serial.print("VOY A activo_CON_OBJETIVO");}
  else
    motor.moverMotor(); // para que se quede quieto.

b.ID = 2;

}

void init_Gl_variables()
{
  currentState = setupRadar;
  nextState = setupRadar;
  Count_Target = 0;
  flag = 0;
  Count_Target_tiempoReal = 0;
  state = activo_SIN_OBJETIVO;
  mstate = acercandose;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinState, INPUT);
  pinMode(pinData, INPUT);
  pinMode(pinAterrizaje, OUTPUT);
  digitalWrite(pinAterrizaje, LOW);

  //setupCOMMS();

  init_Gl_variables();  // inicializamos las variables globales en esta función
  b.ID = 3;
  b.velocidad = 3.4;
  b.distancia = 24.5;
  b.angulo = 1.2;
  b.intensidad = 90.1;
  VEL_GIRO = 1000;




  //enviar_data_radar(b);
  //setup_Coms();


}

void loop() {
  
    update_GL_State();

    enter_GL_State();

  //motor.moverMotor();
  //enviar_data_radar(b);

  if (Serial.available() > 0)
  {
    switch (Serial.read())
    {
      case '1':
        nextState = working;
        Serial.println("VOY A WORKING");
        break;

      case '2':
        nextState = Standby;
        Serial.println("VOY A standby");
        mstate = acercandose;
        state = activo_SIN_OBJETIVO;
        break;

      case '3':
        nextState = working;
        mstate = acercandose;
        state = activo_CON_OBJETIVO;
        Serial.println("estoy en WORKING y viene objetivo");
        break;

        case '4':
        nextState = working;
        mstate = aterrizando;
        state = activo_CON_OBJETIVO;
        Serial.println("estoy en WORKING y va a aterrizar");
        break;

        default:
        nextState = setupRadar;
        Serial.println("VOY al comienzo");
        mstate = acercandose;
        state = activo_SIN_OBJETIVO;
        break;
    }
  }


}
