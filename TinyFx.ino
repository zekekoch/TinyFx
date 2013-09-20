// much comes from 
/*------------------------------------#
#----- FASTSPI2 EFFECTS EXAMPLES -----#
#------------ teldredge --------------#
#-------- www.funkboxing.com ---------#
#------ teldredge1979@gmail.com ------#
#-------------------------------------*/

#include <Adafruit_NeoPixel.h>

#define PIN 1
//#define pixelCount 111
#define pixelCount 100
#define LED_COUNT pixelCount

// Sometimes chipsets wire in a backwards sort of way

Adafruit_NeoPixel LEDS = Adafruit_NeoPixel(pixelCount, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  LEDS.begin();
  LEDS.show(); // Initialize all pixels to 'off'

  LEDS.show(); // Initialize all pixels to 'off'

}

//const uint32_t CBlack = LEDS.Color(0,0,0);
//uint32_t CRed = LEDS.Color(255,0,0);
//const uint32_t CBlue = LEDS.Color(0,255,0);
//const uint32_t CGreen = LEDS.Color(0,0,255);
//---LED SETUP STUFF

byte BOTTOM_INDEX = 0;
byte TOP_INDEX = int(LED_COUNT/2);
int FIRST_THIRD = int(pixelCount/3);
int SECOND_THIRD = FIRST_THIRD * 2;

byte EVENODD = LED_COUNT%2;
byte ledMode = 888;             //-START IN RAINBOW LOOP

//---LED FX VARS
byte idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ihue = 0;                //-HUE (0-360)
byte ibright = 0;             //-BRIGHTNESS (0-255)
byte isat = 0;                //-SATURATION (0-255)
bool bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
int lcount = 0;              //-ANOTHER COUNTING VAR

//------------------------------------- UTILITY FXNS --------------------------------------
//---SET THE COLOR OF A SINGLE RGB LED

uint32_t CHSV(int hue, int sat, int val) {
    // hue: 0-359, sat: 0-255, val (lightness): 0-255

    byte r, g, b, base;

    if (sat == 0) { // Achromatic color (gray).
        r=val;
        g=val;
        b=val;
    } else  {
        base = ((255 - sat) * val)>>8;
        switch(hue/60) {
            case 0:
                r = val;
                g = (((val-base)*hue)/60)+base;
                b = base;
                break;
            case 1:
                r = (((val-base)*(60-(hue%60)))/60)+base;
                g = val;
                b = base;
                break;
            case 2:
                r = base;
                g = val;
                b = (((val-base)*(hue%60))/60)+base;
                break;
            case 3:
                r = base;
                g = (((val-base)*(60-(hue%60)))/60)+base;
                b = val;
                break;
            case 4:
                r = (((val-base)*(hue%60))/60)+base;
                g = base;
                b = val;
                break;
            case 5:
                r = val;
                g = base;
                b = (((val-base)*(60-(hue%60)))/60)+base;
                break;
        }

        return LEDS.Color(r,g,b);
    }
}

//---FIND INDEX OF HORIZONAL OPPOSITE LED
int horizontal_index(int i) {
  //-ONLY WORKS WITH INDEX < TOPINDEX
  if (i == BOTTOM_INDEX) {return BOTTOM_INDEX;}
  if (i == TOP_INDEX && EVENODD == 1) {return TOP_INDEX + 1;}
  if (i == TOP_INDEX && EVENODD == 0) {return TOP_INDEX;}
  return LED_COUNT - i;  
}

//---FIND INDEX OF ANTIPODAL OPPOSITE LED

int antipodal_index(int i) {
  //int N2 = int(LED_COUNT/2);
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {iN = ( i + TOP_INDEX ) % LED_COUNT; }
  return iN;
}

//---FIND ADJACENT INDEX CLOCKWISE
int adjacent_cw(int i) {
  int r;
  if (i < LED_COUNT - 1) {r = i + 1;}
  else {r = 0;}
  return r;
}

