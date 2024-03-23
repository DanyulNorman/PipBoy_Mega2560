#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include <Rotary.h>
#include "pipboy_text.h"
#define DARK_GREEN 0x0280//0x0460
#define MED_GREEN 0x0460//
#define RA8875_INT 3
#define RA8875_CS 10
#define RA8875_RESET 9


const int8_t lightPin_GIGER = 31;
const int8_t lightPin_RADIO = 33;
const int8_t pin_GIGER1 = 32;
const int8_t pin_GIGER2 = 34;
const int8_t pin_GIGER3 = 36;
Rotary rotary = Rotary(35, 37); 
const int8_t buttonPin_DIAL = 39;
const int8_t buttonPin_STAT = 45;
const int8_t buttonPin_ITEM = 47;
const int8_t buttonPin_DATA = 49; 
//const int8_t buttonPin_RADIO = 51; 
int8_t buttonState_DIAL = 0;
int8_t buttonState_STAT = 0;
int8_t buttonState_ITEM = 0;
int8_t buttonState_DATA = 0;
//int8_t buttonState_RADIO = 0;
int8_t lightState_GIGER = 0;
int8_t _currentTab = 0;
int8_t _currentScroll=0;
int8_t _currentSection=0;
int8_t currentStateCLK;
int8_t lastStateCLK;
const char currentDir =" ";
unsigned long lastButtonPress = 0;
bool playGiger;
bool changingSECTION;
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

int linesPrinted = 0;
int8_t maxSelection = 2;
// Define debounce delay (adjust as needed)
#define DEBOUNCE_DELAY 50 // in milliseconds

#define USE_ADAFRUIT_GFX_FONTS
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>


// Define variables to keep track of button state and debounce time
unsigned long lastDebounceTime = 0;
bool buttonState_DIAL_last = HIGH; // assuming HIGH when not pressed
uint8_t textY = 33;

