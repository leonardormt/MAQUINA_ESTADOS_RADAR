
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#define TARGET_MAX_COUNT 100
#define IN_AIR_MAX_TARGETS 20
#define MAX_VEL 3000

#define pinEstado 2 // entrada digital para el cambio de estado 





///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Estructuras de datos//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


union bytetofloat {
byte asBytes[4];
float asFloat;
} Vel,Dist,Ints,Ang;
union bytetoInt {
byte asBytes[4];
int asInt;
} ID;
typedef struct Target{
  float velocidad;
  float angulo;
  float distancia;
  float intensidad;
  int ID;
};
enum State{
  reposo, activo
};
typedef struct {

  float VEL;
  float rpm;

  //faltan los datos que enviar

} DATA;



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////VARIABLES GLOBALES////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


//contadores de objetivos total y a tiempo real
int Count_Target;
int Count_Target_tiempoReal;
Target Objetivos[TARGET_MAX_COUNT]; //Este es un vector con elnumero totales de objetivos localizados
Target ObjetivosTiempoReal[IN_AIR_MAX_TARGETS]; // vecor con los objetivos localizados en el momento
Target closest_target;


//Velocidad a la que deberá girar la plataforma
float VEL_GIRO;

//Inicialización de variables necesarias para la conexion UDP por ETHERNET
IPAddress ip(192,168,1,27); //Escribimos nuestra IP
unsigned int localPort=2700;
uint32_t Aux=0;
byte mac[]={0x2C,0x60,0x0C,0x92,0x88,0xB7};
EthernetUDP Udp;

//VARIALBLE DE ESTADO
State currentState;
//DATA sendingData;  //Esto es lo que pasamos por RS485
byte packetBuffer[UDP_TX_PACKET_MAX_SIZE*4];  //Lugar donde recibiremos datos
bool flag;  //Bandera para indicar si es el inicio del programa
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


int sacarID(byte buff[])                //funcion para sacar la ID del paquete correspondiente
{
  int id;
    ID.asBytes[0]=buff[2];                  //Empezamos desde  buff[2] ya que es el Byte desde el cual empieza la ID
    ID.asBytes[1]=buff[3];
    ID.asBytes[2]=buff[4];
    ID.asBytes[3]=buff[5];

    id=ID.asInt;

    return id;
  
}

void cleanBuffer(byte buff[])           //Función para limpiar el buffer de datos despues de cada iteraccion
  {
    for(int i=0;i<strlen(buff);i++)
    {
      buff[i]=NULL;
    }
  }
  

void printIP (IPAddress ad)  //función para imprimir la dirección IP 
{
  for (int i=0;i<4;i++)
  {
    Serial.print(ad[i]);

    if(i<3)
    Serial.print(".");
  }
  Serial.println(" ");

}

  
  void inicializar(){
     Serial.begin(115200);   //Velocidad de datos
   Serial.println("COMIENZA LA PRUEBA RADAR");
   Ethernet.begin(mac,ip); //Nos conectamos a la IP deseada
  //delay?
   Serial.println(Ethernet.localIP());
   if(Udp.begin(localPort))           //Comenzamos la conexión y comprobamos
   Serial.println("CONEXION OK");
  
   Serial.println(Udp.remoteIP());
   //delay 10
  }

  
  void desglosarPaquete(byte Buff[])      //Función principal del programa. En esta se tratan los datos recibidos en el paquete
  {
    int numObjetivos;
   
    Target aux;
    int flag_aux=0;
    int cont_targets=0;
  
 // /*  
  if(!(int(Buff[0])))                                     //EL primer byte del buffer siempre es '1' asi que en caso contrario significa que hemos recibido un paquete erroneo
      printf("ERRRROOOOOOOOOOOOOOOOOOOOR");
      else
      { 
        numObjetivos= int(Buff[1]);                       //Analizamos el numero de objetivos detectados por el radar. Esa información está en el Byte 1 del buffer

        for(int j=0;j<numObjetivos;j++)                               //SABIENDO QUE DEPENDIENDO DEL "numObjetivos"  VAMOS A TENER MAS O MENOS DATOS EN EL BUFFER  
        {                                                             //Pero si que sabemos en que posición se encontraran dichos datos en el caso de que tengamos objetivos detectados
          for(int i=0;i<5;i++)                                        // 'j' se refiere al numero de objetivos
          {                                                           // i<5 debido que se trantan 5 grupos de datos
            switch (i)
            {

                case 0:
                       Vel.asBytes[0]= Buff[38+20*j+i*4+0];           //A partir del byte 38 de nuestro buffer es donde se guardará la información relativa a los objetivos detectados
                       Vel.asBytes[1]= Buff[38+20*j+i*4+1];           
                       Vel.asBytes[2]= Buff[38+20*j+i*4+2]; 
                       Vel.asBytes[3]= Buff[38+20*j+i*4+3];
                       break;
                case 1:
                       Ang.asBytes[0]= Buff[38+20*j+i*4+0];           //DE CADA OBJETIVO SE SACAN 5 DATOS :Velocidad,angulo,Distancia,Intensidad de la señal y la Identificación
                       Ang.asBytes[1]= Buff[38+20*j+i*4+1];           // Cada uno de los anteriores datos ocupa 4 bytes
                       Ang.asBytes[2]= Buff[38+20*j+i*4+2];
                       Ang.asBytes[3]= Buff[38+20*j+i*4+3];
                       break;
                case 2:                                               
                       Dist.asBytes[0]= Buff[38+20*j+i*4+0];
                       Dist.asBytes[1]= Buff[38+20*j+i*4+1];
                       Dist.asBytes[2]= Buff[38+20*j+i*4+2];
                       Dist.asBytes[3]= Buff[38+20*j+i*4+3];
                       break;
                case 3:
                       Ints.asBytes[0]= Buff[38+20*j+i*4+0];
                       Ints.asBytes[1]= Buff[38+20*j+i*4+1];          //Los Datoss se guardan en forma de Bytes en una "UNION" para despues ser interpretados en forma de "int" o "float"
                       Ints.asBytes[2]= Buff[38+20*j+i*4+2];
                       Ints.asBytes[3]= Buff[38+20*j+i*4+3];
                       break;
                case 4:
                       ID.asBytes[0]= Buff[38+20*j+i*4+0];
                       ID.asBytes[1]= Buff[38+20*j+i*4+1];
                       ID.asBytes[2]= Buff[38+20*j+i*4+2];
                       ID.asBytes[3]= Buff[38+20*j+i*4+3];
                       break;
            }    
          }

          aux.velocidad=Vel.asFloat;                                            
          aux.angulo=Ang.asFloat; 
          aux.distancia=Dist.asFloat;
          aux.intensidad=Ints.asFloat;          //Tras haber sacado Los datos se guardan en una variable de la estructura target
          aux.ID=ID.asInt;                                        


          for(int z=0;z<Count_Target;z++)
          {
             if(aux.ID==Objetivos[z].ID)
             {
                Objetivos[z]=aux;                         //Y se transmiten a un vector de TARGETS
                flag_aux=1;
             }
          }

          ObjetivosTiempoReal[cont_targets]=aux;
          cont_targets++;

          if(!flag_aux)
          {
           Objetivos[Count_Target]=aux;
           Count_Target++;
          }
          flag_aux=0;
        }

          Count_Target_tiempoReal=cont_targets;
        
      }//*/
  }


