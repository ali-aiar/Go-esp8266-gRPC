#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <test.pb.h>

#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
WiFiClient client;
IPAddress Ip (192,168,100,109);
TestMessage e;

void sendTemp(TestMessage sende) {
  uint8_t buffer[128];
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  
  if (!pb_encode(&stream, TestMessage_fields, &sende)){
    Serial.println("failed to encode temp proto");
    Serial.println(PB_GET_ERROR(&stream));
    return;
  }
  
  client.write(buffer, stream.bytes_written);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin("WIFI BIZNET", "samakykdibawah");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
   if (!client.connect(Ip, 3333)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec to reconnect...");
    delay(5000);
    return;
  }

  e=TestMessage_init_zero;
  e.test_number=656;
  sendTemp(e);

  if (client.available()) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec to reconnect...");
    delay(5000);
    return;
  }
}

// void setup() {

//   Serial.begin(115200);

//   uint8_t buffer[128];

//   TestMessage message = TestMessage_init_zero;

//   pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

//   message.test_number = 540;

//   bool status = pb_encode(&stream, TestMessage_fields, &message);

//   if (!status)
//   {
//       Serial.println("Failed to encode");
//       return;
//   }

//   Serial.print("Message Length: ");
//   Serial.println(stream.bytes_written);

//   Serial.print("Message: ");

//   for(int i = 0; i<stream.bytes_written; i++){
//     Serial.printf("%02X",buffer[i]);
//   }
// }

// void loop() {}