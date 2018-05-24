#include "radar.h"
#include "motor_tri.h"
#define pinMega 2 // entrada digital para el cambio de estado 
#define tiempoEspera 1000


enum globalState{
  setupRadar, setupMotor,setupComs,Standby,working,error
};

enum workingState{

  activo_CON_OBJETIVO,activo_SIN_OBJETIVO,errorAct
};

//VARIALBLE DE ESTADO

bool state_flag='0';

globalState currentState;
globalState nextState;