void set_TAB_text(){
  
  // Define text strings
  String texts[] = {"STAT", "ITEM", "DATA", "RADIO"};
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(RA8875_GREEN);

  tft.setCursor(200, textY);
  tft.print(texts[0].c_str());

  tft.setCursor(300, textY);
  tft.print(texts[1].c_str());

  tft.setCursor(402, textY);
  tft.print(texts[2].c_str());

  tft.setCursor(502, textY);
  tft.print(texts[3].c_str());

}
void set_TAB_outline(uint8_t index){
  uint8_t lineY = 44;
  
  clearLines(lineY);
  clearSPECIAL();
  bool bottomBoxesDefault;
  tft.drawLine(63, lineY, 675, lineY, RA8875_GREEN);
  tft.drawLine(63, lineY, 63, lineY+8, RA8875_GREEN); 
  tft.drawLine(720, lineY, 720, lineY+8, RA8875_GREEN);
  

   switch(index){
      case 0:
      tft.drawLine(185, lineY, 274, lineY, RA8875_BLACK);
      tft.drawLine(185, lineY, 185, lineY-20, RA8875_GREEN); //side left
      tft.drawLine(274, lineY, 274, lineY-20, RA8875_GREEN);//side right
      tft.drawLine(185, 24, 191, 24, RA8875_GREEN); 
      tft.drawLine(268, 24, 274, 24, RA8875_GREEN);
      bottomBoxesDefault = false;
      break;
      case 1:
      tft.drawLine(284, lineY, 372, lineY, RA8875_BLACK);
      tft.drawLine(284, lineY, 284, lineY-20, RA8875_GREEN); //side left
      tft.drawLine(372, lineY, 372, lineY-20, RA8875_GREEN);//side right
      tft.drawLine(284, 24, 290, 24, RA8875_GREEN); 
      tft.drawLine(366, 24, 372, 24, RA8875_GREEN);
      bottomBoxesDefault = true;

      break;
      case 2:
      tft.drawLine(386, lineY, 483, lineY, RA8875_BLACK);
      tft.drawLine(386, lineY, 386, lineY-20, RA8875_GREEN); //side left
      tft.drawLine(483, lineY, 483, lineY-20, RA8875_GREEN);//side right
      tft.drawLine(386, 24, 392, 24, RA8875_GREEN); 
      tft.drawLine(477, 24, 483, 24, RA8875_GREEN);
      bottomBoxesDefault = true;
      break;
      case 3:
      tft.drawLine(189, lineY, 274, lineY, RA8875_BLACK);
      tft.drawLine(189, lineY, 189, lineY-20, RA8875_GREEN); //side left
      tft.drawLine(274, lineY, 274, lineY-20, RA8875_GREEN);//side right
      tft.drawLine(189, 24, 195, 24, RA8875_GREEN); 
      tft.drawLine(268, 24, 274, 24, RA8875_GREEN);
      bottomBoxesDefault = true;
      break;
    }
    //BOTTOM BOXES
    //STAT - SHORT LONG SHORT - HP / LVL / AP  
    //ITEM - SHORT SHORT LONG - WGT / CAPS/ EMPTY
    //DATA - SHORT SHORT LONG - DATE / TIME / EMPTY
    //RADIO - SHORT SHORT LONG - DATE / TIME / EMPTY
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(RA8875_GREEN);
    if(bottomBoxesDefault){
      tft.fillRect(0,433,800,40,RA8875_BLACK);
      tft.fillRect(65,433,134,32,DARK_GREEN);
      tft.fillRect(202,433,134,32,DARK_GREEN);
      tft.fillRect(324,433,381,32,DARK_GREEN);
    }
    else{
      tft.fillRect(0,433,800,40,RA8875_BLACK);
      tft.fillRect(65,433,134,32,DARK_GREEN);
      tft.fillRect(202,433,381,32,DARK_GREEN);
      tft.fillRect(586,433,134,32,DARK_GREEN);
    }

    switch(index){
      case 0:
        tft.setCursor(68, 456);
        tft.print("HP 250/250");
        tft.setCursor(210, 456);
        tft.print("LVL 44 ");
        tft.drawRect(295,441,270,15,RA8875_GREEN);
        tft.fillRect(295,441,60,15,RA8875_GREEN);
        tft.setCursor(616, 456);
        tft.print("AP 42/60");
      break;
      case 1:
        
        tft.drawBitmap(70,437,(const unsigned char*)wgt,24,24,RA8875_GREEN);
        tft.setCursor(102, 456);
        tft.print("32/148");
        
        tft.drawBitmap(210,437,(const unsigned char*)caps,24,24,RA8875_GREEN);
        tft.setCursor(240, 456);
        tft.print("35000");
        
      break;
      default:
        tft.setCursor(70, 456);
        tft.print("03.11.2117");
        
        tft.setCursor(210, 456);
        tft.print("5:36PM");
      break;
    }
}
void clearLines(uint8_t lineY){
      tft.drawLine(185, lineY, 274, lineY, RA8875_GREEN);
      tft.drawLine(185, lineY, 185, lineY-20, RA8875_BLACK); //side left
      tft.drawLine(274, lineY, 274, lineY-20, RA8875_BLACK);//side right
      tft.drawLine(185, 24, 195, 24, RA8875_BLACK); 
      tft.drawLine(268, 24, 274, 24, RA8875_BLACK);

      tft.drawLine(284, lineY, 372, lineY, RA8875_GREEN);
      tft.drawLine(284, lineY, 284, lineY-20, RA8875_BLACK); //side left
      tft.drawLine(372, lineY, 372, lineY-20, RA8875_BLACK);//side right
      tft.drawLine(284, 24, 290, 24, RA8875_BLACK); 
      tft.drawLine(366, 24, 372, 24, RA8875_BLACK);

      tft.drawLine(386, lineY, 483, lineY, RA8875_GREEN);
      tft.drawLine(386, lineY, 386, lineY-20, RA8875_BLACK); //side left
      tft.drawLine(483, lineY, 483, lineY-20, RA8875_BLACK);//side right
      tft.drawLine(386, 24, 392, 24, RA8875_BLACK); 
      tft.drawLine(477, 24, 483, 24, RA8875_BLACK);

      tft.drawLine(625, lineY, 720, lineY, RA8875_GREEN);
      tft.drawLine(625, lineY, 625, lineY-20, RA8875_BLACK); //side left
      tft.drawLine(720, lineY, 720, lineY-20, RA8875_BLACK);//side right
      tft.drawLine(625, 30, 631, 30, RA8875_BLACK); 
      tft.drawLine(714, 30, 720, 30, RA8875_BLACK);
}
void set_SECTION(uint8_t tab, uint8_t section){
      
      tft.setFont(&FreeSans12pt7b);
      tft.setCursor(170, 80);
      tft.setTextColor(RA8875_GREEN);  
      clearALLDyamic();
  switch(tab){
    case 0:     //STAT
      for (int i = 0; i < 4; i++) {
            // Read the string from PROGMEM
            const char* sec_stat_text = (const char*)pgm_read_word(&(stat_SEC_STAT[i]));
            // Set the text color based on condition
            if (i == section) {
                tft.setTextColor(RA8875_GREEN);
            } else {
                // Set default text color
                tft.setTextColor(DARK_GREEN, RA8875_BLACK); // You need to define DEFAULT_TEXT_COLOR
            }
            // Write the text
            tft.print(sec_stat_text);
            // If this is not the last iteration, print a space
            if (i > 3) {
                tft.print("   ");
            }
        }
        switch(section){
          case 0://STATUS
          set_STATUS(0);
          break;
          case 1://EFFECTS
          set_EFFECTS();
          break;
          case 2://SPECIAL
          set_SPECIAL(0);
          break;
        }
    break;
    case 1:
    //set_WEAP
        for (int i = 0; i < 4; i++) {
            // Read the string from PROGMEM
            const char* sec_stat_text = (const char*)pgm_read_word(&(stat_SEC_ITEM[i]));
            // Set the text color based on condition
            if (i == section) {
                tft.setTextColor(RA8875_GREEN);
            } else {
                // Set default text color
                tft.setTextColor(DARK_GREEN, RA8875_BLACK); // You need to define DEFAULT_TEXT_COLOR
            }
            // Write the text
            tft.print(sec_stat_text);
            // If this is not the last iteration, print a space
            
        }
        switch(section){
          case 0://APPAREL
          break;
          case 1://WEAP
            set_WEAP(0);
          break;
          case 2://AID
          break;
          case 3://MISC
          break;
        }
    break;  
    case 2:
      for (int i = 0; i < 4; i++) {
            // Read the string from PROGMEM
            const char* sec_stat_text = (const char*)pgm_read_word(&(stat_SEC_DATA[i]));
            // Set the text color based on condition
            if (i == section) {
                tft.setTextColor(RA8875_GREEN);
            } else {
                // Set default text color
                tft.setTextColor(DARK_GREEN, RA8875_BLACK); // You need to define DEFAULT_TEXT_COLOR
            }
            // Write the text
            tft.print(sec_stat_text);
            // If this is not the last iteration, print a space
            if (i > 3) {
                tft.print("   ");
            }
        }
      break;
  }
  changingSECTION = false;
      
}
void clear_SECTION(){
    tft.fillRect(50,50,600,40,RA8875_BLACK);
}
void set_ITEM(uint8_t index){
  clearSPECIAL();
  //showVaulBoy(false);
}
void set_DATA(uint8_t index){
  //showVaulBoy(false);
  clearSPECIAL();
}
void showVaulBoy(bool show){
 
  if(show){
    //status meters
    tft.fillRect(384,99,32,9,RA8875_GREEN);//top
    tft.fillRect(287,177,32,9,RA8875_GREEN);//ltop
    tft.fillRect(287,278,32,9,RA8875_GREEN);//lbot
    tft.fillRect(478,177,32,9,DARK_GREEN);//rtop
    tft.fillRect(478,177,25,9,RA8875_GREEN);//rtop
    tft.fillRect(478,278,32,9,RA8875_GREEN);//rbot
    tft.fillRect(380,314,32,9,DARK_GREEN);//bot
    tft.fillRect(380,314,20,9,RA8875_GREEN);//bot

    //status stat blocks
    tft.fillRect(279,340,48,48,DARK_GREEN);//bot
    tft.fillRect(330,340,30,48,DARK_GREEN);//bot
    tft.fillRect(377,340,48,48,DARK_GREEN);//bot
    tft.fillRect(428,340,30,48,DARK_GREEN);//bot
    tft.fillRect(461,340,30,48,DARK_GREEN);//bot
    tft.fillRect(494,340,30,48,DARK_GREEN);//bot

    tft.setTextColor(RA8875_GREEN);

    //status stat imgs
    tft.drawBitmap(281,348,(const unsigned char*)gun,42,30,RA8875_GREEN);
    tft.drawBitmap(334,342,(const unsigned char*)aim,24,24,RA8875_GREEN);
    tft.setCursor(334, 382);
    tft.print("18");
    tft.drawBitmap(377,348,(const unsigned char*)helmet,45,36,RA8875_GREEN);
    tft.drawBitmap(434,342,(const unsigned char*)shield,19,24,RA8875_GREEN);
    tft.setCursor(433, 382);
    tft.print("10");
    tft.drawBitmap(467,342,(const unsigned char*)voltage,14,24,RA8875_GREEN);
    tft.setCursor(465, 382);
    tft.print("15");
    tft.drawBitmap(498,342,(const unsigned char*)radiation,24,22,RA8875_GREEN);
    tft.setCursor(497, 382);
    tft.print("10");
    


    tft.drawBitmap(352,124,(const unsigned char*)vaultboy_status,90,180,MED_GREEN);

  }
  

}
///SET THE TEXT AND GRAPHICS FOR THE STAT / STATUS SECTION
void set_STATUS(uint8_t index){
  
  clearSPECIAL();
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0.3);
  tft.fillRect(65,397,120,32,MED_GREEN);
  tft.fillRect(198,397,125,32,MED_GREEN);
  tft.setTextColor(DARK_GREEN);
  tft.setCursor(70, 420);
  tft.print("STIMPAK (1)");
  tft.setCursor(200, 420);
  tft.print("RADAWAY (0)");

  uint16_t lineY = 114;
  tft.fillRect(100,lineY,50,50,DARK_GREEN);  
  tft.drawBitmap(100+12,lineY+10,(const unsigned char*)radiation,24,24,RA8875_GREEN);
  tft.drawBitmap(100+30,lineY+30,(const unsigned char*)clock_sm,15,15,RA8875_GREEN);//
  
  lineY+=55;
  tft.fillRect(100,lineY,50,50,DARK_GREEN);  
  tft.drawBitmap(100+12,lineY+10,(const unsigned char*)food,24,24,RA8875_GREEN);
  tft.drawBitmap(100+30,lineY+30,(const unsigned char*)clock_sm,15,15,RA8875_GREEN);//
  lineY+=55;

  tft.fillRect(100,lineY,50,50,DARK_GREEN);  
  tft.drawBitmap(100+12,lineY+10,(const unsigned char*)pill,24,24,RA8875_GREEN);//clock_sm
  tft.drawBitmap(100+30,lineY+30,(const unsigned char*)clock_sm,15,15,RA8875_GREEN);//
  
  showVaulBoy(true);

    

  

}
int delayTime;
void set_EFFECTS(){
  uint16_t lineY = 120;
  uint16_t lineY2 = 440;
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(RA8875_GREEN);
  tft.fillRect(60,lineY,50,50,DARK_GREEN);  
  tft.fillRect(115,lineY,600,50,DARK_GREEN);
  tft.setCursor(125, lineY+30);
  tft.print("Radiation:");
  tft.setCursor(350, lineY+30);
  tft.print("STR -3  END -2");
  tft.drawBitmap(60+12,lineY+10,(const unsigned char*)radiation,24,22,RA8875_GREEN);
  lineY+=55;
  tft.fillRect(60,lineY,50,50,DARK_GREEN);
  tft.fillRect(115,lineY,600,50,DARK_GREEN);
  tft.setCursor(125, lineY+30);
  tft.print("Hungry:");
  tft.setCursor(350, lineY+30);
  tft.print("CHAR -1  END -2");
  tft.drawBitmap(60+12,lineY+10,(const unsigned char*)food,24,24,RA8875_GREEN);
  lineY+=55;
  tft.fillRect(60,lineY,50,50,DARK_GREEN);
  tft.fillRect(115,lineY,600,50,DARK_GREEN);
  tft.setCursor(125, lineY+30);
  tft.print("Addicted:");
  tft.setCursor(350, lineY+30);
  tft.print("INT -5  LCK -2  END -2");
  tft.drawBitmap(60+12,lineY+10,(const unsigned char*)pill,24,24,RA8875_GREEN);
  lineY+=55;
  
  
  tft.setFont(&FreeSans24pt7b);
  // tft.setCursor(275, textY+20);
  // tft.print("WELCOME");
  // tft.setFont(&FreeSans12pt7b);
  // tft.setCursor(150, 195);
  // tft.print("VAULT  -  TEC");
  // tft.setFont(&FreeSans9pt7b);
  // tft.setCursor(150, 230);
  // tft.print("     INDUSTRIES");
  // tft.drawBitmap(140,250,(const unsigned char*)vault_tec_logo,180,73,RA8875_GREEN);
  //playGigerTones();
  playGiger = true;
  delayTime = random(50, 100);

}
uint8_t prevIndex;
void set_SPECIAL(uint8_t index){
  //showVaulBoy(false);
  
  uint16_t statX = 90;
  uint16_t valX = 340;
  uint16_t statY = 120;
  uint8_t lineHeight = 35;
  const int screenHeight = 480;
  //clearSPECIAL();
  // Your existing code
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextColor(RA8875_GREEN);
  // Loop through the stat_SPEC and stat_SPEC_val arrays
      for (int i = 0; i < 7; i++) {
          if(i==index){
            tft.fillRect(statX-10, statY-22,295,30,RA8875_GREEN);  
            tft.setTextColor(RA8875_BLACK);
                     
          }
          else if(i==prevIndex){
            tft.fillRect(statX-10, statY-22,295,30,RA8875_BLACK);  
            tft.setTextColor(RA8875_GREEN);
          }
             
          else{
            tft.setTextColor(RA8875_GREEN);
          }
          tft.setCursor(statX, statY);
          tft.print((char*)pgm_read_word(&(stat_SPEC[i])));
          tft.setCursor(valX, statY);
          tft.print((char*)pgm_read_word(&(stat_SPEC_val[i])));
            
          // Increase statY by lineHeight
          statY += lineHeight;
          
      }
      uint16_t tX = 480;
      uint16_t tY = 95;
      //set the image and desc
      tft.fillRect(410, 95,350,150,RA8875_BLACK);
      tft.setTextColor(RA8875_GREEN);
      tft.fillRect(380,260,380,170,RA8875_BLACK);
      tft.setCursor(400, 285);
      switch(index){
        case 0:
        tft.drawBitmap(tX+39,tY,(const unsigned char*)strength,83,128,RA8875_GREEN);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[0])));
        tft.setCursor(400, 315);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[1])));
        tft.setCursor(400, 345);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[2])));  
        tft.setCursor(400, 375);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[3])));    
        break;
        case 1:
        tft.drawBitmap(tX,tY,(const unsigned char*)perception,158,183,RA8875_GREEN);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[4])));
        tft.setCursor(400, 315);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[5])));
        tft.setCursor(400, 345);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[6])));
        tft.setCursor(400, 375);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[7])));  
        break;
        case 2:
        tft.drawBitmap(tX+20,tY,(const unsigned char*)endurance,111,125,RA8875_GREEN);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[8])));
        tft.setCursor(400, 315);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[9])));
        tft.setCursor(400, 345);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[10])));
        tft.setCursor(400, 375);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[11])));  
        break;
        case 3:
        tft.drawBitmap(tX+67,tY,(const unsigned char*)charisma,55,119,RA8875_GREEN);        
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[12])));
        tft.setCursor(400, 315);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[13])));
        tft.setCursor(400, 345);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[14])));
        tft.setCursor(400, 375);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[15])));  
        break;
        case 4:
        tft.drawBitmap(tX+30,tY,(const unsigned char*)intelligence,102,126,RA8875_GREEN);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[16])));
        tft.setCursor(400, 315);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[17])));
        tft.setCursor(400, 345);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[18])));
        tft.setCursor(400, 375);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[19])));
        break;
        case 5:
        tft.drawBitmap(tX+20,tY,(const unsigned char*)agility,118,112,RA8875_GREEN);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[20])));
        tft.setCursor(400, 315);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[21])));
        tft.setCursor(400, 345);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[22])));
        tft.setCursor(400, 375);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[23])));
        break;
        case 6:
        tft.drawBitmap(tX+30,tY,(const unsigned char*)luck,102,121,RA8875_GREEN);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[24])));
        tft.setCursor(400, 315);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[25])));
        tft.setCursor(400, 345);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[26])));
        tft.setCursor(400, 375);
        tft.print((char*)pgm_read_word(&(stat_SPEC_desc[25])));
        break;
      }
      
      
      
      
      prevIndex = index;
}

