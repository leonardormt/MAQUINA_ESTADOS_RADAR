#include "StateMACH.h"





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
  currentState=setupMotor;
  else
  {
    nextState=error;
    Serial.println("ERROR INICIAL DE RADAR");
  }
}

void setup_Motor()
{
   if(ini_motor())
  currentState=setupComs;
  else
  {
    nextState=error;
    Serial.println("ERROR INICIAL DEL MOTOR");
  }
   
}

void setup_Coms()
{
  int flag=0;
  if(!digitalRead(pinMega))
  setupCOMMS();
  int present;
  int past;
  while(flag!=2)
  {
    
    if(flag==1)
    {
      present=millis();
      if(!digitalRead(pinMega))
      {
        if((present-past)>=tiempoEspera)
        {
          sendMSG("C00");
          flag=2;
          nextState=Standby;
        }
        else
        {
          sendMSG("C11");
          flag=2;
          nextState=error;
        }
        
      }
      
    }

    if(digitalRead(pinMega) && flag==0)
    {
      flag=1;
      past=millis();
      present=past;
    }
  }
  
  
}

void StandbyF(){
  
}

void workingF(){
  
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
  currentState = setupRadar;
  Count_Target=0;
  flag=0;
  Count_Target_tiempoReal=0;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinMega,INPUT);
  
  init_Gl_variables();  // inicializamos las variables globales en esta función
  
 setup_Coms();
 

}

void loop() { 
/*
  update_GL_State();
  
  enter_GL_State();*/

}
