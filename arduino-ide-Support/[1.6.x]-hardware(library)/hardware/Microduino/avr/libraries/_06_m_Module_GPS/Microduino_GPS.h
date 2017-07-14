// 源自Adafruit_GPS库
// 修订：@老潘orz  wasdpkj@hotmail.com
// 支持GPS模块：Microduino_GPS（NEO6M、NEOM8N）
// 支持自适应波特率

/***********************************
  This is our GPS library

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above must be included in any redistribution
****************************************/

#ifndef _MICRODUINO_GPS_H_
#define _MICRODUINO_GPS_H_

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#include "SoftwareSerial.h"


#define CONFIG_DEFAULT 0
#define CONFIG_SAVE 1

#define UPDATE_1HZ 0
#define UPDATE_2HZ 1
#define UPDATE_4HZ 2
#define UPDATE_5HZ 3

#define GPS_SBAS 		0
#define BEIDOU_SBAS	 	1
#define QZSS_SBAS 		2
#define GLONASS_SBAS	3

#define CONTINUOUS 0
#define POWERSAVE 1

// how long to wait when we're looking for a response
#define MAXWAITSENTENCE 5


class Microduino_GPS {
 public:
  void begin(uint32_t baud); 

  Microduino_GPS(SoftwareSerial *ser); // Constructor when using SoftwareSerial
  Microduino_GPS(HardwareSerial *ser); // Constructor when using HardwareSerial

  char *lastNMEA(void);
  boolean newNMEAreceived();
  void common_init(void);
  void pause(boolean b);

  uint8_t parseHex(char c);

  char read(void);
  boolean parse(char *);

//  boolean wakeup(void);
// boolean standby(void);

  uint8_t hour, minute, seconds, year, month, day;
  uint16_t milliseconds;
  float latitude, longitude, geoidheight, altitude;
  float speed, angle, magvariation, HDOP;
  char lat, lon, mag;
  boolean fix;
  uint8_t fixquality, satellites;


  uint16_t LOCUS_serial, LOCUS_records;
  uint8_t LOCUS_type, LOCUS_mode, LOCUS_config, LOCUS_interval, LOCUS_distance, LOCUS_speed, LOCUS_status, LOCUS_percent;
  
  void set_config(uint8_t set_config);
  void set_updata(uint8_t _set_updata);
  void set_baud(uint32_t _set_baud);
  void set_cnssmode(uint8_t _set_cnssmode);
  void set_powermode(uint8_t _set_powermode);

 private:
  boolean paused;
  
  uint8_t parseResponse(char *response);
  SoftwareSerial *gpsSwSerial;
  HardwareSerial *gpsHwSerial;	
  
  void rx_empty(void);
};


#endif