void set_WEAP(uint8_t index){
  uint16_t statX = 90;
  uint16_t valX = 316;
  uint16_t statY = 120;
  uint16_t statY2 = 275;
  uint8_t lineHeight = 35;
  const int screenHeight = 480;
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextColor(RA8875_GREEN);
  tft.fillRect(520,255,200,30,DARK_GREEN);
  tft.fillRect(520,290,200,30,DARK_GREEN);
  tft.fillRect(520,325,200,30,DARK_GREEN);
  tft.fillRect(520,360,200,30,DARK_GREEN);
  for (int i = 0; i < 5; i++) {
    if (i == index) {
        tft.fillRect(statX - 10, statY - 22, 350, 30, RA8875_GREEN);        
        tft.setTextColor(RA8875_BLACK);

    } 
    else if (i == prevIndex) {
        tft.fillRect(statX - 10, statY - 22, 350, 30, RA8875_BLACK);
        tft.setTextColor(RA8875_GREEN);
    } 
    else {
        tft.setTextColor(RA8875_GREEN);
    }

    tft.setCursor(statX, statY);
    tft.print((char*)pgm_read_word(&(WEAP[i])));
    
    // include the image, just as in the example.
    // Write the image, taking the width, height and data from the structure in the purple.c file

        // Increase statY by lineHeight
        statY += lineHeight; // Increment statY by 3 times the lineHeight to leave space between weapon entries
    }

          prevIndex = index;
          setWeaponStat(index);
}
void setWeaponStat(uint8_t index){
  uint16_t statX = 90;
  uint16_t valX = 316;
  uint16_t statY = 120;
  uint16_t statY2 = 275;
  uint8_t lineHeight = 35;
  tft.setTextColor(RA8875_GREEN);
  for (int category = 0; category < 4; category++) {
      const char* const* categoryPtr = (const char* const*)pgm_read_ptr(&(WEAP_STATS[category]));
      tft.setCursor(525, statY2);
      tft.print((char*)pgm_read_word(&(categoryPtr[index])));
      statY2 += lineHeight; // Move to the next line
  }
    tft.drawBitmap(552,130,(const unsigned char*)pgm_read_ptr(&(WEAP_IMG[index])),94,49,RA8875_GREEN);

}
void clearALLDyamic(){
    tft.fillRect(0,84,800,346,RA8875_BLACK);
}
void clearSPECIAL(){
  
  uint16_t statX = 50;
  uint16_t valX = 275;
  uint16_t statY = 120;
  uint8_t lineHeight = 35;
  const int screenHeight = 480;
  for (int i = 0; i < 7; i++) {
      tft.setTextColor(RA8875_BLACK);            

      tft.setCursor(statX, statY);
      tft.print("                    ");
      tft.setCursor(valX, statY);
      tft.print("   ");
      
        
      // Increase statY by lineHeight
      statY += lineHeight;
    
  }
}

