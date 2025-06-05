
#include <BleKeyboard.h>
#include "esp_pm.h"
#include "esp_sleep.h"

BleKeyboard bleKeyboard("ESP32C3 Bluey","Joevial Devices",100);

#define bt1 4
#define bt2 0
#define bt3 6
#define bt4 1
#define bt5 3
#define vBatpin 2

#define every(interval) \
    static uint32_t __every__##interval = millis(); \
    if (millis() - __every__##interval >= interval && (__every__##interval = millis()))

bool playing = false;
 unsigned long previousBlink = 0;
 int batPct = 100;
float vBat;
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  pinMode(vBatpin, INPUT);
  pinMode(bt1,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(bt2,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(bt3,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(bt4,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(bt5,INPUT_PULLUP);    //GPIO0(BOOT)をプルアップ付き入力設定
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  esp_pm_config_esp32c3_t pm_config = {
    .max_freq_mhz = 160,
    .min_freq_mhz = 10,
    .light_sleep_enable = true
  };
  esp_pm_configure(&pm_config);
   vBat = analogReadMilliVolts(vBatpin) / 500.0; // Read the battery voltage from GPIO2
   vBat = analogReadMilliVolts(vBatpin) / 500.0; // Read the battery voltage from GPIO2
  batPct = constrain(mapf(vBat, 3.3, 4.2, 0, 100), 0, 100); // Map and constrain battery percentage
  Serial.println("Starting loop work!");
}

double mapf(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



void loop() {
  every(1000){

     Serial.print("vBat: ");
    Serial.print(vBat, 3);
    Serial.print(" V, Battery Percentage: ");
    Serial.print(batPct);
    Serial.println("%");
  }
  if(bleKeyboard.isConnected()) {
    unsigned long blinkInterval = (batPct * 100); // Convert percentage to milliseconds (e.g. 100% = 10000ms)
    if(millis() - previousBlink >= blinkInterval) {
       vBat = analogReadMilliVolts(vBatpin) / 500.0; // Read the battery voltage from GPIO2
       vBat = analogReadMilliVolts(vBatpin) / 500.0; // Read the battery voltage from GPIO2
      batPct = constrain(mapf(vBat, 3.3, 4.2, 0, 100), 0, 100); // Map and constrain battery percentage
      digitalWrite(8, LOW);   // Turn LED on
      delay(10);            // Keep on for 100ms
      digitalWrite(8, HIGH);  // Turn LED off
      previousBlink = millis();
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
  else {
    if(millis() - previousBlink >= 250) {
      digitalWrite(8, LOW);   // Turn LED on
      delay(10);            // Keep on for 100ms
      digitalWrite(8, HIGH);  // Turn LED off
      previousBlink = millis();
    }
  }

}
