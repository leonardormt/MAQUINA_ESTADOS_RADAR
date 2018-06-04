#include "radar.h"
#include "motor_tri.h"
#define pinMega A0 // entrada digital para el cambio de estado 
#define tiempoEspera 105


enum globalState{
  setupRadar, setupMotor,setupComs,Standby,working,error
};

enum workingState{

  activo_CON_OBJETIVO,activo_SIN_OBJETIVO,//errorAct
};

//VARIALBLE DE ESTADO

bool state_flag='0';

globalState currentState;
globalState nextState;

workingState state;

bool motor_error;
bool radar_error=true;
bool coms_error;
bool working_error;
bool some_error;
