

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
  Serial.print("Esperando respuesta del MASTER  ");

  /* This will initialize Controllino RS485 pins */
  //Same as
 

}

void enviar_error_state()
{
  String msg;
  msg+="$3DOLARES$;";
  Serial.println(msg);
  sendMSG(msg);
  
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

void enviar_aterrizaje(){
    String msg;
    msg+="$A";
     msg+=";";
    sendMSG(msg);
  
}
void CheckRST(){
    if((digitalRead(pinData)== HIGH) && !(stby_flag))
      {
        stby_t1=millis();
        stby_flag=true;
       }

      if(stby_flag && ((digitalRead(pinData))== LOW))
      {
        stby_t2=millis();
         if((stby_t2-stby_t1) > (tiempoEspera_reset - time_tol) && (stby_t2-stby_t1)<(tiempoEspera_reset + time_tol ))
         {
          Serial.print(stby_t2-stby_t1);
          stby_flag=false;
          nextState=setupRadar; //antes era setupComs
          init_Gl_variables();
          Serial.println("  SUCCES Setup");
          
         }
         else
         {
          Serial.print(stby_t2-stby_t1);
            stby_flag=false;
            Serial.println("  NOT SUCCES Setup");
            //Pendiente por ver el tratamiento de estos errores
         }
         }
}

bool CheckSendData(){
    if((digitalRead(pinData)== HIGH) && !(data_flag))
      {
        data_t1=millis();
        data_flag=true;
       }

      if(data_flag && ((digitalRead(pinData))== LOW))
      {
        data_t2=millis();
         if((data_t2-data_t1) > (tiempoEspera_data-time_tol) && (data_t2-data_t1)<(tiempoEspera_data + time_tol ))
         {
          Serial.print(data_t2-data_t1);
          data_flag=false;
          return true;
          Serial.println("  SUCCES DATA");
          
         }
         else
         {
          Serial.print(data_t2-data_t1);
            data_flag=false;
            Serial.println("  NOT SUCCES DATA");
            
            //Pendiente por ver el tratamiento de estos errores
         }
         }
         return false;
}