//---FIND ADJACENT INDEX COUNTER-CLOCKWISE
int adjacent_ccw(int i) {
  int r;
  if (i > 0) {r = i - 1;}
  else {r = LED_COUNT - 1;}
  return r;
}

int nextThird(int i) {
    int iN = i + (int)(pixelCount / 3);
    if (iN >= pixelCount) {
        iN = iN % pixelCount;
    }
    return iN;
}


//------------------------LED EFFECT FUNCTIONS------------------------
void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
    for(int i = 0 ; i < LED_COUNT; i++ ) {
      LEDS.setPixelColor(i, cred, cgrn, cblu);
    }  
    LEDS.show();
    delay(5);
}

void rainbow_fade() {                         //-m2-FADE ALL LEDS THROUGH HSV RAINBOW
    ihue++;
    if (ihue >= 359) {ihue = 0;}
    for(int idex = 0 ; idex < LED_COUNT; idex++ ) {
      LEDS.setPixelColor(idex, CHSV(ihue, 255, 255));
    }
}

void rainbow_loop(int istep) {              //-m3-LOOP HSV RAINBOW
  idex++;
  ihue = ihue + istep;
  if (idex >= LED_COUNT) 
  {
    idex = 0;
  }
  if (ihue >= 359) {
    ihue = 0;
  }
  LEDS.setPixelColor(idex, CHSV(ihue, 255, 255));
}


void rainbow_vertical(int istep) {        //-m23-RAINBOW 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX) {idex = 0;}  
  ihue = ihue + istep;
  if (ihue > 359) {ihue = 0;}
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  LEDS.setPixelColor(idexA, CHSV(ihue, 255, 255));
  LEDS.setPixelColor(idexB, CHSV(ihue, 255, 255));

}


/*
void random_burst() {                         //-m4-RANDOM INDEX/COLOR
  idex = random(0,LED_COUNT);
  ihue = random(0,359);  
  LEDS.setPixelColor(idex, CHSV(ihue, 255, 255));
}
*/

