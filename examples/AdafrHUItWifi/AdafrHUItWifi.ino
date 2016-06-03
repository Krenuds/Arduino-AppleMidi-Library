// These need to be included when using standard Ethernet
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include "AppleMidi.h"

#define RED_LED 14
#define GREEN_LED 13
#define BLUE_LED 12


char ssid[] = "WNSOM"; //  your network SSID (name)
char pass[] = "1westnewburyschoolofmusic";    // your network password (use for WPA, or use as key for WEP)

//char ssid[] = "NETGEAR53"; //  your network SSID (name)
//char pass[] = "roundpond693";    // your network password (use for WPA, or use as key for WEP)

unsigned long t0 = millis();
bool isConnected = false;

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  Serial.print("\nGetting IP address...");

WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
WiFi.mode(WIFI_STA);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);


  // Serial communications and wait for port to open:
  KillLEDs ();
  Serial.println();
  Serial.print("IP address is ");
  Serial.println(WiFi.localIP());

  AppleMIDI.begin("Feather");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

  AppleMIDI.OnReceiveNoteOn(OnAppleMidiNoteOn);
  AppleMIDI.OnReceiveNoteOff(OnAppleMidiNoteOff);
  AppleMIDI.OnReceiveControlChange(OnMidiControlChange);
  AppleMIDI.OnReceiveProgramChange(OnMidiProgramChange);





}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop() {
  AppleMIDI.run();
}
void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  isConnected  = true;
  Serial.print("Connected to session ");
  Serial.println(name);
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected  = false;
  Serial.println("Disconnected");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOn from channel:");
  Serial.print(channel);
  Serial.print(" note:");
  Serial.print(note);
  Serial.print(" velocity:");
  Serial.print(velocity);
  Serial.println();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
  if (channel == 1 && note == 0 && velocity == 0) {
    AppleMIDI.noteOn(0, 127, 1);
    AppleMIDI.noteOff(0, 127, 1);
  }
  else {
    Serial.print("Incoming NoteOff from channel:");
    Serial.print(channel);
    Serial.print(" note:");
    Serial.print(note);
    Serial.print(" velocity:");
    Serial.print(velocity);
    Serial.println();
  }

}

void OnMidiControlChange(byte channel, byte note, byte velocity) {
  if (channel = 1) {
    if (note = 44) {
      if (velocity == 67)
        digitalWrite (RED_LED, HIGH);
    }
  }
//  Serial.print("Incoming CC from channel:");
//  Serial.print(channel);
//  Serial.print(" note:");
//  Serial.print(note);
//  Serial.print(" velocity:");
//  Serial.print(velocity);
//  Serial.println();
}

void OnMidiProgramChange(byte channel, byte note) {
  //Serial.print("Incoming Prog Change from channel:");
  //Serial.print(channel);
  //Serial.print(" note:");
  //Serial.print(note);
  //Serial.println();
}


void KillLEDs() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}
