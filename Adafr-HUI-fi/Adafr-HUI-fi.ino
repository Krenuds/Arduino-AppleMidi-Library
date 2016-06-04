// These need to be included when using standard Ethernet
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include "AppleMidi.h"

#define STOP_THRESHOLD 100 //time in ms

#define RED_LED 13
#define GREEN_LED 12
#define BLUE_LED 14


char ssid[] = "+++++"; //  your network SSID (name)
char pass[] = "+++++";    // your network password (use for WPA, or use as key for WEP)

unsigned long t0 = millis();
unsigned long lastFrameTime = millis();

bool isConnected = false;
bool tapeRolling = false;


APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
  Serial.print("\nGetting IP address...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(GREEN_LED, HIGH);
  // Serial communications and wait for port to open:

  Serial.println();
  Serial.print("IP address is ");
  Serial.println(WiFi.localIP());

  AppleMIDI.begin("FeatherWifi");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);
  AppleMIDI.OnReceiveTimeCodeQuarterFrame(OnMidiReceiveTimeCodeQuarterFrame);

  AppleMIDI.OnReceiveSystemExclusive(OnMidiReceiveSystemExclusive);
 /* 
  AppleMIDI.OnReceiveNoteOn(OnMidiReceiveNoteOn);
  AppleMIDI.OnReceiveNoteOff(OnMidiReceiveNoteOff);
  AppleMIDI.OnReceiveControlChange(OnMidiReceiveControlChange);
  AppleMIDI.OnReceiveAfterTouchPoly(OnMidiReceiveAfterTouchPoly);
  AppleMIDI.OnReceiveProgramChange(OnMidiReceiveProgramChange);
  AppleMIDI.OnReceiveAfterTouchChannel(OnMidiReceiveAfterTouchChannel);
  AppleMIDI.OnReceivePitchBend(OnMidiReceivePitchBend);


  AppleMIDI.OnReceiveSongPosition(OnMidiReceiveSongPosition);
  AppleMIDI.OnReceiveSongSelect(OnMidiReceiveSongSelect);
  AppleMIDI.OnReceiveTuneRequest(OnMidiReceiveTuneRequest);
  AppleMIDI.OnReceiveClock(OnMidiReceiveClock);
  AppleMIDI.OnReceiveStart(OnMidiReceiveStart);
  AppleMIDI.OnReceiveContinue(OnMidiReceiveContinue);
  AppleMIDI.OnReceiveStop(OnMidiReceiveStop);
  AppleMIDI.OnReceiveActiveSensing(OnMidiReceiveActiveSensing);
  AppleMIDI.OnReceiveSystemReset(OnMidiReceiveSystemReset);
*/



}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop() {
  AppleMIDI.run();
  tapeRolling = HandlePlayback(millis());
  if(WiFi.status() != WL_CONNECTED) {
    digitalWrite(GREEN_LED, LOW);
  }

}
void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  isConnected  = true;
  Serial.print("Connected to session ");
  Serial.println(name);
}

bool HandlePlayback(unsigned long currentTime) {
  if (currentTime - lastFrameTime > STOP_THRESHOLD) {
    if(tapeRolling) {
      Serial.println("Tape Stopped");
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(RED_LED, LOW);      
      }

    return false;
    }
    if (!tapeRolling) {
      Serial.println("Tape Rolling"); 
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(RED_LED, HIGH);   
    }

    return true;
  }


// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  digitalWrite(BLUE_LED, LOW);
  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(GREEN_LED, HIGH);  
  }
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
  lastFrameTime = millis();
}

void OnMidiReceiveSystemExclusive(byte * dataBuffer, unsigned int len){
  Serial.printf("OnMidiReceiveSystemExclusive: ");

  while(*dataBuffer) {
    Serial.printf("%x ", *dataBuffer);
    dataBuffer++;
    }
    Serial.println();
}


  /*
  void OnMidiReceiveControlChange(byte channel, byte note, byte pressure){Serial.println("OnMidiReceiveControlChange");}  
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
  */