void playGigerTones(int delayTime){
   tone(53, 60, 20);
  int delayTime2 = random(1, 10);
  if(delayTime2 < 5){
    digitalWrite(pin_GIGER1, LOW); 
    digitalWrite(pin_GIGER2, HIGH); 
    digitalWrite(pin_GIGER3, HIGH); 
  }
  if(delayTime2 > 5 || delayTime2 < 8){
    digitalWrite(pin_GIGER1, LOW); 
    digitalWrite(pin_GIGER2, HIGH); 
    digitalWrite(pin_GIGER3, HIGH); 
  }
  else{
    
    digitalWrite(pin_GIGER1, HIGH); 
    digitalWrite(pin_GIGER2, HIGH); 
    digitalWrite(pin_GIGER3, HIGH); 
  }
  //TODO: PULSE NEEDLE
  delay(delayTime * delayTime2);
  digitalWrite(pin_GIGER1, HIGH); 
    digitalWrite(pin_GIGER2, HIGH); 
    digitalWrite(pin_GIGER3, HIGH); 
}
void HandleScroll(int8_t scrollIndex,int8_t tabNumber,int8_t sectionNumber){
    ///what tab are we on?
    if(tabNumber == 0){
      //stat
      switch(sectionNumber){
        case 0: //status NO scrolling
        break;
        case 1://effects
        //no scrolling yet
        break;
        case 2://special
        set_SPECIAL(scrollIndex);
        break;
      }
    }
    else if(tabNumber==1){
      switch(sectionNumber){
      case 0: //apparel
      break;
      case 1://weap
        set_WEAP(scrollIndex);
      break;
      case 2://aid
      break;
      }
    }
    else if(tabNumber==2){

    }
    else if(tabNumber==3){

    }
    
    
}
void startupScreen(){
  uint16_t lineY = 44;
  uint16_t lineY2 = 440;

  tft.drawLine(63, lineY, 720, lineY, RA8875_GREEN);
  tft.drawLine(63, lineY, 63, lineY+8, RA8875_GREEN); 
  tft.drawLine(720, lineY, 720, lineY+8, RA8875_GREEN);
  tft.fillRect(255,10,280,40,RA8875_BLACK);
  tft.drawLine(63, lineY2, 720, lineY2, RA8875_GREEN);
  tft.drawLine(63, lineY2, 63, lineY2-8, RA8875_GREEN); 
  tft.drawLine(720, lineY2, 720, lineY2-8, RA8875_GREEN);
  tft.setFont(&FreeSans24pt7b);
  tft.setTextColor(RA8875_GREEN);
  tft.setCursor(275, textY+20);
  tft.print("WELCOME");
  tft.setFont(&FreeSans12pt7b);
  tft.setCursor(150, 195);
  tft.print("VAULT  -  TEC");
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(150, 230);
  tft.print("     INDUSTRIES");
  tft.drawBitmap(140,250,(const unsigned char*)vault_tec_logo,180,73,RA8875_GREEN);
  tft.drawBitmap(420,100,(const unsigned char*)VaultBoy2,250,265,RA8875_GREEN);

}
void boot(){

   int textHeight = 20; 
    int looper = 1;
    // tft.setTextColor(RA8875_GREEN);
    //     tft.setFont(&FreeMonoBold9pt7b);
    // int startupTextCount = sizeof(STARTUPTEXT) / sizeof(STARTUPTEXT[0]);
    // // Print each line of the text array 
    // for (int i = 0; i < startupTextCount * 4; i++) { // Double the loop count to repeat STARTUPTEXT array
    //     int textIndex = i % startupTextCount; // Get the index within the STARTUPTEXT array
    //     tft.setTextColor(RA8875_GREEN);
    //     tft.setCursor(30, i * textHeight); // Adjust the y-coordinate
    //     tft.print((char*)pgm_read_word(&(STARTUPTEXT[textIndex])));
    //     int delayTime = random(5, 10); // Random delay between 100 and 300 milliseconds
    //     tone(53, 100, 30);
    //     //delay(delayTime);
    // }
    // //set_TAB_outline(0);
    // delay(1500);
  // tft.fillScreen(RA8875_BLACK);
  //   int startupTextCount = sizeof(BOOTTEXT) / sizeof(BOOTTEXT[0]);
  //   // Print each line of the text array 
  //   for (int8_t i = 0; i < startupTextCount; i++) { // Double the loop count to repeat STARTUPTEXT array
  //       int8_t textIndex = i % startupTextCount; // Get the index within the STARTUPTEXT array
        
  //       tft.setCursor(50,textHeight); // Adjust the y-coordinate
  //       tft.setFont(&FreeMonoBold12pt7b);        
  //       tft.print((char*)pgm_read_word(&(BOOTTEXT[textIndex])));
  //       tft.setCursor(50, 100);
  //       int delayTime = random(100, 300); // Random delay between 100 and 300 milliseconds
  //       tone(53, 100, 15);
  //       textHeight = textHeight + 30;
  //       delay(delayTime);
  //   }
  //   delay(1500);
  //   tone(53, 1000, 15);
    tft.fillScreen(RA8875_BLACK);
    int startupTextCount = sizeof(BOOTTEXT) / sizeof(BOOTTEXT[0]);

    for (int8_t i = 0; i < startupTextCount; i++) {
        int8_t textIndex = i % startupTextCount;
        const char *text = (char*)pgm_read_word(&(BOOTTEXT[textIndex])); // Get the text from PROGMEM
        int textLength = strlen(text); // Get the length of the text

        tft.setCursor(50, textHeight);
        tft.setFont(&FreeMonoBold12pt7b);

        // Print characters one by one with a short delay and tone
        for (int j = 0; j < textLength; j++) {
            tft.print(text[j]); // Print one character

            // Play a short tone
            tone(53, 90, 15);
            delay(15); // Short delay between characters
        }

        tft.setCursor(50, 100);
        int delayTime = random(50, 200); // Random delay between 100 and 300 milliseconds
        textHeight = textHeight + 30;
        delay(delayTime);
    }

    delay(1500);
    tone(53, 1000, 100);
    delay(2000);
    tft.fillScreen(RA8875_BLACK);
    startupScreen();
    delay(5000);
}
void setup(){
  
  pinMode(10, OUTPUT);
  pinMode(lightPin_GIGER,OUTPUT);
  pinMode(pin_GIGER1,OUTPUT);
  pinMode(pin_GIGER2,OUTPUT);
  pinMode(pin_GIGER3,OUTPUT);
  pinMode(buttonPin_STAT, INPUT);
  pinMode(buttonPin_ITEM, INPUT);
  pinMode(buttonPin_DATA, INPUT);
  //pinMode(buttonPin_RADIO, INPUT);
  pinMode(buttonPin_DIAL, INPUT_PULLUP);  
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);
  Serial.begin(9600);
  
  /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);

  /* Switch to text mode */
  
  tft.setTextColor(RA8875_GREEN,RA8875_BLACK);
  tft.graphicsMode();

  // boot();


  tft.fillScreen(RA8875_BLACK);
  
  set_TAB_text();
  set_TAB_outline(0);
  set_SECTION(0,0);
  
}
void loop(){
  unsigned char result = rotary.process();

  // read the state of the pushbutton value:
  //(const unsigned char*)vaultboy
 
  buttonState_STAT = digitalRead(buttonPin_STAT);
  buttonState_ITEM = digitalRead(buttonPin_ITEM);
  buttonState_DATA = digitalRead(buttonPin_DATA); 
  //buttonState_RADIO = digitalRead(buttonPin_RADIO); 
  buttonState_DIAL = digitalRead(buttonPin_DIAL);  
  
 
   // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState_STAT == HIGH) {
      if(_currentTab != 0){
        _currentScroll = 0;
        _currentSection = 0;
        tone(53,250,20);
        set_TAB_outline(0);
        clear_SECTION();
        set_SECTION(0,0);
        //clearALLDyamic();
        
        //playGiger=true;
        _currentTab = 0;
      }
      
  }
  if(buttonState_ITEM == HIGH){
    if(_currentTab != 1){
      _currentTab = 1;
      //playGiger=false;
      _currentScroll = 0;
      _currentSection = 0;
      tone(53,250,20);
      set_TAB_outline(1);
      clear_SECTION();
      set_SECTION(1,0);
      //clearALLDyamic();
      }
  }
  if(buttonState_DATA == HIGH){
    if(_currentTab != 2){
      _currentTab = 2;
      playGiger=false;
      tone(53,250,20);
      _currentScroll = 0;
      _currentSection = 0;
      clear_SECTION();
      set_SECTION(2,0);
      set_TAB_outline(2);

      }
  }
  // else if(buttonState_RADIO == HIGH){
  //   if(_currentTab != 3){
  //     _currentTab = 3;
  //     playGiger=false;
  //     tone(53,250,20);
  //     set_TAB_outline(3);}
  // }
    if (result == DIR_CW) {
      _currentScroll--;
      if(_currentScroll < 0){
        _currentScroll = 0;
      }
      
      tone(53,100,20);
      HandleScroll(_currentScroll,_currentTab,_currentSection);
  } 
  else if (result == DIR_CCW) {
       _currentScroll ++;
       switch(_currentTab){
          case 0:
          if(_currentScroll > 6){
            _currentScroll = 0;
          }
          break;
          case 1:
            if(_currentScroll > 5){
              _currentScroll = 0;
            }
          break;
          case 2:
            if(_currentScroll > 6){
              _currentScroll = 0;
            }
          break;
          case 3:
            if(_currentScroll > 6){
              _currentScroll = 0;
            }
          break;
       }
      
      tone(53,250,20);
      HandleScroll(_currentScroll,_currentTab,_currentSection);
  }
  if (buttonState_DIAL == LOW && !isButtonBouncing()) {
    //TODO: CHANGE TO SECTION CHECKS
    _currentSection++;
    playGiger = false;
    tone(53, 250, 20);
    if (_currentTab == 0) {
      maxSelection = 2;
    } else {
      maxSelection = 3;
    }
    if (_currentSection > maxSelection) {
      _currentSection = 0;
    }
    //changingSECTION = true;
    set_SECTION(_currentTab, _currentSection);
  }
  

 
	
  if(playGiger){
    playGigerTones(delayTime);
  }
  else{
    digitalWrite(pin_GIGER1, HIGH); 
    digitalWrite(pin_GIGER2, HIGH); 
    digitalWrite(pin_GIGER3, HIGH); 
  }
    
  // Put in a slight delay to help debounce the reading
	delay(2);
}
bool isButtonBouncing() {
  // Read the current time
  unsigned long currentTime = millis();
  
  // Check if enough time has passed since the last button press
  if (currentTime - lastDebounceTime < DEBOUNCE_DELAY) {
    return true; // Button is still bouncing
  }
  
  // Update the last debounce time
  lastDebounceTime = currentTime;
  
  return false; // Button is not bouncing
}