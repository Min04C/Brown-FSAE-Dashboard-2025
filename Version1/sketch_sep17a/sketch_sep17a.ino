// Dashboard 2025 Code
#include <mcp_can.h> // necessary for CAN Protocal communication commands
#include <SPI.h> // necessary for serial communication bewteen the SPI devices the MicroController 
                 // (lowkey might already be automatically downloaded)

// Define the pins betweeen the MCP2515 Board and the MicroController
//#define MISO_Pin 12 // might need to rename w/ new update: MISO -> CIPO
//#define MOSI_Pin 11 // might need to rename w/ new update: MOSI -> COPI
//#define SCK_Pin 13
#define CS_Pin 10 
#define INTRPT_Pin 9 // or 0


// Introduce the variables (data metrics)
float voltage;
float coolTemp;
float engnSpeed;
float wheelSpeed;

bool can_start = true; // 1=true

void setup() {

  MCP_CAN CAN(CS_Pin);  
  // Configure CAN Library
  Serial.begin(9600);
  while (!Serial); 
  
  Serial.println("CAN Receiver Callback");
  CAN.setSPIFrequency(100000); //max MCP is 10MHz
  CAN.setClockFrequency(8E6)
  
  if (CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN Initialized Successfully!");
    CAN.setMode(MCP_NORMAL);  // Set CAN to normal mode for receiving
    can_start = true;
  } else {
    Serial.println("Error Initializing CAN...");
    can_start = false;
    while (1);  // Hang if initialization fails
  }
  // Register the receive callback
  CAN.onReceive(onReceive);
}

void loop() {
  // Re-attempt initialization if CAN fails
  if (!can_start) {
    if (CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) {
      Serial.println("CAN Re-initialized Successfully!");
      CAN.setMode(MCP_NORMAL);
      can_start = true;
    }
  }

  // READ: Check for incoming CAN messages
  if (CAN.checkReceive() == CAN_MSGAVAIL) {
    readCANMessage();  // Process the incoming message
  } else {
    Serial.println("No CAN message available");
  }

  // REFORMAT
}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("onReceived function opened packet ID: ");
  Serial.println(CAN.packetId());
  Serial.print(" and as hex ");
  Serial.print(CAN.packetId(), HEX);

  if (CAN.packetId() == 0x700) {        Serial.println("COOLTEMPPPPPPPP");
  } else if (CAN.packetId() == 0x701) { Serial.println("VOLTAGEEEEEEEEE");
  } else if (CAN.packetId() == 0x702) { Serial.println("ENGNSPEEDDDDDDD");
  } else if (CAN.packetId() == 0x703) { Serial.println("WHEELSPEEDDDDDD"); }

  if (CAN.packetExtended()) {
      Serial.print("extended ");
  }
  if (CAN.packetRtr()) {
    // Remote transmission request, packet contains no data
    Serial.print("RTR and requested length ");
    Serial.println(CAN.packetDlc());
  } else { // only print packet data for non-RTR packets 
    Serial.print(" and length ");
    Serial.println(packetSize);
    // convert CAN buffer to float
    union data {
      uint32_t bits;
      float number;
    } t;
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
      
    Serial.println();
    Serial.print(t.number);
    if (CAN.packetId() == 0x700) {        voltage = t.number;   
    } else if (CAN.packetId() == 0x701) { voltage = t.number;
    } else if (CAN.packetId() == 0x702) { engnSpeed = t.number / 6;
    } else if (CAN.packetId() == 0x703) { wheelSpeed = t.number; }
    voltage = t.number;
    Serial.println();
  }
  Serial.println();
}

// Function to read and process incoming CAN messages
void readCANMessage() {
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char buf[8];

  CAN.readMsgBuf(&rxId, &len, buf);  // Read CAN message

  // Process CAN message based on ID
  if (rxId == 0x701) {
    voltage = extractFloatFromBuffer(buf);
  } else if (rxId == 0x700) {
    coolTemp = extractFloatFromBuffer(buf);
  } else if (rxId == 0x702) {
    engnSpeed = extractFloatFromBuffer(buf) / 6;  // Convert engine speed
    Serial.print("Engine Speed (RPM): ");
    Serial.println(engnSpeed); // Debug output for RPM
  } else if (rxId == 0x703) {
    wheelSpeed = extractFloatFromBuffer(buf);
  }
}


// Helper function to convert CAN buffer to float
float extractFloatFromBuffer(unsigned char* buf) {
  union {
    uint32_t bits;
    float number;
  } data;

  data.bits = 0;
  for (int i = 0; i < 4; i++) {
    data.bits = (data.bits << 8) | buf[i];
  }
  return data.number;
}
