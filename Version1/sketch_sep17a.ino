// Dashboard 2025 Code

// These are the libraries:
#include <CAN.h> // necessary for CAN Protocal communication commands
#include <SPI.h> // necessary for serial communication bewteen the SPI devices the MicroController 
                 // (lowkey might already be automatically downloaded)

// Define the pins betweeen the MCP2515 Board and the MicroController
#define MISO_Pin 12 // might need to rename w/ new update: MISO -> CIPO
#define MOSI_Pin 11 // might need to rename w/ new update: MOSI -> COPI
#define CS_Pin 10 
#define SCK_Pin 13
#define INTRPT_Pin 0 // or 9

// Introduce the variables (data metrics)
float voltage;
float coolTemp;
float engnSpeed;
float wheelSpeed;

bool can_start = 1;

void setup() {

  // Input, Output pins
  pinMode(CS_Pin, OUTPUT); 
  pinModeINTRPT_Pin, INPUT); // can library attaches its own interrup to pin

  // Configure CAN Library
  CAN.setPins(CS_Pin, INTRPT_Pin);
  Serial.begin(9600);
  Serial1.begin(9600); // Pick between the two
  while (!Serial);
  Serial.println("CAN Receiver Callback");
  CAN.setSPIFrequency(100000); //max MCP is 10MHz
  CAN.setClockFrequency(8E6)
  
  // Start the CAN bus at 500 kbps
  if (!CAN.begin(250E3)) {
    Serial.println("Startign CAN failed!");
    while (1);
  } else {
    Serial.println("CAN Started");
    can_start = 0;
  }

  // Register the receive callback
  CAN.onReceive(onReceive);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (can_start){
    while(!CAN.begin(250E3)){
      Serial.println("CAN Failed")
      delay(100);
    }
  } else {
    can_start = 0;
  }
  // Read, reformat, and print data to Dashboard
}

void onReceive(int packetSize) {
  // received a packet
  //// // Serial.println("hello");
  // Serial.println(CAN.packetId());
  if(CAN.packetId() == 0x701) {
    Serial.print("Received at 0x701");
    
    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }
    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }
    Serial.print("packet with id ");
    Serial.print(CAN.packetId());
    Serial.print(" and as hex ");
    Serial.print(CAN.packetId(), HEX);
    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);
      // only print packet data for non-RTR packets
      union data {
        uint32_t bits;
        float number;
      };
      union data t;
      t.bits = 0;
      int i = 0;
      while (CAN.available()) {
        uint32_t j = CAN.read();
        if(i < 4) {
          t.bits = (t.bits << 8) + j;
          Serial.print(j,HEX);
          Serial.print(" ");
          i++;
        }
      }
      // Serial.println();
      // Serial.print(t.number);
      voltage = t.number;
      // Serial.println();
    }
    // Serial.println();
  }
  if(CAN.packetId() == 0x700) {
    Serial.print("Received at 0x700");
    if (CAN.packetExtended()) {
      // Serial.print("extended ");
    }
    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      // Serial.print("RTR ");
    }
    // Serial.print("packet with id ");
    // Serial.print(CAN.packetId());
    //// Serial.print(CAN.packetId(), HEX);
    if (CAN.packetRtr()) {
      // Serial.print(" and requested length ");
      // Serial.println(CAN.packetDlc());
    } else {
      // Serial.print(" and length ");
      // Serial.println(packetSize);
      // only print packet data for non-RTR packets
      union data {
        uint32_t bits;
        float number;
      };
      union data t;
      t.bits = 0;
      int i = 0;
      while (CAN.available()) {
        uint32_t j = CAN.read();
        if(i < 4) {
          t.bits = (t.bits << 8) + j;
          // Serial.print(j,HEX);
          // Serial.print(" ");
          i++;
        }
      }
      // Serial.println();
      // Serial.print(t.number);
      coolTemp = t.number;
      // Serial.println();
    }
    // Serial.println();
  }
  if(CAN.packetId() == 0x702) {
    // Serial.print("Received ");
    if (CAN.packetExtended()) {
      // Serial.print("extended ");
    }
    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      // Serial.print("RTR ");
    }
    // Serial.print("packet with id ");
    // Serial.print(CAN.packetId());
    //// Serial.print(CAN.packetId(), HEX);
    if (CAN.packetRtr()) {
      // Serial.print(" and requested length ");
      // Serial.println(CAN.packetDlc());
    } else {
      // Serial.print(" and length ");
      // Serial.println(packetSize);
      // only print packet data for non-RTR packets
      union data {
        uint32_t bits;
        float number;
      };
      union data t;
      t.bits = 0;
      int i = 0;
      while (CAN.available()) {
        uint32_t j = CAN.read();
        if(i < 4) {
          t.bits = (t.bits << 8) + j;
          // Serial.print(j,HEX);
          // Serial.print(" ");
          i++;
        }
      }
            // Serial.println();
      // Serial.print(t.number);
      engnSpeed = t.number / 6;
      // Serial.println();
    }
    // Serial.println();
  }
  if(CAN.packetId() == 0x703) {
    // Serial.print("Received ");
    if (CAN.packetExtended()) {
      // Serial.print("extended ");
    }
    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      // Serial.print("RTR ");
    }
    // Serial.print("packet with id ");
    // Serial.print(CAN.packetId());
    //// Serial.print(CAN.packetId(), HEX);
    if (CAN.packetRtr()) {
      // // Serial.print(" and requested length ");
      // // Serial.println(CAN.packetDlc());
    } else {
      // // Serial.print(" and length ");
      // // Serial.println(packetSize);
      // only print packet data for non-RTR packets
      union data {
        uint32_t bits;
        float number;
      };
      union data t;
      t.bits = 0;
      int i = 0;
      while (CAN.available()) {
        uint32_t j = CAN.read();
        if(i < 4) {
          t.bits = (t.bits << 8) + j;
          // // Serial.print(j,HEX);
          // // Serial.print(" ");
          i++;
        }
      }
      // // Serial.println();
      // // Serial.print(t.number);
      wheelSpeed = t.number;
      // // Serial.println();
    }
    // // Serial.println();
  }
}
