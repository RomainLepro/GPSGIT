
/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>



#define analogYaw 5
#define analogPitch 6

int t,dt;
float vmax = 0;
float v = 1;
float Oz = 0.0;//yaw
float Oy = 0.0;//pitch
bool set = false;
float alt = 0.0;
float dist = 0.0;

// The serial connection to the GPS module
SoftwareSerial gpsSerial(7,8);
TinyGPSPlus gps;

double HOME_LAT = 46.64968;
double HOME_LNG = 00.36231;

void setup(){
  gpsSerial.begin(9600);
  set = true;
}



void loop(){
  while (gpsSerial.available() > 0){
    gps.encode(gpsSerial.read());}

  if(gps.location.isUpdated() && gps.location.lat()!= 0.0){
      v = gps.speed.mps();
      if(!set){
        if(v < 0.1  && millis()> 4000){ // if imobile, set home
          set = true;
          HOME_LAT = gps.location.lat();
          HOME_LNG = gps.location.lng();
          alt = gps.altitude.meters();
        }}
      else{
        dist = gps.distanceBetween(gps.location.lat(),gps.location.lng(),HOME_LAT,HOME_LNG);
        if(dist>20){ // dont turn if close to the waypoint
          Oz = gps.courseTo(gps.location.lat(),gps.location.lng(),HOME_LAT,HOME_LNG)-gps.course.deg();
          if (Oz>180){Oz-=360;}
          else if (Oz<-180){Oz+=360;}
          Oy = gps.altitude.meters()-alt+40; // fligh alt = 40 m
          if (Oy>10){Oy=10;}
          else if (Oy<-10){Oy = -10;}}
        
      }

    analogWrite(analogYaw,Oz);
    analogWrite(analogPitch,Oy);

}}