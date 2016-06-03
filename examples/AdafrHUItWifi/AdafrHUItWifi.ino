// These need to be included when using standard Ethernet
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>

#include "AppleMidi.h"


char ssid[] = "WNSOM"; //  your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)

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
  WiFi.begin(ssid, pass);
  Serial.print("\nGetting IP address...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  // Serial communications and wait for port to open:

  Serial.println();
  Serial.print("IP address is ");
  Serial.println(WiFi.localIP());

  AppleMIDI.begin("DonFeather");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);
  
  AppleMIDI.OnReceiveNoteOn(OnMidiReceiveNoteOn);
  AppleMIDI.OnReceiveNoteOff(OnMidiReceiveNoteOff);
  AppleMIDI.OnReceiveControlChange(OnMidiReceiveControlChange);
  AppleMIDI.OnReceiveAfterTouchPoly(OnMidiReceiveAfterTouchPoly);
  AppleMIDI.OnReceiveProgramChange(OnMidiReceiveProgramChange);
  AppleMIDI.OnReceiveAfterTouchChannel(OnMidiReceiveAfterTouchChannel);
  AppleMIDI.OnReceivePitchBend(OnMidiReceivePitchBend);
  AppleMIDI.OnReceiveSystemExclusive(OnMidiReceiveSystemExclusive);
  AppleMIDI.OnReceiveTimeCodeQuarterFrame(OnMidiReceiveTimeCodeQuarterFrame);
  AppleMIDI.OnReceiveSongPosition(OnMidiReceiveSongPosition);
  AppleMIDI.OnReceiveSongSelect(OnMidiReceiveSongSelect);
  AppleMIDI.OnReceiveTuneRequest(OnMidiReceiveTuneRequest);
  AppleMIDI.OnReceiveClock(OnMidiReceiveClock);
  AppleMIDI.OnReceiveStart(OnMidiReceiveStart);
  AppleMIDI.OnReceiveContinue(OnMidiReceiveContinue);
  AppleMIDI.OnReceiveStop(OnMidiReceiveStop);
  AppleMIDI.OnReceiveActiveSensing(OnMidiReceiveActiveSensing);
  AppleMIDI.OnReceiveSystemReset(OnMidiReceiveSystemReset);




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

void OnMidiReceiveNoteOff(byte channel, byte note, byte velocity) {
  if (channel == 1 && note == 0 && velocity == 0) {
    AppleMIDI.noteOn(0, 127, 1);
    AppleMIDI.noteOff(0, 127, 1);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OnMidiReceiveNoteOn(byte channel, byte note, byte velocity) {
  if (channel == 1 && note == 0 && velocity == 0) {
    AppleMIDI.noteOn(0, 127, 1);
    AppleMIDI.noteOff(0, 127, 1);
  }
}

  void OnMidiReceiveTimeCodeQuarterFrame(byte data){
    Serial.printf("OnMidiReceiveTimeCodeQuarterFrame: %02x\n", data);
    }


  
  void OnMidiReceiveControlChange(byte channel, byte note, byte pressure){/*Serial.println("OnMidiReceiveControlChange");*/}  
  void OnMidiReceiveAfterTouchPoly(byte channel, byte note, byte pressure){Serial.println("OnMidiReceiveAfterTouchPoly");}
  void OnMidiReceiveProgramChange(byte channel, byte number){Serial.println("OnMidiReceiveProgramChange");}
  void OnMidiReceiveAfterTouchChannel(byte channel, byte pressure){Serial.println("OnMidiReceiveAfterTouchChannel");}
  void OnMidiReceivePitchBend(byte channel, int bend){Serial.println("OnMidiReceivePitchBend");}
  void OnMidiReceiveSystemExclusive(byte * array, byte size){Serial.println("OnMidiReceiveSystemExclusive");}

  void OnMidiReceiveSongPosition(unsigned int beats){Serial.println("OnMidiReceiveSongPosition");}
  void OnMidiReceiveSongSelect(byte songnumber){Serial.println("OnMidiReceiveSongSelect");}
  void OnMidiReceiveTuneRequest(){Serial.println("OnMidiReceiveTuneRequest");}
  void OnMidiReceiveClock(){Serial.println("OnMidiReceiveClock");}
  void OnMidiReceiveStart(){Serial.println("OnMidiReceiveStart");}
  void OnMidiReceiveContinue(){Serial.println("OnMidiReceiveContinue");}
  void OnMidiReceiveStop(){Serial.println("OnMidiReceiveStop");}
  void OnMidiReceiveActiveSensing(){Serial.println("OnMidiReceiveActiveSensing");}
  void OnMidiReceiveSystemReset(){Serial.println("OnMidiReceiveSystemReset");}
