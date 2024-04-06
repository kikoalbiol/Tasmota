/*
  xdrv_101_my_project_with_commands.ino - My IoT device support for Tasmota
*/


#ifdef USE_MY_PROJECT_CMD
/*********************************************************************************************\
 * My IoT Device with command support
 *
 *
\*********************************************************************************************/

#warning **** My IoT Device Driver with commands is included... ****

#define XDRV_101 101


/*********************************************************************************************\
 * My IoT Device Functions
\*********************************************************************************************/

// This variable will be set to true after initialization
bool initSuccess = false;

/* 
  Optional: if you need to pass any command for your device 
  Commands are issued in Console or Web Console
  Commands:
    Say_Hello  - Only prints some text. Can be made something more useful...
    SendMQTT   - Send a MQTT example message
    Help       - Prints a list of available commands  
*/

const char PROJECT_CMDMyProjectCommands[] PROGMEM = "|"  // No Prefix
  "Say_Hello|" 
  "SendMQTT|"
  "Help|"
  "ConfigMatrix";

void (* const PROJECT_CMDMyProjectCommand[])(void) PROGMEM = {
  &PROJECT_CMDCmdSay_Hello, &PROJECT_CMDCmdSendMQTT, &PROJECT_CMDCmdHelp ,&PROJECT_CMDCONFIG};

void PROJECT_CMDCONFIG(void) {
    if (XdrvMailbox.data_len > 0) {
        if (XdrvMailbox.payload != 0) {
              String message = XdrvMailbox.data;
              AddLog(LOG_LEVEL_INFO, message.c_str());
        }
    }
    ResponseCmndDone();
}
void PROJECT_CMDCmdSay_Hello(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("Say_Hello: Hello!"));
  ResponseCmndDone();
}

char PROJECT_CMDpayload[200];
char PROJECT_CMDtopic[100];

void PROJECT_CMDCmdSendMQTT(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("Sending MQTT message."));

  snprintf_P(PROJECT_CMDtopic, sizeof(PROJECT_CMDtopic), PSTR("tasmota/myproject"));

  snprintf_P(PROJECT_CMDpayload, sizeof(PROJECT_CMDpayload), 
            PSTR("{\"" D_JSON_TIME "\":\"%s\",\"name\":\"My Project\"}"), 
            GetDateAndTime(DT_LOCAL).c_str()
  );

  // retain = true
  MqttPublishPayload(PROJECT_CMDtopic, PROJECT_CMDpayload, strlen(PROJECT_CMDpayload), false);

  ResponseCmndDone();
}

void PROJECT_CMDCmdHelp(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("Help: Accepted commands - Say_Hello, SendMQTT, Help"));
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Tasmota Functions
\*********************************************************************************************/



void PROJECT_CMDMyProjectInit()
{

  /*
    Here goes My Project setting.
    Usually this part is included into setup() function
  */


  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("My Project init..."));

  Serial.begin(115200);

  // Set initSuccess at the very end of the init process
  // Init is successful
  initSuccess = true;

}



void PROJECT_CMDMyProjectProcessing(void)
{

  /*
    Here goes My Project code.
    Usually this part is included into loop() function
  */

}






/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv101(uint32_t function)
{


  bool result = false;

  if (FUNC_INIT == function) {
    PROJECT_CMDMyProjectInit();
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("My project init is done..."));
  }
  else if (initSuccess) {

    switch (function) {
      // Select suitable interval for polling your function
//    case FUNC_EVERY_SECOND:
      case FUNC_EVERY_250_MSECOND:
//    case FUNC_EVERY_200_MSECOND:
//    case FUNC_EVERY_100_MSECOND:
        PROJECT_CMDMyProjectProcessing();
        break;

      // Command support
      case FUNC_COMMAND:
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Calling My Project Command..."));
        result = DecodeCommand(PROJECT_CMDMyProjectCommands, PROJECT_CMDMyProjectCommand);
        break;

    }

  }

  return result;
}

#endif  // USE_MY_PROJECT_CMD