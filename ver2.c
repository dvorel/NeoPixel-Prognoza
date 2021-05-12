#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_MPR121.h"

/*za tipku*/
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

/*za neopixel*/
#define PIXEL_PIN 2
#define PIXEL_COUNT 256
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

/*WIFI*/
char ssid[] = "IME";
char pass[] = "PASSWORD";

/*slike*/
long long wifi[] PROGMEM = {
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 
0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 
0xff0000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0xff0000, 
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

long long wifiCON[] PROGMEM = {
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 
0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 
0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x2dfc0c, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 
0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x01ff0c, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ff0d, 0x00ff0d, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

long long oblak[] PROGMEM  = {
0x000000, 0x000000, 0x999900, 0x000000, 0x999900, 0x000000, 0x999900, 0x000000, 0x151617, 0xffffff, 0xffffff, 0xffffff, 0x818282, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x666600, 0x999900, 0x999900, 0x999900, 0x76777a, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x616161, 0x000000, 0x000000, 
0x000000, 0x000000, 0x999900, 0x999900, 0x666600, 0x000000, 0xffffff, 0xffffff, 0x818282, 0x000000, 0x000000, 0x818282, 0xffffff, 0xffffff, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x999900, 0x000000, 0xffffff, 0x818282, 0x000000, 0x020101, 0x000000, 0x000000, 0x000000, 0x818282, 0xffffff, 0x616161, 0x000000, 
0x000000, 0x000000, 0x999900, 0x999900, 0xffffff, 0x818282, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, 0xffffff, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffffff, 0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x818282, 0xffffff, 0x818282, 
0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, 0x818282, 
0x000000, 0x000000, 0x000000, 0xffffff, 0xffffff, 0xd7d7d7, 0x696969, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xd7d7d7, 0xffffff, 0xffffff, 0x151617, 
0x000000, 0x000000, 0x000000, 0x000000, 0xd7d7d7, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xd7d7d7, 0x020101, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

long long grmljavina[] PROGMEM{  
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x717171, 0xffffff, 0x60605f, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x717171, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0x333333, 0x000000, 0x000000, 0x000000, 0xffffff, 0xffffff, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x040404, 0xffffff, 0xffffff, 0x000000, 0x000000, 0x040404, 0x010005, 0x130d0a, 0x000000, 0x1c1b1c, 0xffffff, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffffff, 0xffffff, 0x1c1b1c, 0x000000, 0x010005, 0x000000, 0x916125, 0xffcc33, 0x010005, 0x000000, 0xffffff, 0xffffff, 0x000000, 0x000000, 
0x000000, 0x545454, 0xffffff, 0x000000, 0x000000, 0x030101, 0x000002, 0x916125, 0xffcc33, 0x010108, 0x010005, 0x000000, 0x000000, 0xffffff, 0x636363, 0x000000, 
0x000000, 0x666666, 0xffffff, 0x000000, 0x000000, 0x000000, 0x090301, 0xffcc33, 0x916125, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, 0x636363, 0x000000, 
0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x282f36, 0x916125, 0xffcc33, 0xffcc33, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x717171, 0xffffff, 0x807f7f, 0x0d1726, 0x916125, 0xffcc33, 0x212325, 0x807f7f, 0x807f7f, 0x757576, 0x040404, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x916125, 0xffcc33, 0x916125, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000002, 0xffcc33, 0x916125, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

long long snijeg[] PROGMEM{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x00ffff, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x00ffff, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x00ffff, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x00ffff, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x00ffff, 0x000000, 0x000000, 0x000000, 0x000000, 
};

long long kisa[] PROGMEM{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x292626, 0x292626, 0x0f0f0f, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x292626, 0x292626, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x292626, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x333333, 0xffffff, 0xffffff, 0x292626, 0x292626, 0x000000, 0x292626, 0xffffff, 0x292626, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x292626, 0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x010201, 0x292626, 0xffffff, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x292626, 0xffffff, 0x292626, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x292626, 0xffffff, 0x292626, 0x000000, 0x000000, 
0x000000, 0x292626, 0xffffff, 0x292626, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x292626, 0xffffff, 0x292626, 0x000000, 
0x000000, 0xffffff, 0x292626, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, 0x292626, 0x000000, 
0x000000, 0x292626, 0xffffff, 0x000000, 0x0a1816, 0x00ffff, 0x000000, 0x0a1816, 0x00ffff, 0x000000, 0x0a1816, 0x00ffff, 0x292626, 0xffffff, 0x000000, 0x000000, 
0x000000, 0x010000, 0x292626, 0xffffff, 0x00ffff, 0x0a1816, 0x000000, 0x00ffff, 0x0a1816, 0x000000, 0x00ffff, 0x0a1816, 0xffffff, 0x292626, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x0a1816, 0x00ffff, 0x000000, 0x0a1816, 0x00ffff, 0x000000, 0x0a1816, 0x00ffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x00ffff, 0x0a1816, 0x000000, 0x00ffff, 0x0a1816, 0x000000, 0x00ffff, 0x0a1816, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

long long sunce[] PROGMEM{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x221600, 0x000000, 0x926900, 0x936900, 0x000000, 0x221600, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x170f00, 0xffa600, 0x140d00, 0x9a6d00, 0x9b6e00, 0x130c00, 0xffa600, 0x170f00, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x170f00, 0x000000, 0x6f4800, 0x170f00, 0x170f00, 0x170f00, 0x170f00, 0x6f4800, 0x000000, 0x170f00, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x291b00, 0xffa700, 0x724a00, 0x211500, 0xffa700, 0xffa800, 0xffa800, 0xffa700, 0x211500, 0x170f00, 0xffa700, 0x291b00, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x0c0800, 0x170f00, 0xffa700, 0xffaa00, 0x000000, 0x000000, 0xffaa00, 0xffa700, 0x170f00, 0x0c0800, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x906800, 0x9b6e00, 0x170f00, 0xffa800, 0x000000, 0x000000, 0x000000, 0x000000, 0xffa800, 0x170f00, 0x9b6e00, 0x906800, 0x000000, 0x000000, 
0x000000, 0x000000, 0x906800, 0x9b6e00, 0x170f00, 0xffa800, 0x000000, 0x000000, 0x000000, 0x000000, 0xffa800, 0x170f00, 0x9d6f00, 0x926900, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x0c0800, 0x170f00, 0xffa700, 0xffaa00, 0x000000, 0x000000, 0xffaa00, 0xffa700, 0x170f00, 0x0c0800, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x291b00, 0xffa700, 0x714900, 0x211500, 0xffa700, 0xffa800, 0xffa800, 0xffa700, 0x211500, 0x714900, 0xffa700, 0x291b00, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x170f00, 0x170f00, 0x714900, 0x170f00, 0x170f00, 0x170f00, 0x170f00, 0x6f4800, 0x000000, 0x170f00, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x170f00, 0xffa600, 0x140d00, 0x9a6d00, 0x9a6d00, 0x130c00, 0xffa600, 0x170f00, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x221600, 0x000000, 0x926900, 0x926900, 0x000000, 0x221600, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

byte brojevi_temp[10][15] = {
  {
    1, 1, 1, 
    1, 0, 1, 
    1, 0, 1, 
    1, 0, 1, 
    1, 1, 1 },
  {
    0, 0, 1, 
    0, 0, 1, 
    0, 0, 1, 
    0, 0, 1, 
    0, 0, 1 },
  {
    1, 1, 1, 
    0, 0, 1, 
    1, 1, 1, 
    1, 0, 0, 
    1, 1, 1 },
  {
    1, 1, 1, 
    0, 0, 1, 
    1, 1, 1, 
    0, 0, 1, 
    1, 1, 1 },
    {
    1, 0, 1, 
    1, 0, 1, 
    1, 1, 1, 
    0, 0, 1, 
    0, 0, 1 },
    {
    1, 1, 1, 
    1, 0, 0, 
    1, 1, 1, 
    0, 0, 1, 
    1, 1, 1 },
    {
    1, 1, 1, 
    1, 0, 0, 
    1, 1, 1, 
    1, 0, 1, 
    1, 1, 1 },
    {
    1, 1, 1, 
    0, 0, 1, 
    0, 0, 1, 
    0, 0, 1, 
    0, 0, 1 },
    {
    1, 1, 1, 
    1, 0, 1, 
    1, 1, 1, 
    1, 0, 1, 
    1, 1, 1 },
    {
    1, 1, 1, 
    1, 0, 1, 
    1, 1, 1, 
    0, 0, 1, 
    1, 1, 1 }
};

byte brojevi_sat[10][28] = {
  {
    1, 1, 1, 1, 
    1, 0, 0, 1, 
    1, 0, 0, 1, 
    1, 0, 0, 1,
    1, 0, 0, 1,
    1, 0, 0, 1, 
    1, 1, 1, 1 },
  {
    0, 0, 0, 1,
    0, 0, 1, 1,
    0, 1, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1 
    },
  {
    1, 1, 1, 1, 
    0, 0, 0, 1,
    0, 0, 0, 1, 
    1, 1, 1, 1, 
    1, 0, 0, 0,
    1, 0, 0, 0,
    1, 1, 1, 1 },
  {
    1, 1, 1, 1, 
    0, 0, 0 ,1,
    0, 0, 0, 1, 
    1, 1, 1, 1, 
    0, 0, 0, 1,
    0, 0, 0, 1, 
    1, 1, 1, 1 },
    {
    1, 0, 0, 1, 
    1, 0, 0 ,1,
    1, 0, 0 ,1, 
    1, 1, 1, 1, 
    0, 0, 0, 1,
    0, 0, 0, 1, 
    0, 0, 0, 1 },
    {
    1, 1, 1, 1, 
    1, 0, 0, 0,
    1, 0, 0, 0, 
    1, 1, 1, 1, 
    0, 0, 0, 1,
    0, 0, 0, 1, 
    1, 1, 1, 1 },
    {
    1, 1, 1, 1, 
    1, 0, 0, 0,
    1, 0, 0, 0, 
    1, 1, 1, 1,
    1, 0, 0, 1,
    1, 0, 0, 1, 
    1, 1, 1, 1 },
    {
    1, 1, 1, 1, 
    1, 0, 0, 1, 
    0, 0, 0, 1, 
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1, 
    0, 0, 0, 1 },
    {
    1, 1, 1, 1, 
    1, 0, 0, 1,
    1, 0, 0, 1, 
    1, 1, 1, 1, 
    1, 0, 0, 1,
    1, 0, 0, 1, 
    1, 1, 1, 1 },
    {
    1, 1, 1, 1, 
    1, 0, 0, 1,
    1, 0, 0, 1, 
    1, 1, 1, 1, 
    0, 0, 0, 1,
    0, 0, 0, 1, 
    1, 1, 1, 1 }
};

/*ostalo od prije*/
const long utcOffsetInSeconds = 7200;
int expiryTime = 5;

WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
HTTPClient http;
int status = WL_IDLE_STATUS;

/*globalne odredene varijable*/
long long frame[256];              //polje koje ce se prikazati na displayu
long boja_sat = 0x0307fc;          //boja za sat
long boja_temp = 0x19ff2c;         //default boja za temp, promjena s temp
const char server[] = "rssv-weather.herokuapp.com"; //server za dohvacanje prognoze
bool sat = false;                  //default prognoza, za sat: sat = true

const int satDelay = 30 * 1000;   //delay za dohvacanje/prikaz sata (10 sec) 
long prognozaDelay = 5 * 60000;       //delay za dohvacanje/prikaz prognoze (5 min)

/*ostale globalne*/
//unsigned long currentMillis;
unsigned long trenutanDelay = 1000;//pocetan delay (prvi loop nakon paljenja)
unsigned long lastUpdate = 0;
unsigned long lastTouched = 0;
int lastMinutes = -1;
int lastTemp = NULL;
String lastWeather = "";

//touch
bool capReady = false;
const int touchDelay = 1000;
uint16_t t;

//dohvacanje prognoze i sata
String response;
String weatherDescription;
int temperatura = 0;
unsigned long hours;
unsigned long minutes;


/*funkcije*/
void printWiFiStatus();
void obrisiSve();         //postavlja varijable na 0 da prisili update i prikaz na display
void obrisiFrame();         //postavlja sve u frame varijabli na crnu boju (ugasen pixel)
void oblakFrame();          //dodaje sliku oblaka u frame
void grmljavinaFrame();
void snijegFrame();
void kisaFrame();
void sunceFrame();          //sve iste kao za oblak samo druge slike, slike su definirane iznad
void DEMO();            //vrti sve slike
void pokaziPrognozu(String);    //prikaz prognoze na display ako se promjenila
void clockUpdate();         //dohvaca sat sa time servera
void pokaziSat(int, int);     //prikazuje sat
bool makehttpRequest();       //dohvaca prognozu sa servera
void bojaTemp(int);         //postavlja boju temperature ovisno o temperaturi
void dodajBroj(int x, int y, int broj, bool temp);//dodaje broj u frame varijablu
void pokaziFrame();         //pokazuje frame na display


void setup()
{ 
  Serial.begin(9600);
    
  strip.begin();
    strip.setBrightness(20);
    strip.show();

    wifiFrame();
    WiFi.begin(ssid, pass);
    Serial.println("connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
        expiryTime -= 1;
        if (expiryTime == 0)
        {
            break;
        }
    }
  
    Wire.begin();
  
    if (cap.begin(0x5A)) {
      Serial.println("MPR121 Pronaden");
      capReady = true;
    }
    wifiConFrame();
    delay(100);

    timeClient.begin();
    makehttpRequest();    
}

void loop(){  
  unsigned long currentMillis = millis();

  if( capReady && (currentMillis - lastTouched) > touchDelay){
    t = cap.touched();
    if(t == 1){
      lastTouched = currentMillis;
      t = 0;
      sat = !sat;
      //postavljanje na nulu da refresha display
      trenutanDelay = 1;
      lastTemp = NULL;
      lastWeather = "";
      lastMinutes = -1;
    }
  }
  
  if(sat){
    //ucitaj ako trebaupdate
    if((currentMillis - lastUpdate) >= trenutanDelay){
      lastUpdate = currentMillis;
      trenutanDelay = satDelay;
      
      clockUpdate(); 
      //Ispis trenutnog vremena na serial monitor
      Serial.print("h:");
      Serial.print(hours); //24-hr
      Serial.print("min:");
      Serial.print(minutes);
      //pokazi sat
      pokaziSat((int)hours, (int)minutes);
    }
  }
  else{
    //ucitaj ako treba
    if ((currentMillis - lastUpdate) > trenutanDelay){
      Serial.println("Request weather!");
      if(makehttpRequest() == false){
        //ako ne ucita prognozu kraci delay
        trenutanDelay = 10000;
        printWiFiStatus();
      }
      else{
        //prikaz prognoze i postavljanje delay-a
        trenutanDelay = prognozaDelay;
        lastUpdate = currentMillis;
        pokaziPrognozu(weatherDescription);
        }
    }
  }
  delay(10);
}

/*sat*/
void clockUpdate(){
  timeClient.update();
  hours = timeClient.getHours();
  minutes = timeClient.getMinutes();
}

void pokaziSat(int h, int m){  
  //promjena samo ako se promjenilo vrijeme
  if(m!=lastMinutes){
    obrisiFrame();
       //sati
    dodajBroj(2, 0, h/10, false);
    dodajBroj(8, 0, h%10, false);
    //minute
    dodajBroj(4, 9, m/10, false);
    dodajBroj(10, 9, m%10, false);    
    lastMinutes = m;
    pokaziFrame();
  }
}

/*prognoza*/
void pokaziPrognozu(String prognoza)
{
  //ako se nije promjenilo vrijeme ili temp nece dirati display
    if(prognoza != lastWeather || lastTemp != temperatura){
        obrisiFrame();  //postavi sve na 0
        if (prognoza == "Clouds"){
          oblakFrame();
        }
        else if (prognoza == "Clear"){
          sunceFrame();
        }
        else if (prognoza == "Rain"){
          kisaFrame();
        }
        else if (prognoza == "Drizzle"){
          kisaFrame();
        }
        else if (prognoza == "Snow"){
          snijegFrame();
        }
        else if (prognoza == "Thunderstorm"){
          grmljavinaFrame();
        }    

        bojaTemp(temperatura);
        dodajBroj(0, 11 , temperatura/10, true);
        dodajBroj(4, 11 , temperatura%10, true);
        lastWeather = prognoza;
        lastTemp = temperatura;
        pokaziFrame();
    }
}

void bojaTemp(int temp){  //boja za prikaz temperature, ovisno o temp
  if(temp< -7){
    boja_temp = 0x0000ff;
  }
  else if(temp < 5){
    boja_temp = 0x0084ff;    
  }
  else if(temp < 15){
    boja_temp = 0xffdb59; 
  }
  else if(temp < 25){
    boja_temp = 0xff7519;
  }
  else{
    boja_temp = 0xff0000;
  }  
}

/*dodavanje u frame*/
void obrisiFrame(){
  for(int i=0;i<256;i++){
    frame[i] = 0x000000;
  }
}
void wifiFrame(){
  int n = sizeof(wifi) / sizeof(wifi[0]);
  n--;
  for(int i = n;i>=0;i--){
    frame[i] = wifi[i];
  }
  pokaziFrame();
}

void wifiConFrame(){
  int n = sizeof(wifiCON) / sizeof(wifiCON[0]);
  n--;
  for(int i = n;i>=0;i--){
    frame[i] = wifiCON[i];
  }
  pokaziFrame();
}

void oblakFrame (){
  int n = sizeof(oblak) / sizeof(oblak[0]);
  n--;
  for(int i = n;i>=0;i--){
    frame[i] = oblak[i];
  }
}

void grmljavinaFrame(){
  int n = sizeof(grmljavina) / sizeof(grmljavina[0]);
  n--;
  for(int i = n;i>=0;i--){
    frame[i] = grmljavina[i];
  }
}

void snijegFrame(){
  int n = sizeof(snijeg) / sizeof(snijeg[0]);
  n--;
  for(int i = n;i>=0;i--){
    frame[i] = snijeg[i];
  }
}

void kisaFrame(){
  int n = sizeof(kisa) / sizeof(kisa[0]);
  n--;
  for(int i = n;i>=0;i--){
    frame[i] = kisa[i];
  }
}

void sunceFrame(){
  int n = sizeof(sunce) / sizeof(sunce[0]);
  n--;
  for(int i = n;i>=0;i--){
    frame[i] = sunce[i];
  }
}

void dodajBroj(int x, int y, int broj, bool temp){
  //x, y pocetna lokacija
  int X, Y, broj_n, frame_n;
  if(temp){
    X = 3;
    Y = 5;
  }
  else{
    X = 4;
    Y = 7;
  }
  
  for(int i=0;i<Y;i++){
    for(int j=0;j<X;j++){
      broj_n = i*X + j;
      frame_n = ((16 * y) + x) + (i*16 + j);
  
      if (frame_n<256){
        //broj za temp      
        if(temp){
          if(brojevi_temp[broj][broj_n]!=0){
            frame[frame_n] = boja_temp;}
        }            
        //broj za sat
        else{
          if(brojevi_sat[broj][broj_n]!=0){
            frame[frame_n] = boja_sat;}
          }     
        }         
      }      
    }    
}


void pokaziFrame(){   //frame na display
  int r, g, b;
  
  for(int i=0;i<16;i++){
    for(int j=0;j<16;j++){
      int grid_n = 255 - ((16*i) + j); //display je 'naopako' pa postavljam pixele od iza
      int n;

      //hex u rgb
      r = (int)frame[grid_n] >> 16;
      g = (int)frame[grid_n] >> 8 & 0xFF;
      b = (int)frame[grid_n] & 0xFF;

      //provjera koji je red
      if(i%2==0){
        n = (16*i)+(16-(j+1));
      }
      else{
        n = (16*i)+j;     
      }
  
      strip.setPixelColor(n, r, g, b);
    }
  }
  
  strip.show();  
}

void obrisiSve(){
  lastMinutes = -1;
  lastWeather = "";
  lastTemp = -100;
  lastUpdate = -1000;
}

void DEMO(){
  int D = 15000;
  //sunceFrame();
  pokaziFrame();
  delay(D); 
  kisaFrame();
  pokaziFrame();
  delay(D); 
  snijegFrame();
  pokaziFrame();
  delay(D); 
  oblakFrame();
  pokaziFrame();  
  delay(D);
  grmljavinaFrame();
  pokaziFrame();
  delay(D);   
}

// print Wifi status
void printWiFiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

// to request data from OWM
bool makehttpRequest()
{
    // close any connection before send a new request to allow client make connection to server
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://rssv-weather.herokuapp.com/getWeather"))
    {
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
    
        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                String payload = http.getString();
                Serial.println(payload);
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            return false;
        }
        http.end();
    }
    else
    {
        Serial.printf("[HTTP} Unable to connect\n");
        return false;
    }

    client.stop();

    // if there's a successful connection:
    if (client.connect(server, 80))
    {
        Serial.println("connecting...");
        // send the HTTP PUT request:
        client.println("GET /getWeather HTTP/1.1");
        client.println("Host: rssv-weather.herokuapp.com");
        client.println("User-Agent: ArduinoWiFi/1.1");
        client.println("Connection: close");
        client.println();

        unsigned long timeout = millis();
        while (client.available() == 0)
        {
            if (millis() - timeout > 5000)
            {
                Serial.println(">>> Client Timeout !");
                client.stop();
                return false;
            }
        }

        char c = 0;
        while (client.available())
        {
            c = client.read();
            response = response + c;
        }

        String data = response.substring(response.indexOf("!") + 1);
        //Serial.println("PODACI SA SERVERA: ");
        //Serial.println(data);

        int len = data.length();
        int commaIndex = data.indexOf(',');
        int delimIndex = data.indexOf(' ');
        
        weatherDescription = data.substring(0, commaIndex);
        temperatura = (data.substring(delimIndex, len)).toInt();

        Serial.println(temperatura);
        Serial.println(weatherDescription);
        return true;
    }
    else{
        // if no connction was made:
        Serial.println("connection failed");
        return false;
    }
    return true;
}
