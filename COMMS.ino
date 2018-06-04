

#define DATA_HEADER $D
#define MSG_ENDING ;
#define CONFIRM_HEADER $C
#define ERROR_HEADER $E
#define NO_TARGET_DETECTED_MSJ $D-1;
#define Motor_Radar_errors "$E110;"
#define Radar_error "$E010;"
#define Motor_error "$E100;"
#define NO_error "$E000;"




bool estado;

char msgRecibido[10];
uint8_t cont;

String str = "What's up";

void sendMSG(String buff) {
  //Envio sin recivo
  PORTJ = PORTJ & B10011111;
  PORTJ = PORTJ | B01100000;

  Serial3.println(buff);
}




void setupCOMMS() {

  Serial3.begin(115200);
  Controllino_RS485Init(); 
  //sendMSG("$C001;");
  Serial.print("EEEE    ");

  /* This will initialize Controllino RS485 pins */
  //Same as
 

}

void enviar_data_radar(Target a)
{
  String msg;
  msg+="$D";
 
  msg+=a.ID;
  msg+=","; 
  msg+=a.velocidad;
  msg+=",";
  msg+=a.distancia;
  msg+=",";
  msg+=a.angulo;
  msg+=",";
  msg+=a.intensidad;
  msg+=";";
  Serial.println(msg);
  sendMSG(msg);
  
}

void enviar_data_error(){

 if((motor_error) && !(radar_error))
 sendMSG(Motor_error);
  else if(!(motor_error) && (radar_error))
  sendMSG(Radar_error);
  else if((motor_error) && (radar_error))
  sendMSG(Motor_Radar_errors);
  else if(!(motor_error) && !(radar_error))
  sendMSG(NO_error);
}

