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
#define FANRELAY  4
#define PUMPRELAY 7
#define LEDRELAY  9
#define FOTORES   A0
#define YL49PIN   A1

dht11 DHT11;

void  setup()
{
  Serial.begin(9600);
  Serial.println("################ Bienvenido ####################");

  Serial.println(" _____ ____  _____ _____ _      _  ________  _  ");
  Serial.println("/  __//  __\/  __//  __// \  /|/ \/    /\  \//  ");
  Serial.println("| |  _|  \/||  \  |  \  | |\ ||| ||  __\ \  /   ");
  Serial.println("| |_//|    /|  /_ |  /_ | | \||| || |    / /    ");
  Serial.println("\____\\_/\_\\____\\____\\_/  \|\_/\_/   /_/     ");
  Serial.println("                                                 ");
  Serial.println(" ____  ____  _____  ____  _      _  ____        ");
  Serial.println("/  __\/  _ \/__ __\/  _ \/ \  /|/ \/   _\       ");
  Serial.println("| | //| / \|  / \  | / \|| |\ ||| ||  /         ");
  Serial.println("| |_\\| \_/|  | |  | |-||| | \||| ||  \_        ");
  Serial.println("\____/\____/  \_/  \_/ \|\_/  \|\_/\____/       "); 
}

void loop()
{
  Serial.println();

  //leemos al sensor dht 
  int chk = DHT11.read(DHT11PIN);
  //checamos si si leyo
  
  if(isDHTReady(DHT11.humidity,DHT11.temperature)){
    //si leyo entonces operamos con humedad del aire y temperatura
    if(isAirWet())
      pulsaInhale();
    getTemp();
  }
  
  
  isLRDReady(getLumens(FOTORES))?Serial.print("Si"):Serial.print("NO");
   


  //soil humidity  
  isDry(analogRead(YL49PIN))?Serial.print("Si"):Serial.print("NO");


	

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
  anaValue = map(anaValue, 1023, 0, 0, 100); //OscuridadTotal:0  ==> bn iluminado: 100%

  Serial.print("Luminosidad promedio:  [\b\b ]");
  Serial.print((int)anaValue, DEC);
  Serial.print("%");
  return anaValue;  
}
  
bool isDry(int sensorValue){

  Serial.print("La humedad de la tierra es: ");
	Serial.println(sensorValue);

  //  Prints the condition of soil.  Dry, Wet or Perfect
	if (sensorValue >= 1000){  
    (Serial.print("Tierra Seca! ): \n Enciendiendo bomba de agua"));
    pulsaBomba();
    return true;
  }else if ((sensorValue <= 999)  && (sensorValue >=901)){
    (Serial.print("Tierra perfecta! :D   "));
    return false;
  }else  if (sensorValue <= 900) {
    (Serial.print("Tierra muy mojada :o \n Encendiendo extractores"));
    pulsaExhale();
    return false;
	}else;
}
void pulsaBomba(){
//TODO

}void pulsaInhale(){
//TODO

}void pulsaExhale(){
//TODO

}bool isDHTReady(float h, float t){   
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }
  return true;
}
bool isLRDReady(int l){
  if (isnan(l)){
    Serial.println("Failed to read from LIGHT sensor!");
    return false;
  } 
  return true;
}
bool isAirWet(){
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  //no sabemos aun q % es humedo o no
  //TODO add condition
  return false;   
}
