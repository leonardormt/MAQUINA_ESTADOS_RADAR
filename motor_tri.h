#include <SPI.h>
#include <Controllino.h>



///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

//                 Funciones para el movimiento del motor trifasico ///

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define pinFrencuencia 10
#define pinMotor 3
#define MAX_VEL 24000
#define VEL_PREDET 1440




///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////




bool ini_motor(){
  pinMode(pinMotor,OUTPUT);

  return true;
  //Serial.begin(9600);
  
  
}

void moverMotor(){

  int velocidad=VEL_GIRO; //esta es la variable global en la que se almacena la velocidad del objetivo mas cercano.

  if(velocidad==0)
    velocidad=1440;

   velocidad=map(velocidad,0,MAX_VEL,0,255);
  analogWrite(pinmotor,velocidad);
  
}


void pwm(){
  
}




