/***
 *      /$$$$$$                                          /$$  /$$$$$$                
 *     /$$__  $$                                        |__/ /$$__  $$               
 *    | $$  \__/  /$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$$  /$$| $$  \__//$$   /$$      
 *    | $$ /$$$$ /$$__  $$ /$$__  $$ /$$__  $$| $$__  $$| $$| $$$$   | $$  | $$      
 *    | $$|_  $$| $$  \__/| $$$$$$$$| $$$$$$$$| $$  \ $$| $$| $$_/   | $$  | $$      
 *    | $$  \ $$| $$      | $$_____/| $$_____/| $$  | $$| $$| $$     | $$  | $$      
 *    |  $$$$$$/| $$      |  $$$$$$$|  $$$$$$$| $$  | $$| $$| $$     |  $$$$$$$      
 *     \______/ |__/       \_______/ \_______/|__/  |__/|__/|__/      \____  $$      
 *                                                                    /$$  | $$      
 *                                                                   |  $$$$$$/      
 *                                                                    \______/       
 *     /$$$$$$$              /$$                         /$$                         
 *    | $$__  $$            | $$                        |__/                         
 *    | $$  \ $$  /$$$$$$  /$$$$$$    /$$$$$$  /$$$$$$$  /$$  /$$$$$$$               
 *    | $$$$$$$  /$$__  $$|_  $$_/   |____  $$| $$__  $$| $$ /$$_____/               
 *    | $$__  $$| $$  \ $$  | $$      /$$$$$$$| $$  \ $$| $$| $$                     
 *    | $$  \ $$| $$  | $$  | $$ /$$ /$$__  $$| $$  | $$| $$| $$                     
 *    | $$$$$$$/|  $$$$$$/  |  $$$$/|  $$$$$$$| $$  | $$| $$|  $$$$$$$               
 *    |_______/  \______/    \___/   \_______/|__/  |__/|__/ \_______/               
 *                                                                                   
 *                                                                                                                                                                  
 */

#include <dht11.h>



#define DHT11PIN  5
#define IFANRELAY 4
#define OFANRELAY 3
#define PUMPRELAY 7
#define LEDRELAY  9
#define FOTORES   A0
#define YL49PIN   A1

dht11 DHT11;

void  setup()
{
  Serial.begin(9600);
  pinMode(PUMPRELAY,OUTPUT);
  pinMode(IFANRELAY,OUTPUT);
  pinMode(OFANRELAY,OUTPUT);
  //pinMode(LEDRELAY,OUTPUT);


  Serial.println("################ Bienvenido ####################");
}

void loop()
{
  Serial.println("Started Loop\n");

  //leemos al sensor dht 
  int chk = DHT11.read(DHT11PIN);
  //checamos si si leyo


  
  if(isDHTReady(DHT11.humidity,DHT11.temperature)){
    //si leyo entonces operamos con humedad del aire y temperatura
    if(isAirWet()){
      pulsaExhale();
      pulsaInhale();
    }
    getTemp();
  }else{
        Serial.println("ERR : No se pudo leer el sensor de la humedad en el aire!\n");
  }
  Serial.println("End DHT procedures\n");
  
  isLRDReady(getLumens(FOTORES))?Serial.print("OK\n"):Serial.println("ERR : Error al leer el fotoresistor!\n");

   
  Serial.println("End LRD process\n");

  //soil humidity  
  isDry(analogRead(YL49PIN))?Serial.println("ERR : Mala lectura de humedad en la tierra\n"):Serial.print("OK\n");
  Serial.println("End YL-49 process\n");

	

  delay(2000);

}
float getTemp(){
  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);
}
//fotoresister requires LD33V
int getLumens(int fotoResPin){
  int anaValue = 0;
  for(int i = 0; i < 10; i++) // vamos a leer el sensor 10 veces 
  {
    anaValue += analogRead(fotoResPin);   
    delay(50);
  }
  
  anaValue = anaValue/10; //promediemos
  anaValue = map(anaValue, 1023, 0, 0, 100); //OscuridadTotal:100%  ==> bn iluminado: 0%

  anaValue=100-anaValue;

  Serial.print("Luminosidad promedio:  [");
  Serial.print((int)anaValue, DEC);
  Serial.print("%]\n");
  return anaValue;  
}
  
bool isDry(int sensorValue){

  Serial.print("La humedad de la tierra es: ");
	Serial.println(sensorValue);

  //  Prints the condition of soil.  Dry, Wet or Perfect
	if (sensorValue >= 1000){  
    (Serial.print("Tierra Seca! ): \n Enciendiendo bomba de agua\n"));
    pulsaBomba();
    return true;
  }else if ((sensorValue <= 999)  && (sensorValue >=901)){
    (Serial.print("Tierra perfecta! :D   \n"));
    return false;
  }else  if (sensorValue <= 900) {
    (Serial.print("Tierra muy mojada :o \n Encendiendo extractores \n"));
    pulsaExhale();
    return false;
	}else;
}
void pulsaBomba(){
  digitalWrite(PUMPRELAY,HIGH);
  delay(1000);
  digitalWrite(PUMPRELAY,LOW);
  delay(1000);
}void pulsaInhale(){
  digitalWrite(IFANRELAY,HIGH);
  delay(1000);
  digitalWrite(IFANRELAY,LOW);
  delay(1000);
}void pulsaExhale(){
  digitalWrite(OFANRELAY,HIGH);
  delay(1000);
  digitalWrite(OFANRELAY,LOW);
  delay(1000);
}bool isDHTReady(float h, float t){   
  return (isnan(h) || isnan(t))? false : true;
}
bool isLRDReady(int l){
  return (isnan(l))? false : true;
}
bool isAirWet(){
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  
  if(DHT11.humidity<60){
    return false;
  }else if(DHT11.humidity > 60 && DHT11.humidity<80){
    return false;
  }else{
    return true;
  }

}
