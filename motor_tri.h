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

class Motor_tri 
{
  float rpm;
  int pin;

  public:
  Motor_tri();
  Motor_tri(int a){pin=a;}
  void ini_motor();
  void moverMotor();
  void pwm();
  
};


Motor_tri::Motor_tri(){

  pin=pinMotor;
  ini_motor();
  
}
void Motor_tri::ini_motor(){
  
  pinMode(pin,OUTPUT);

 
  
  
}

void Motor_tri::moverMotor(){

  int velocidad=VEL_GIRO; //esta es la variable global en la que se almacena la velocidad del objetivo mas cercano.

  if(velocidad==0)
    velocidad=1440;

   velocidad=map(velocidad,0,MAX_VEL,0,255);
  analogWrite(pinMotor,velocidad);
  
}


void Motor_tri::pwm(){
  
}




