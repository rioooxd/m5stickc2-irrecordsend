
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtext.h>
#include <EEPROM.h>
#include <M5StickCPlus2.h>
#include <Arduino.h>

const size_t kEepromSize = 8;
// Address to store the IR code, const as we only need one value
const int kIrCodeAddr = 0;
// Default delay to use in most cases
const uint32_t kDelayMs = 100;
// IR Receiver Pin
int ir_recv_pin = 26;  
int ir_send_pin = 19;

IRrecv irrecv(ir_recv_pin);
IRsend irsend(ir_send_pin);
decode_results results;

uint64_t ir_code;
uint64_t temp_code;

void program()
{
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.setTextColor(TFT_GREEN);
  M5.Lcd.setTextSize(3);
  M5.Lcd.println("Send IR\nto save");

  while(true)
  {
    if (irrecv.decode(&results) && !results.repeat)
    {
      temp_code = results.value;
      M5.Lcd.fillScreen(TFT_BLACK);
      M5.Lcd.setCursor(0, 0, 3);
      M5.Lcd.setTextColor(TFT_GREEN);
      M5.Lcd.setTextSize(2);
      M5.Lcd.println("Code received\nClick M5 button\nto save");
      delay(kDelayMs);
      M5.update();
      if(M5.BtnA.wasReleased())
      {
        ir_code = temp_code;
        EEPROM.writeULong64(kIrCodeAddr, ir_code);
        EEPROM.commit();
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0, 0, 3);
        M5.Lcd.setTextColor(TFT_GREEN);
        M5.Lcd.setTextSize(3);
        M5.Lcd.println("Code saved\nto memory");
        delay(kDelayMs * 10);
        break;
      }
      else if(M5.BtnB.wasReleased())
      {
        break;
      }
    }
  }
  irrecv.resume();
  delay(kDelayMs);
  menu();
}
void menu()
{
  M5.update();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.setTextColor(TFT_GREEN);
  M5.Lcd.setTextSize(1);
  M5.Lcd.println("IR Copy and Send\n");
  M5.Lcd.println("Press M5 button\nto send\n");
  M5.Lcd.println("Press B button\nto receive");
}
void setup() 
{
  
    M5.begin();
    EEPROM.begin(kEepromSize);
    irrecv.enableIRIn(); 
    irsend.begin();
    ir_code = EEPROM.readULong64(kIrCodeAddr);
    M5.Lcd.setRotation(3);
    menu();
}

void loop() 
{
  M5.update();
  if(M5.BtnA.wasReleased())
  {
    irsend.sendNEC(ir_code);
  }
  else if(M5.BtnB.wasReleased())
  {
    program();
    delay(kDelayMs);
  }
  delay(kDelayMs);

}