/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/


// -- Master parameter control --------------------
#undef  CFG_HOLDER
#define CFG_HOLDER        4617                   // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// -- Setup your own Wifi settings  ---------------
#undef  STA_SSID1
#define STA_SSID1         "ESTOESUNAKASADELOKOS"             // [Ssid1] Wifi SSID

#undef  STA_PASS1
#define STA_PASS1         "erestu640"     // [Password1] Wifi password

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#define MQTT_HOST         "mqtt.kakiko.esp" // [MqttHost]

#undef  MQTT_PORT
#define MQTT_PORT         1883                   // [MqttPort] MQTT port (10123 on CloudMQTT)

#undef  MQTT_USER
#define MQTT_USER         "client"         // [MqttUser] Optional user

#undef  MQTT_PASS
#define MQTT_PASS         "client"         // [MqttPassword] Optional password


// Own Configuration goes here
#ifdef PROJECT
#undef PROJECT
#define PROJECT           "SESOR_I2C_IFIC"       // PROJECT is used as the default topic delimiter
#endif 


#define TI_DEBUG 1

//#define USE_MY_PROJECT  // Include your custom code into Tasmota
//#define USE_MY_PROJECT_EX3
//#define USE_MY_PROJECT
#define 
#ifdef USE_IFIC_ADC_GUALI1
 #undef USE_IFIC_ADC_GUALI1
#endif
#define USE_IFIC_ADC_GUALI1



#ifdef USE_LD2410
 #undef USE_LD2410
#endif
#define USE_LD2410



#endif  // _USER_CONFIG_OVERRIDE_H_
