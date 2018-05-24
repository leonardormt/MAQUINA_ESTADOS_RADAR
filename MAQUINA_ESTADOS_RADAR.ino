#include "StateMACH.h"



void enter_GL_State(){
    switch(currentState)
  {
    case setupRadar: setupRadar(); break;
    case setupMotor: setupMotor();break;
    case setupComs: setupComs();break;
    case Standby: Standby();break;
    case working: working();break;
    case error: error();break;
  }
}

void setupRadar()
{
  if(inicializar_radar())
  currentState=setupMotor;
  else
  {
    nextState=error;
    Serial.println("ERROR INICIAL DE RADAR");
  }
}

void setupMotor()
{
   if(ini_motor())
  currentState=setupComs;
  else
  {
    nextState=error;
    Serial.println("ERROR INICIAL DEL MOTOR");
  }
   
}

void setupComs()
{
  
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

void activo_con(){
  
  
      VEL_GIRO=sacar_Velocidad();

      if(!VEL_GIRO)
      state_flag=false;
      
      //moverMotor();//FALTA AUN POR HACER ESTA

      //send_DATA(); // Y ESTA
      
    
}

void activo_sin(){

//  moverMotor();
  
  if(sacar_Velocidad())
    state_flag=true;
}

void init_Gl_variables()
{
  currentState = reposo;
  Count_Target=0;
  flag=0;
  Count_Target_tiempoReal=0;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(pinEstado,INPUT);
  
  init_Gl_variables();  // inicializamos las variables globales en esta función
  
  ini_motor();

}

void loop() { 

  update_GL_State();
  
  enter_GL_State();

}
