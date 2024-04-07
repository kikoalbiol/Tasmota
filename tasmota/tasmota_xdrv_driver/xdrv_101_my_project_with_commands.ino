/*
  xdrv_101_my_project_with_commands.ino - My IoT device support for Tasmota
*/

#ifdef USE_I2C


#define XI2C_96                     1  // See I2CDEVICES.md

#ifdef USE_IFIC_ADC_GUALI1
/*********************************************************************************************\
 * My IoT Device with command support
 *
 *
\*********************************************************************************************/

#warning **** My IoT Device Driver with commands is included... ****

#define XDRV_101 101


uint8_t guali_address=0x44;

/*********************************************************************************************\
 * My IoT Device Functions
\*********************************************************************************************/

// This variable will be set to true after initialization
bool initSuccess = false;

char PROJECT_CMDpayload[200];
char PROJECT_CMDtopic[100];


/* 
  Optional: if you need to pass any command for your device 
  Commands are issued in Console or Web Console
  Commands:
    Say_Hello  - Only prints some text. Can be made something more useful...
    SendMQTT   - Send a MQTT example message
    Help       - Prints a list of available commands  
*/

int bmp_count=0;


void GualiDetect(void) {

    int i=0;

    if (!I2cSetDevice(guali_address)) 
    { return; }
    
    I2cSetActiveFound(guali_address, "IFIC_ADC_GUALI1");
    AddLog(LOG_LEVEL_INFO, PSTR("GUALI GualiDetect..."));
    bmp_count++;
}
  


const char PROJECT_CMDMyProjectCommands[] PROGMEM = "|"  // No Prefix
  "Say_Hello|" 
  "SendMQTT|"
  "Help|"
  "ConfigMatrix|"
  "ConfigRawMatrix";

void (* const PROJECT_CMDMyProjectCommand[])(void) PROGMEM = {
  &PROJECT_CMDCmdSay_Hello, &PROJECT_CMDCmdSendMQTT, &PROJECT_CMDCmdHelp ,&PROJECT_CMDCONFIG,&PROJECT_RAW_CMDCONFIG};

uint8_t buff_data[512];
uint8_t start=0;
uint8_t lenght=0;

void PROJECT_CMDCONFIG(void) {
    bool done=false;
    if (XdrvMailbox.data_len > 0) {
        if (XdrvMailbox.payload != 0) {
              String message = XdrvMailbox.data;
              AddLog(LOG_LEVEL_INFO, message.c_str());
              JsonParser parser(XdrvMailbox.data);
              
              if(parser)
              {
                   JsonParserObject root = parser.getRootObject();
                   auto data_d=root[PSTR("data")];
                   snprintf_P(PROJECT_CMDtopic, sizeof(PROJECT_CMDtopic), PSTR("tasmota/RAWPROJECT_CMDCONFIG"));
                   AddLog(LOG_LEVEL_INFO,"ROOT IS_OBJ");
                   if (data_d.isArray()) {
                      auto start_d=root[PSTR("start")];
                      AddLog(LOG_LEVEL_INFO,"DATA IS_ARRAY");
                      if(start_d){
                         auto array=data_d.getArray();
                         uint8_t start=start_d.getInt();
                         AddLog(LOG_LEVEL_INFO,"START IS_INT");
                         uint8_t buffsize=std::min<uint8_t>(array.size(),512-start);
                         for(int i=0;i<buffsize;i++)
                         {
                             buff_data[i+start]=array[i].getInt();
                             lenght=std::max<uint8_t>(lenght,i+start);
                         }
                         snprintf_P(PROJECT_CMDtopic, sizeof(PROJECT_CMDtopic), PSTR("tasmota/USE_IFIC_ADC_GUALI1/%u"),buffsize);
                         done=true;
                         auto commit=root[PSTR("commit")];
                         if (commit) {
                           if(commit.getBool())
                               I2cWriteBuffer(guali_address,buff_data[0],buff_data+1,lenght-1);
                          snprintf_P(PROJECT_CMDtopic, sizeof(PROJECT_CMDtopic), PSTR("tasmota/USE_IFIC_ADC_GUALI1/%u"),lenght);
                          lenght=0;
                         }
                      }
                   }
                   else
                   {
                         AddLog(LOG_LEVEL_ERROR,"{data: [],start: int, [commit, bool]}");
                   }
                      
              }
              else
              {
                     AddLog(LOG_LEVEL_ERROR,"NOT PARSED");
              }
              MqttPublishPayload(PROJECT_CMDtopic, PROJECT_CMDpayload, strlen(PROJECT_CMDpayload), false);

        }
        if(done)
          ResponseCmndDone();
    }
}

