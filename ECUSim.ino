//Miguel Pulido - Systems Architect
//Engine Control Unit (ECU) simulator

//Including CAN_BUS_libraries, headers
#include <SPI.h>
#include "mcp_can.h"
//#include <mcp_can_dfs.h>


//Declare Integer canId as 32bit
INT32U canId = 0x000;
//2024  4   65    12    18    248   185   147

//If you experience an error "CAN BUS init fail", change CS pin from D10 to D9 by default.
//Make this modification to "MCP_CAN CAN(9); 
//Set CS to pin 9﻿ 

// MCP_CAN(9)         // Set CS pin


const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

unsigned char len = 0;
unsigned char buf[8];
char str[20];

String BuildMessage="";
int MSGIdentifier=0;


void setup() {
// put your setup code here, to run once;

//Set CAN BUS baud rate
Serial.begin(38400);
//Initialize CAN_BUS
START_INIT:

    if(CAN_OK == CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }

}

void loop() {
  // put your main code here, to run repeatedly:

    // applies random number generator between two values for simulation
    char rndCoolantTemp=random(1,200);
    char rndRPM=random(1,55);
    char rndSpeed=random(0,255);
    char rndIAT=random(0,255);
    char rndMAF=random(0,255);
    char rndAmbientAirTemp=random(0,200);
    char rndCAT1Temp=random(1,55);

    //GENERAL ROUTINE
    unsigned char SupportedPID[8] =       {1,2,3,4,5,6,7,8};
    unsigned char MilCleared[7] =         {4, 65, 63, 34, 224, 185, 147};

    //SENSORS
    unsigned char CoolantTemp[7] =        {4, 65, 5, rndCoolantTemp, 0, 185, 147};
    unsigned char rpm[7] =                {4, 65, 12, rndRPM, 224, 185, 147};
    unsigned char vspeed[7] =             {4, 65, 13, rndSpeed, 224, 185, 147};
    unsigned char IATSensor[7] =          {4, 65, 15, rndIAT, 0, 185, 147};
    unsigned char MAFSensor[7] =          {4, 65, 16, rndMAF, 0, 185, 147};
    unsigned char AmbientAirTemp[7] =     {4, 65, 70, rndAmbientAirTemp, 0, 185, 147};
    unsigned char CAT1Temp[7] =           {4, 65, 60, rndCAT1Temp, 224, 185, 147};
    unsigned char CAT2Temp[7] =           {4, 65, 61, rndCAT1Temp, 224, 185, 147};
    unsigned char CAT3Temp[7] =           {4, 65, 62, rndCAT1Temp, 224, 185, 147};
    unsigned char CAT4Temp[7] =           {4, 65, 63, rndCAT1Temp, 224, 185, 147};

    if(CAN_MSGAVAIL == CAN.checkReceive())
    {

      CAN.readMsgBuf(&len, buf);
        canId = CAN.getCanId();
        Serial.print("<");Serial.print(canId);Serial.print(",");

        for(int i = 0; i<len; i++)
        {
          BuildMessage = BuildMessage + buf[i] + ",";
        }
        Serial.println(BuildMessage);

        //Check wich message was received.
        if(BuildMessage=="2,1,0,0,0,0,0,0,") {CAN.sendMsgBuf(0x7E8, 0, 8, SupportedPID);}
        if(BuildMessage=="2,1,1,0,0,0,0,0,") {CAN.sendMsgBuf(0x7E8, 0, 7, MilCleared);}

        //SEND SENSOR STATUSES
        if(BuildMessage=="2,1,5,0,0,0,0,0,") {CAN.sendMsgBuf(0x7E8, 0, 7, CoolantTemp);}
        if(BuildMessage=="2,1,12,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, rpm);}
        if(BuildMessage=="2,1,13,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, vspeed);}
        if(BuildMessage=="2,1,15,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, IATSensor);}
        if(BuildMessage=="2,1,16,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, MAFSensor);}
        if(BuildMessage=="2,1,70,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, AmbientAirTemp);}
        if(BuildMessage=="2,1,60,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, CAT1Temp);}
        if(BuildMessage=="2,1,61,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, CAT2Temp);}
        if(BuildMessage=="2,1,62,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, CAT3Temp);}
        if(BuildMessage=="2,1,63,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, CAT4Temp);}

        BuildMessage="";



    }


}
