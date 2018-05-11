#include "radar.h"



void enterState()
{
    switch(currentState)
  {
    case reposo: reposos(); break;
    case activo: activos();break;
  }
}

void updateState(){
  if(digitalRead(pinEstado))
     currentState=activo;
     else
     currentState=reposo;
}


void reposos(){
  
}

void activos(){
  
  
      VEL_GIRO=sacar_Velocidad();

      
//      moverMotor();//FALTA AUN POR HACER ESTA

  //    send_DATA(); // Y ESTA
      
    
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
  inicializar();

}

void loop() { 

  updateState();
  
  enterState();

}