Target ordenarDistancia(Target a[],int cantidad)  //FUNCION QUE ORDENA DE MENOR A MAYOR LOS OBJETIVOS SEGUN LA DISTANCIA Y ADEMAS DEVUELVE EL OBETIVO MAS CERCANO.
{
  Target aux;
  bool bandera=0;
for( int i = 0; i < cantidad; i++)
    {
        for( int j = i; j < cantidad; j++)
        {
            if(a[i].distancia > a[j].distancia)
            {
                aux = a[i];
                a[i] = a[j];
                a[j] = aux;
            }
        }
    }

      for(int k=0;k< cantidad;k++)
      {
        if(a[k].velocidad>0 && bandera==0)
           {
             aux=a[k];
             bandera=1;
           }
      }


  return aux;

}


void print_target(Target a)
{
   Serial.print("       LA distancia es de ----> ");
   Serial.println(a.distancia);
   
   Serial.print("       LA Velocidad es de ----> ");
   Serial.println(a.velocidad);
   
   Serial.print("       El angulo es de ----> ");
   Serial.println(a.angulo);
   
   Serial.print("       LA Intensidad de la señal es de ----> ");
   Serial.println(a.intensidad);

   
   Serial.print("LA ID es  ----> ");
   Serial.println(a.ID);

   Serial.println(" ");
   Serial.println(" ");

}

void printTargets()
{
  if(Count_Target_tiempoReal)              //Vemos cuantos objetivos hay detectados a tiempo real
        {
           Serial.println("HEMOS DETECTADO OBJETIVOS ");
            Serial.println(" ");
          for(int p=0;p<Count_Target_tiempoReal;p++)                         //Imprimimos su información por pantalla.
          {
             Serial.print("ESTE ES EL OBJETIVO A TIEMPO REAL NUMERO ");
             Serial.println((p+1)); 
             print_target(ObjetivosTiempoReal[p]);
          }
        }
}

float sacar_Velocidad(){
   int packetSize = Udp.parsePacket();


   //si hay paquete...
   if (packetSize)
   {

    cleanBuffer(packetBuffer); //VACIAMOS A CAD DE CARACTERES
    
    Udp.read(packetBuffer,packetSize); //Según la libreria 

    
    if (!flag)      //La Primera vez que entramos en el LOOP guardamos el valor de la ID del mensaje para comprobar mas adelante errores
    {
        Aux=sacarID(packetBuffer);
        flag=1;
    }

    if(Aux==sacarID(packetBuffer))  //Comprobación de que no haya paquetes repetidos
    {                                                                                     //PUEDE DAR ERROR SI POR CASUALIDAD SE SALTA 1 PAQUETE (no debería pasar)
      //Serial.println("Pasamos a desglosar paquertes ");
      //Serial.println(" ");

      
      desglosarPaquete(packetBuffer);  //Función que entregando el buffer de datos , coge los datos, los cambia al formato deseado y los guarda en un vector de objetivos 

      if(Aux>UINT32_MAX) //AUX es la ID del ultimo mensaje.  Si llega al maximo vuelve a empezar de 0
      Aux=0;
      else
      Aux++;

      closest_target=ordenarDistancia(ObjetivosTiempoReal, Count_Target_tiempoReal); //closest_target se referirá al objetivo mas cercano que se mueva en dirección al radar
      
      return closest_target.velocidad; 
      }
   }

   return 0; // en el caso de no detectar nada o fallo supondremos que no va a aterrizar
             // ningun dron por lo que enviamos velocidad=0.
}

