#include "StateMACH.h"

Target b;



void enter_GL_State(){
    switch(currentState)
  {
    case setupRadar: setup_Radar(); break;
    case setupMotor: setup_Motor();break;
    case setupComs: setup_Coms();break;
    case Standby: StandbyF();break;
    case working: workingF();break;
    case error: errorF();break;
  }
}

void setup_Radar()
{
  if(inicializar_radar())
  nextState=setupMotor;
  else
  {
    nextState=setupMotor;
    radar_error=true;
    Serial.println("ERROR INICIAL DE RADAR");
  }
}

void setup_Motor()
{
   Serial.println("Iniciando MOTOR TRI");
  nextState=setupComs;
  

}

void setup_Coms()
{
  int flag=0;
  int present;
  int past;
  Serial.println("Iniciando comunicaciones");
  
  if(!digitalRead(pinState)){
     
  setupCOMMS();
  enviar_data_error();
  while(flag!=2)
  {
    if(digitalRead(pinState) && flag==0)
    {
      flag=1;
      past=millis();
      
      //sendMSG("$C01;");

      Serial.println("YA ME HA llegado");
    }
    
    if(flag==1)
    {
     
      present=millis();
      if((present-past)>tiempoEspera )
      {
          Serial.println(present-past);
          sendMSG("$E001;");
          flag=2;
          nextState=Standby;
      }
      if(!digitalRead(pinState))
      {
        Serial.print((present-past));
        if(((present-past)<=(tiempoEspera)) && ((present-past)>(tiempoEspera-time_tol)) )
        {
          sendMSG("$C001;");
          flag=2;
          nextState=Standby;
          Serial.print("VOY A Standby");
        }
        else
        {
          Serial.println(present-past);
          sendMSG("$E001;");
          flag=2;
          nextState=Standby;
        }
        
      }
      
    }
  

    
  }
  
  
}
}


void StandbyF(){
//   moverMotor(0);
   if (digitalRead(pinState))
   { nextState=working; Serial.print("VOY A WORKING");}
    else
    nextState=Standby; 

  
  CheckRST();

   if(CheckSendData())
  enviar_data_radar(b);
 
}

void workingF(){
  switch (state)
  {
    case activo_SIN_OBJETIVO:activo_sin(); break;
    
    case activo_CON_OBJETIVO:activo_con(); break;
    
    
  }
  CheckRST();
  if(CheckSendData())
  enviar_data_radar(b);
  
}

void errorF(){
  
}

void update_GL_State(){
 /* if(digitalRead(pinEstado))
     {
      if(state_flag)
      currentState=activo_CON_OBJETIVO;
      else 
      currentState=activo_SIN_OBJETIVO;
     }
     else
     currentState=reposo;
*/
currentState=nextState;
}


void reposos(){
  
}

void acercandoseF(){


  Serial.print("Objetivo detectado");
  
      VEL_GIRO=sacar_Velocidad();
      motor.moverMotor();
   
if(closest_target.distancia<=Distacia_minima)
{
   Serial.print("Objetivo va a aterrizar");
  mstate=aterrizando;
  aux_target=closest_target;
  VEL_GIRO=aux_target.velocidad;
  return;
}
      if(!VEL_GIRO)
        state=activo_SIN_OBJETIVO;
      
      
      else
        motor.moverMotor();
    
  
}


void aterrizandoF(){
// moverMotor();
 if(!aterrizaje_flag)
 {
  aterrizaje_t1=millis();
  aterrizaje_flag=true;
 }
    if( (aterrizaje_flag==true) && (millis()-aterrizaje_t1)>= tiempo_aterrizaje)
          {
              state=activo_SIN_OBJETIVO;
              mstate=acercandose;
              aterrizaje_flag=false;
              motor.frenado();  //funcion para frenar
          }
  
  
}

void activo_con(){

  switch (mstate)
  {
    case acercandose:acercandoseF(); break;
    
    case aterrizando:aterrizandoF(); break;
    
    
  }
  
}

void activo_sin(){

  Serial.print("ESTOY TRABAJNDO PERO NO DETECTO");
//  moverMotor();
  int vel=sacar_Velocidad();
  if(vel>0)
    state=activo_CON_OBJETIVO;
    else
    motor.moverMotor();

  
}

void init_Gl_variables()
{
  currentState = setupRadar;
  nextState=setupRadar;
  Count_Target=0;
  flag=0;
  Count_Target_tiempoReal=0;
  state=activo_SIN_OBJETIVO;
  mstate=acercandose;
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinState,INPUT);
  pinMode(pinData,INPUT);

  //setupCOMMS();
  
  init_Gl_variables();  // inicializamos las variables globales en esta función
  b.ID=3;
  b.velocidad=3.4;
  b.distancia=24.5;
  b.angulo=1.2;
  b.intensidad=90.1;
  //enviar_data_radar(b);
 //setup_Coms();
 

}

void loop() { 

  update_GL_State();
  
  enter_GL_State();

  //enviar_data_radar(b);

}
