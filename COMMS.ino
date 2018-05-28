

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
  sendMSG("$C001;");
  Serial.print("EEEE    ");

  /* This will initialize Controllino RS485 pins */
  //Same as
 

}




