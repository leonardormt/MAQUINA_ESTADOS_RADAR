#include "radar.h"
#include "motor_tri.h"
#define pinEstado 2 // entrada digital para el cambio de estado 

bool state_flag='0';
//VARIALBLE DE ESTADO
globalState currentState;
globalState nextState;

enum globalState{
  setupRadar, setupMotor,setupComs,Standby,working,error;;
};

enum workingState{

  activo_CON_OBJETIVO,activo_SIN_OBJETIVO,errorAct;
}