void PROJECT_RAW_CMDCONFIG(void) {
    bool done=false;
    if (XdrvMailbox.data_len > 0) {
        if (XdrvMailbox.payload != 0) {
              String message = XdrvMailbox.data;
              AddLog(LOG_LEVEL_INFO, message.c_str());
              JsonParser parser(XdrvMailbox.data);
              JsonParserToken root = parser.getRoot();
              snprintf_P(PROJECT_CMDtopic, sizeof(PROJECT_CMDtopic), PSTR("tasmota/PROJECT_CMDCONFIG"));
              if(root.isArray())
              {
                      AddLog(LOG_LEVEL_INFO,"ISARRAY");
                      auto array=root.getArray();
                      auto buffsize=std::min<int>(array.size(),512);
                      for(int i=0;i<buffsize;i++)
                      {
                          buff_data[i]=array[i].getInt();
                      }
                      snprintf_P(PROJECT_CMDtopic, sizeof(PROJECT_CMDtopic), PSTR("tasmota/USE_IFIC_ADC_GUALI1/%u"),buffsize);
                      done=true;
                      
              }
              else
              {
                     AddLog(LOG_LEVEL_ERROR,"NOT ARRAY");
              }
              MqttPublishPayload(PROJECT_CMDtopic, PROJECT_CMDpayload, strlen(PROJECT_CMDpayload), false);

        }
        if(done)
          ResponseCmndDone();
    }
}


void PROJECT_CMDCmdSay_Hello(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("USE_IFIC_ADC_GUALI1"));
  ResponseCmndDone();
}


void PROJECT_CMDCmdSendMQTT(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("Sending MQTT message."));

  snprintf_P(PROJECT_CMDtopic, sizeof(PROJECT_CMDtopic), PSTR("tasmota/USE_IFIC_ADC_GUALI1"));

  snprintf_P(PROJECT_CMDpayload, sizeof(PROJECT_CMDpayload), 
            PSTR("{\"" D_JSON_TIME "\":\"%s\",\"name\":\"USE_IFIC_ADC_GUALI1\"}"), 
            GetDateAndTime(DT_LOCAL).c_str()
  );

  // retain = true
  MqttPublishPayload(PROJECT_CMDtopic, PROJECT_CMDpayload, strlen(PROJECT_CMDpayload), false);

  ResponseCmndDone();
}

void PROJECT_CMDCmdHelp(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("Help: Accepted commands - Say_Hello, SendMQTT, Help, ConfigMatrix, ConfigRawMatrix"));
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


  AddLog(LOG_LEVEL_INFO, PSTR("GUALI init..."));

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



void BmpShow(bool json) {

      char name[16]="IFIC_GUALI_ADC";

#ifdef USE_WEBSERVER
        WSContentSend_PD(name);
#endif  // USE_WEBSERVER
}





bool my_firstime=true;
bool my_firstime_first_log=true;

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv101(uint32_t function)
{

  if (!I2cEnabled(XI2C_96)) { 
    if(my_firstime_first_log)
         AddLog(LOG_LEVEL_INFO, PSTR("I2C GUALI NOT ENABLED"));
    my_firstime_first_log=false;
    return false; 
  }
  else if(my_firstime)
  { 
    AddLog(LOG_LEVEL_INFO, PSTR("I2C Xdrv101 GUALI DRIVER ENABLED"));
    my_firstime=false;
  }

  bool result = false;

  if (FUNC_INIT == function) {
    GualiDetect();
    PROJECT_CMDMyProjectInit();
    AddLog(LOG_LEVEL_INFO, PSTR("I2C GUALI CONFIGURED"));
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
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        BmpShow(0);
        break;
#endif  // USE_WEBSERVER

      // Command support
      case FUNC_COMMAND:
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Calling My Project Command..."));
        result = DecodeCommand(PROJECT_CMDMyProjectCommands, PROJECT_CMDMyProjectCommand);
        break;

    }

  }

  return result;
}

#endif  // USE_IFIC_ADC_GUALI1
#endif