#include <SPI.h>
#include <FreeStack.h>
#include <SdFat.h>
#include <vs1053_SdFat.h>

#define LIGHT_PIN 5
#define SWITCH_PIN 4
#define PARTY_DURATION_MS 20000

SdFat sd;
vs1053 MP3player;

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);

  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) { 
    sd.initErrorHalt();
  }

  if(!sd.chdir("/")) { 
    sd.errorHalt("sd.chdir");
  }

  MP3player.begin();
  MP3player.setDifferentialOutput(1);
}

bool isPartyActive = false;
long partyStaredAtMs = 0;

void loop() {
  if (!isPartyActive) {
    int switchState = digitalRead(SWITCH_PIN);

    if (switchState == HIGH) {
      partyStaredAtMs = millis();
      isPartyActive = true;

      MP3player.playMP3("track001.mp3", 0);
      digitalWrite(LIGHT_PIN, HIGH);
    }
  }
  else if (millis() - partyStaredAtMs > PARTY_DURATION_MS) {
    isPartyActive = false;

    MP3player.stopTrack();
    digitalWrite(LIGHT_PIN, LOW);
  } 
}
