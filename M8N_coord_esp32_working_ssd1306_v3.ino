/* 
 *  nano pins
 SoftwareSerial mySerial(10, 11); // RX, TX
SoftwareSerial mySerial1(8, 9); // RX, TX

esp32 hardware serial pins
uart0   rx 03   tx 01
uart1   27/ 09      26/ 10
uart2      16      17

*/
long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to Ublox module.
float boylam, enlem, yukseklik, uydu, fiks =22.2222222 ;
int saniye, dakika, saat, gun, ay, yil = 0;

#include "Ublox.h"
#define SERIAL_BAUD 9600
#define GPS_BAUD 9600
Ublox M8_Gps;

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, 21, 22);

//#include <SoftwareSerial.h>

//SoftwareSerial ss(8, 9);


#include <HardwareSerial.h> //https://circuits4you.com/2018/12/31/esp32-hardware-serial2-example/
// https://esp32.com/viewtopic.php?t=10300
HardwareSerial gnss(1);
HardwareSerial sd(2);

void setup() {
   Serial.begin(SERIAL_BAUD);
   sd.begin(38400, SERIAL_8N1, 16, 17); //sd
   gnss.begin(9600, SERIAL_8N1, 27, 26); //gnss

    
     display.init();   
     display.flipScreenVertically();
     display.setFont(ArialMT_Plain_10);
 }

 
void loop() {
   if(!gnss.available())
		return;
  while(gnss.available()){
        char c = gnss.read();
         if (M8_Gps.encode(c)) {
    yukseklik =  M8_Gps.altitude;
    enlem =  M8_Gps.latitude;
    boylam =   M8_Gps.longitude; 
    uydu =   M8_Gps.sats_in_use;
    fiks = M8_Gps.fixtype;

    saniye = M8_Gps.datetime.seconds;
    dakika = M8_Gps.datetime.minutes;
    saat = M8_Gps.datetime.hours;
    gun =  M8_Gps.datetime.day;
    ay =  M8_Gps.datetime.month;
    yil = M8_Gps.datetime.year;
        }
      if (millis() - lastTime > 2000)
 {
  lastTime = millis(); //Update the timer   
   
   //for(byte i = 0; i < N_FLOATS; i++) {
   // Serial.print(gpsArray[i], 6);Serial.print(" ");
 
char print_gnss[40];
 sprintf (print_gnss, "%.7f;%.7f;%.2f,%.0f,%.0f", boylam, enlem, yukseklik, uydu, fiks);
 Serial.print(print_gnss);
 sd.print(print_gnss);
 
 char print_gun [24];
 sprintf (print_gun, ",%02d:%02d:20%2d," , gun, ay,yil );
 Serial.print(print_gun);
 sd.print(print_gun);
  
 char print_zaman [24];
 sprintf (print_zaman, "%02d:%02d:%02d" , saat+3, dakika,saniye );
 Serial.println(print_zaman);
 sd.println(print_zaman);
 
display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
   // display.setFont(ArialMT_Plain_10);
    display.setFont(ArialMT_Plain_16);

  char print_enlem[14];
 sprintf (print_enlem, "%.8f",  enlem);
  display.drawString(0, 0, print_enlem);

  char print_boylam[14];
  sprintf (print_boylam, "%.8f",  boylam); 
  display.drawString(0, 15, print_boylam);

  char print_yukseklik[14];
 sprintf (print_yukseklik, "%.3f",  yukseklik); 
  display.drawString(0, 30, print_yukseklik); 


   char print_uydu[6];
 sprintf (print_uydu, "%.0f",  uydu); 
  display.drawString(0, 45, print_uydu); 
  //display.drawString(0, 0, print_uydu);
  
 display.display();
 
} } }

