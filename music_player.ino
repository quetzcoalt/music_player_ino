#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(2, 3);
DFRobotDFPlayerMini myDFPlayer;

int isPaused = 0;
int tracksNumber;
int volume = 15;
int currentTrack = 1;
bool isPlaying = false;

int backwardPin = 4;
int pausePin = 5;
int forwardPin = 6;
int modePin = 7;

int rotaryBPin = 8;
int rotaryAPin = 9;

bool lastPauseState = HIGH;
bool lastBackwardState = HIGH;
bool lastForwardState = HIGH;
bool lastModeState = HIGH;

int BState;
int lastBState;
int AState;
int lastAState;

int normalPin = 10;
int shufflePin = 11;
int randomPin = 12;

int modeValue = 0;

void setup() {
  // put your setup code here, to run once:
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);

  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Initializing DFPlayer module ... Wait!"));

  if (!myDFPlayer.begin(mySoftwareSerial))
  {
    Serial.println(F("Not initialized."));
    while (true);
  }
  
  Serial.println(F("DFPlayer Mini module initialized!"));

  pinMode(backwardPin, INPUT_PULLUP);
  pinMode(pausePin, INPUT_PULLUP);
  pinMode(forwardPin, INPUT_PULLUP);
  pinMode(modePin, INPUT_PULLUP);

  pinMode(rotaryBPin, INPUT);
  pinMode(rotaryAPin, INPUT);

  pinMode(normalPin, OUTPUT);
  pinMode(shufflePin, OUTPUT);
  pinMode(randomPin, OUTPUT);

  digitalWrite(normalPin, HIGH); 
  digitalWrite(shufflePin, LOW); 
  digitalWrite(randomPin, LOW); 

  tracksNumber = myDFPlayer.readFileCounts();
  Serial.println("There are ");
  Serial.print(tracksNumber);
  Serial.print("tracks.");
  Serial.println();

  myDFPlayer.volume(volume);
  myDFPlayer.play(currentTrack);
  isPlaying = true;

  Serial.println(myDFPlayer.readFileCounts());
}

void loop() {
  /* ---------- PAUSE ---------- */
  int pauseState = digitalRead(pausePin);

  if (pauseState == LOW && lastPauseState == HIGH)
  {
    if (isPlaying)
    {
      myDFPlayer.pause();
      isPlaying = false;
      Serial.println("Playback Paused");
    } else
    {
      myDFPlayer.start();
      isPlaying = true;
      Serial.println("Playback Resumed");
    }

    delay(100);
  }
  lastPauseState = pauseState;

  /* ---------- BACKWARDS ---------- */
  int backwardState = digitalRead(backwardPin);

  if (backwardState == LOW && lastBackwardState == HIGH)
  {
      myDFPlayer.previous();
      isPlaying = true;

      delay(100);
  }
  lastBackwardState = backwardState;

  /* ---------- FORWARDS ---------- */
  int forwardState = digitalRead(forwardPin);

  if (forwardState == LOW && lastForwardState == HIGH)
  {
      myDFPlayer.next(); 
      isPlaying = true;

      delay(100);
  }
  lastForwardState = forwardState;

  /* ---------- MODE ---------- */
  int modeState = digitalRead(modePin);

  if (modeState == LOW && lastModeState == HIGH)
  {
      modeValue = (modeValue + 1) % 3;

      myDFPlayer.disableLoopAll();
      myDFPlayer.disableLoop();

      switch (modeValue) {
        case 0:
          myDFPlayer.enableLoopAll();

          digitalWrite(normalPin, HIGH);
          digitalWrite(shufflePin, LOW); 
          digitalWrite(randomPin, LOW);
          break;
        case 1:
          myDFPlayer.randomAll();

          digitalWrite(normalPin, LOW);
          digitalWrite(shufflePin, HIGH); 
          digitalWrite(randomPin, LOW); 
          break;
        case 2:
          myDFPlayer.enableLoop();

          digitalWrite(normalPin, LOW);
          digitalWrite(shufflePin, LOW); 
          digitalWrite(randomPin, HIGH); 
          break;
        default:
          myDFPlayer.disableLoopAll();
          myDFPlayer.disableLoop();
          break;
      }
      delay(100);
  }
  lastModeState = modeState;
  
  /* ---------- VOLUME ---------- */
  AState = digitalRead(rotaryAPin);

  if (AState != lastAState)
  {
    if (digitalRead(rotaryBPin) != AState)
    {
      if (volume > 0)
      {
        volume -= 1;
        myDFPlayer.volume(volume);
      }
    } else {
      if (volume < 30)
      {
        volume += 1;
        myDFPlayer.volume(volume);
      }
    } 

    delay(100);
  }

  lastAState = AState;
  
}
