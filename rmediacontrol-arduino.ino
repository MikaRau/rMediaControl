
#include <ClickButton.h>
#include <HID-Project.h>
#include <Encoder.h>


int accel = 1;
int mode = 0;

int buttonPin = 11;
ClickButton button(buttonPin, LOW, CLICKBTN_PULLUP);

unsigned long previousMillis = 0;
const long interval = 700;
int blueLed = 10;
int redLed = 9;

Encoder myEnc(3, 2);

long oldPosition  = -999;

int maxVal = 50;
int fadeTime = 2; //ms

void setup() {
  button.debounceTime = 60;
  button.longClickTime  = 700;
  
  pinMode(blueLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  analogWrite(redLed, maxVal);
  Consumer.begin();  
}



void loop() {
  long newPosition = myEnc.read() / accel;
  if (newPosition != oldPosition) {
    
    if(newPosition > oldPosition) {
      if(mode==0) (Consumer.write(MEDIA_VOLUME_UP));
      else if(mode==1) (Consumer.write(MEDIA_NEXT));
    }
    
    if(oldPosition > newPosition) {
     if(mode==0) (Consumer.write(MEDIA_VOLUME_DOWN));
     else if(mode==1) (Consumer.write(MEDIA_PREVIOUS));
    }
    
    oldPosition = newPosition;
    
  }
  
  button.Update();

  if(button.clicks == 1) {
    if(mode==0) Consumer.write(MEDIA_VOLUME_MUTE);
    else if(mode==1) (Consumer.write(MEDIA_PLAY_PAUSE));
  }
  if(button.clicks == -1) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if(mode == 0) {
        for(int fadeVal = maxVal; fadeVal >= 0; fadeVal -= 1) {
          delay(fadeTime);
          analogWrite(redLed, fadeVal);
          }
        mode = 1;
        for(int fadeVal = 0; fadeVal <= maxVal; fadeVal += 1) {
          delay(fadeTime);
          analogWrite(blueLed, fadeVal);
          }
      }
      else {
        for(int fadeVal = maxVal; fadeVal >= 0; fadeVal -= 1) {
          delay(fadeTime);
          analogWrite(blueLed, fadeVal);
          }
        mode = 0;
        for(int fadeVal = 0; fadeVal <= maxVal; fadeVal += 1) {
          delay(fadeTime);
          analogWrite(redLed, fadeVal);
          }
      }
    }
  }
}


