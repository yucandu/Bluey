
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("ESP32C3 Media Keybord","Mendako Lab",50);

#define bt1 0
#define bt2 1
#define bt3 3
#define bt4 4
#define bt5 5

#define every(interval) \
    static uint32_t __every__##interval = millis(); \
    if (millis() - __every__##interval >= interval && (__every__##interval = millis()))

bool playing = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();

  pinMode(bt1,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(bt2,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(bt3,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(bt4,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(bt5,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  Serial.println("Starting loop work!");
}

void loop() {

  if(bleKeyboard.isConnected()) {
    every(2000) {
      digitalWrite(8, LOW);  // Turn LED on
      delay(10);           // Wait 100ms
      digitalWrite(8, HIGH);
    }
    if(digitalRead(bt1) == LOW){
      digitalWrite(8, LOW);
      Serial.println("Sending Scan Next Track media key...");
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
      while (digitalRead(bt1) == LOW){delay(1);}
      digitalWrite(8, HIGH);
    }
    if(digitalRead(bt2) == LOW){
      digitalWrite(8, LOW);
      Serial.println("Sending Scan Previous Track media key...");
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
      while (digitalRead(bt2) == LOW){delay(1);}
      digitalWrite(8, HIGH);
    }


    if(digitalRead(bt3) == LOW){
      digitalWrite(8, LOW);
      //Serial.println("Sending Play media key...");
      if (playing) {
        Serial.println("Sending Pause media key...");
        bleKeyboard.write(KEY_MEDIA_STOP);
        playing = false;
      } else {
        Serial.println("Sending Play media key...");
        bleKeyboard.write(KEY_MEDIA_PLAY);
        playing = true;
      }
      while (digitalRead(bt3) == LOW){delay(1);}
      digitalWrite(8, HIGH);
    }


 if(digitalRead(bt4) == LOW){
      digitalWrite(8, LOW);
      unsigned long pressTime = millis();
      bool longPress = false;
      
      // First press
      Serial.println("Sending Volume Increment media key...");
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
      
      // Check for hold
      while (digitalRead(bt4) == LOW){
       /* if (millis() - pressTime > 1000 && !longPress) {
          longPress = true;  // Entered long press mode
        }
        if (longPress) {
          bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
          delay(50);  // Repeat every 50ms
        } else {*/
          delay(1);
       // }
      }
      digitalWrite(8, HIGH);
    }

    if(digitalRead(bt5) == LOW){
      digitalWrite(8, LOW);
      unsigned long pressTime = millis();
      bool longPress = false;
      
      // First press
      Serial.println("Sending Volume Decrement media key...");
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
      
      // Check for hold
      while (digitalRead(bt5) == LOW){
        if (millis() - pressTime > 1000 && !longPress) {
          longPress = true;  // Entered long press mode
        }
        if (longPress) {
          bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
          delay(50);  // Repeat every 50ms
        } else {
          delay(1);
        }
      }
      digitalWrite(8, HIGH);
    }
  }
  //every(2000){
  //  Serial.print("Ping...");
 // }
}