void color_bounce(bool fade) {                    //-m6-BOUNCE COLOR (SIMPLE MULTI-LED FADE)
  if (bouncedirection == 0) {
    idex = idex + 1;
    if (idex == LED_COUNT) {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if (bouncedirection == 1) {
    idex = idex - 1;
    if (idex == 0) {
      bouncedirection = 0;
    }
  }

  byte iL1 = adjacent_cw(idex);
  byte iL2 = adjacent_cw(iL1);
  byte iL3 = adjacent_cw(iL2);
  byte iR1 = adjacent_ccw(idex);
  byte iR2 = adjacent_ccw(iR1);
  byte iR3 = adjacent_ccw(iR2);
  for(byte i = 0; i < LED_COUNT; i++ ) {
    if (i == idex) 
    {
      LEDS.setPixelColor(i, 255, 0, 0);
    }
    else
    {
      if (fade)
      {
        if (i == iL1) {LEDS.setPixelColor(i, 100, 0, 0);}
        else if (i == iL2) {LEDS.setPixelColor(i, 50, 0, 0);}
        else if (i == iL3) {LEDS.setPixelColor(i, 10, 0, 0);}        
        else if (i == iR1) {LEDS.setPixelColor(i, 100, 0, 0);}
        else if (i == iR2) {LEDS.setPixelColor(i, 50, 0, 0);}
        else if (i == iR3) {LEDS.setPixelColor(i, 10, 0, 0);}          
      } 
      LEDS.setPixelColor(i, 0, 0, 0);
    }
  }
  delay(10);
}



void twoColors() {                    //-m7-POLICE LIGHTS (TWO COLOR SINGLE LED)
  idex++;
  if (idex >= LED_COUNT) {idex = 0;}
  int idexR = idex;
  int idexB = antipodal_index(idexR);  
  for(int i = 0; i < LED_COUNT; i++ ) {
    if (i == idexR) 
    {
      // first tracer (red, blue, green (from 0-255))
      LEDS.setPixelColor(i, 255, 0, 255);
    }
    else if (i == idexB) 
    {
      // second tracer (red, blue, green (from 0-255))
      LEDS.setPixelColor(i, 0, 255, 0);
    }    
    else 
    {
      // background color (red, blue, green (from 0-255))
      LEDS.setPixelColor(i, 0, 0, 25);
    }
  }
}


void twoColorsAll() {                  //-m8-POLICE LIGHTS (TWO COLOR SOLID)
  idex++;
  if (idex >= LED_COUNT) {idex = 0;}
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  LEDS.setPixelColor(idexR, 255, 0, 0);
  LEDS.setPixelColor(idexB, 0, 0, 255);
}

void threeColors() {                    //-m7-POLICE LIGHTS (TWO COLOR SINGLE LED)
  idex++;
  if (idex >= LED_COUNT) {idex = 0;}
  int idex1 = idex;
  int idex2 = nextThird(idex1);
  int idex3 = nextThird(idex2);
  for(int i = 0; i < LED_COUNT; i++ ) {
    if (i == idex1) 
    {
      // first tracer (red, blue, green (from 0-255))
      LEDS.setPixelColor(i, 255, 0, 255);
    }
    else if (i == idex2) 
    {
      // second tracer (red, blue, green (from 0-255))
      LEDS.setPixelColor(i, 255, 0, 0);
    }    
    else if (i == idex3) 
    {
      // second tracer (red, blue, green (from 0-255))
      LEDS.setPixelColor(i, 0, 255, 0);
    }    
    else 
    {
      // background color (red, blue, green (from 0-255))
      LEDS.setPixelColor(i, 0, 0, 25);
    }
  }
}


void threeColorsAll() { //-red, white and blue
    idex++;
    if (idex >= pixelCount) {idex = 0;}
    int idexR = idex;
    int idexW = nextThird(idexR);
    int idexB = nextThird(idexW);
    LEDS.setPixelColor(idexR, 255, 0, 0);
    LEDS.setPixelColor(idexW, 255, 255, 255);
    LEDS.setPixelColor(idexB, 0, 0, 255);
}

/*
void flicker(int thishue, int thissat) {            //-m9-FLICKER EFFECT
  int random_bright = random(0,255);
  int random_delay = random(10,100);
  int random_bool = random(0,random_bright);
  if (random_bool < 10) {
    for(int i = 0 ; i < LED_COUNT; i++ ) {
      LEDS.setPixelColor(i, CHSV(thishue, thissat, random_bright));
    }
    LEDS.show();    
    delay(random_delay);
  }
}
*/

/*
void pulse(int value, bool hueOrSat) {    //-m10-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR 
  if (bouncedirection == 0) {
    value++;
    if (value >= 255) {bouncedirection = 1;}
  }
  if (bouncedirection == 1) {
    value = value - 1;
    if (value <= 1) {bouncedirection = 0;}         
  }  
  for(int idex = 0 ; idex < LED_COUNT; idex++ ) {
    if (hueOrSat)
      LEDS.setPixelColor(idex, CHSV(ihue, 255, value));
    else
      LEDS.setPixelColor(idex, CHSV(ihue, value, 255));

  }
}
*/


/*
void fade_vertical(int ihue) {        //-m12-FADE 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX) {idex = 0;}  
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  ibright = ibright + 10;
  if (ibright > 255) {ibright = 0;}
  LEDS.setPixelColor(idexA, CHSV(ihue, 255, ibright));
  LEDS.setPixelColor(idexB, CHSV(ihue, 255, ibright));
}
*/


void color_loop_vardelay() {                    //-m17-COLOR LOOP (SINGLE LED) w/ VARIABLE DELAY
  idex++;
  if (idex > LED_COUNT) {idex = 0;}
  int di = abs(TOP_INDEX - idex);
  int t = constrain((10/di)*10, 10, 500);
  for(int i = 0; i < LED_COUNT; i++ ) {
    if (i == idex) {
      LEDS.setPixelColor(i, CHSV(0, 255, 255));  
    }
    else {
      LEDS.setPixelColor(i, 0,0,0);  
    }
  }
  LEDS.show();  
  delay(t);
}


/*
void sin_bright_wave(int ihue, int idelay) {        //-m19-BRIGHTNESS SINE WAVE
  static float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
  for(int i = 0; i < LED_COUNT; i++ ) {
    tcount += .1;
    if (tcount > 3.14) {tcount = 0.0;}
    ibright = int(sin(tcount)*255);
    LEDS.setPixelColor(i, CHSV(ihue, 255, ibright));
    LEDS.show();    
    delay(idelay);
  }
}
*/

void pop_horizontal(int ihue) {        //-m20-POP FROM LEFT TO RIGHT UP THE RING
  int ix;
  if (bouncedirection == 0) {
    bouncedirection = 1;
    ix = idex;  
  }
  else if (bouncedirection == 1) {
    bouncedirection = 0;
    ix = horizontal_index(idex);
    idex++;
    if (idex > TOP_INDEX) {idex = 0;}      
  }
  for(int i = 0; i < LED_COUNT; i++ ) {
    if (i == ix) {
      LEDS.setPixelColor(i, CHSV(ihue, 255, 255));
    }
    else {
      LEDS.setPixelColor(i, 0,0,0);
    }
  }
}

/*
void flame() {                                    //-m22-FLAMEISH EFFECT
  int idelay = random(0,35);
  float hmin = 0.1; float hmax = 45.0;
  float hdif = hmax-hmin;
  int randtemp = random(0,3);
  float hinc = (hdif/float(TOP_INDEX))+randtemp;
  int ihue = hmin;
  for(int i = 0; i < TOP_INDEX; i++ ) {
    ihue = ihue + hinc;
    LEDS.setPixelColor(i, CHSV(ihue, 255, 255));
    int ih = horizontal_index(i);    
    LEDS.setPixelColor(ih, CHSV(ihue, 255, 255));
    LEDS.setPixelColor(TOP_INDEX, 255, 255, 255);
    LEDS.show();    
    delay(idelay);
  }
}
*/


/*
void new_rainbow_loop(int idelay){                       //-m88-RAINBOW FADE FROM FAST_SPI2
  ihue -= 1;
  fill_rainbow( leds, LED_COUNT, ihue );  
  LEDS.show();
  delay(idelay);
}
*/

void demo_mode(){
  for(int i = 0;i<500;i++)
  {
    threeColors();
    LEDS.show();
    delay(20);
  }
  for(int i = 0;i<500;i++)
  {
    threeColorsAll();
    LEDS.show();
    delay(20);
  }
  for(int i = 0;i<500;i++)
  {
    twoColors();
    LEDS.show();
    delay(20);
  }
    for(int i = 0;i<500;i++)
  {
    twoColorsAll();
    LEDS.show();
    delay(20);
  }

  /*
  for(int i = 0;i<500;i++)
  {
    rainbow_loop(10);
    LEDS.show();
    delay(10);
  }
  for(int i = 0;i<500;i++)
  {
    color_bounce(true);
    LEDS.show();
    delay(10);
  }
*/
//  for(int i=0; i<r*6; i++) {police_lightsONE();}
//  for(int i=0; i<r*5; i++) {police_lightsALL();}
//    LEDS.show();
//  rainbow_vertical(15);

//  for(int i=0; i<r*20; i++) {random_burst();}

  /*
  for(int i=0; i<r*3; i++) {one_color_all(255,255,255);}

//  for(int i=0; i<r*40; i++) {flicker(160, 50);}
//  for(int i=0; i<r*50; i++) {pulse(0, true);}
//  for(int i=0; i<r*35; i++) {pulse(0, false);}
//  for(int i=0; i<r*5; i++) {fade_vertical(240);}
  one_color_all(0,0,0);
//  for(int i=0; i<r*15; i++) {color_loop_vardelay();}
//  for(int i=0; i<r; i++) {sin_bright_wave(240, 35);}
  for(int i=0; i<r*5; i++) {pop_horizontal(0);}
  one_color_all(0,0,0);  
//  for(int i=0; i<r*3; i++) {flame();}
 */
/*  for(int i=0; i<r*3; i++) {one_color_all(255,0,0);}
  for(int i=0; i<r*3; i++) {one_color_all(0,255,0);}
  for(int i=0; i<r*3; i++) {one_color_all(0,0,255);}
  for(int i=0; i<r*3; i++) {one_color_all(255,255,0);}
  for(int i=0; i<r*3; i++) {one_color_all(0,255,255);}
  for(int i=0; i<r*3; i++) {one_color_all(255,0,255);}
  */
//  for(int i=0; i<r*120; i++) {new_rainbow_loop(5);}  
}


//------------------MAIN LOOP------------------
void loop() {
/*
  if (ledMode == 0) {one_color_all(0,0,0);}            //---STRIP OFF
  if (ledMode == 1) {one_color_all(255,255,255);}      //---STRIP SOLID WHITE
  if (ledMode == 2) {rainbow_fade();}                //---STRIP RAINBOW FADE
  if (ledMode == 3) {rainbow_loop(5);}             //---RAINBOW LOOP
  if (ledMode == 4) {random_burst();}                //---RANDOM
  //if (ledMode == 5) {color_bounce(false);}                //---CYLON v1
  //if (ledMode == 6) {color_bounce(true);}            //---CYLON v2
  if (ledMode == 7) {police_lightsONE();}            //---POLICE LIGHTS SINGLE
  if (ledMode == 8) {police_lightsALL();}            //---POLICE LIGHTS SOLID
//  if (ledMode == 9) {flicker(160,50);}                //---STRIP FLICKER
  if (ledMode == 10) {pulse(0, true);}     //---PULSE COLOR BRIGHTNESS
  if (ledMode == 11) {pulse(0, false);} //---PULSE COLOR SATURATION
  if (ledMode == 12) {fade_vertical(240);}         //---VERTICAL SOMETHING
  if (ledMode == 17) {color_loop_vardelay();}          //---VARIABLE DELAY LOOP
  if (ledMode == 19) {sin_bright_wave(240, 35);}       //---SIN WAVE BRIGHTNESS
  if (ledMode == 20) {pop_horizontal(0);}         //---POP LEFT/RIGHT
  if (ledMode == 22) {flame();}                        //---FLAME-ISH EFFECT
 // if (ledMode == 23) {rainbow_vertical(101);}       //---VERITCAL RAINBOW
  
  // if (ledMode == 88) {new_rainbow_loop(5);}                  //---RAINBOW FADE FROM FAST_SPI CODE

  if (ledMode == 101) {one_color_all(255,0,0);}        //---101- STRIP SOLID RED
  if (ledMode == 102) {one_color_all(0,255,0);}        //---102- STRIP SOLID GREEN
  if (ledMode == 103) {one_color_all(0,0,255);}        //---103- STRIP SOLID BLUE
  if (ledMode == 104) {one_color_all(255,255,0);}      //---104- STRIP SOLID YELLOW
  if (ledMode == 105) {one_color_all(0,255,255);}      //---105- STRIP SOLID TEAL?
  if (ledMode == 106) {one_color_all(255,0,255);}      //---106- STRIP SOLID VIOLET?
  if (ledMode == 888) {demo_mode();}                   //---CYCLE MODES (BLOCKS SERIAL INPUT)
*/
  demo_mode();
  //---PROCESS SERIAL COMMANDS AND ARGS
}