//Code for AD9833 Waveform Generator
//library dan variable initiation
//library
#include <SPI.h>
#include <MD_AD9833.h>
#include <MCP41_Simple.h>
//pin
#define DAT_PIN 11
#define CLK_PIN 13
#define FNC_PIN 8
#define CS_PIN 9
//library's alias conversion
MD_AD9833 gen(FNC_PIN);
MCP41_Simple dp;
//variabel string
const byte numchar = 100;
char receivedchar[numchar];
char tempchar[numchar];
//digipot variable
float maxres= 10000;
float wipres = 0;

//parsing input data variable
char idn[numchar] = {0};
int waveMo;
float satu;
uint32_t dua = 0;
uint32_t tiga = 0;
uint32_t empat;
uint32_t lima;
//condition variable 
boolean data = false;

void setup() {
    Serial.begin(9600);
    gen.begin();
    dp.begin(CS_PIN);
}

void loop() {
  receiveit();
  if (data == true) {
    strcpy(tempchar,receivedchar);
    parsedit();
    generate();
    data = false;
  }
  }
  
// this function used to set data input method  
void receiveit() {
  static boolean progress = false;
  static byte ndx = 0;
  char start = '<';
  char endm = '>';
  char rc;

  while (Serial.available() >0 && data == false) {
    rc = Serial.read();
    if(progress == true) {
      if(rc != endm){
        receivedchar[ndx] = rc;
        ndx++;
        if (ndx >= numchar) {
          ndx = numchar - 1;
        }
      }
      else {
        receivedchar[ndx] = '\0';
        progress = false;
        ndx = 0;
        data = true;
      }
    }
    else if(rc == start) {
      progress = true;
    }
  }
}

//this function tasked to separating input data into several data
void parsedit() {
  char * stokindx;
  stokindx = strtok(tempchar, ",");
  //idn to set sigzen mode
  strcpy(idn, stokindx);
  stokindx = strtok(NULL, ",");
  //waveMo to set signal mode
  waveMo = atoi(stokindx);
  stokindx = strtok(NULL, ",");
  //satu to set amplitude input
  satu = atof(stokindx);
  stokindx = strtok(NULL, ",");
  //dua to set frequency input
  dua = atol(stokindx);
  stokindx = strtok(NULL, ",");
  //tiga to set sweep last-frequency input
  tiga = atol(stokindx);
  stokindx = strtok(NULL, ",");
  //empat to set sweep duration
  empat = atol(stokindx); 
  stokindx = strtok(NULL, ",");
  //lima to set sweep frequency increment
  lima = atol(stokindx);
}

//this function tasked to set the signal mode
void waveMode(){
  MD_AD9833::mode_t mode = gen.getMode();
  switch(waveMo){
    case '0': mode = MD_AD9833::MODE_OFF;break;
    case '1': mode = MD_AD9833::MODE_SINE; break;
    case '2': mode = MD_AD9833::MODE_TRIANGLE; break;
    case '3': mode = MD_AD9833::MODE_SQUARE1; break;

    default: mode = MD_AD9833::MODE_SINE;break;
  }
  gen.setMode(waveMo);
}

void timer(uint32_t hz){
  for (uint32_t hz = dua; hz <=tiga; hz+= lima){
    gen.setFrequency(MD_AD9833::CHAN_0,hz);
    //delay(empat);
    unsigned long period = (empat*1000);
    unsigned long currentTime;
    currentTime = millis();
    while(millis() < currentTime + period){
}
  }
}

//this function set the frequency value
void startwave(uint32_t hz){
  gen.setFrequency(MD_AD9833::CHAN_0,hz);
}

//this function used to set mode, and controlling the required data
void generate(){
  amplitude();
  if(idn[0] == 'f'){
  uint32_t hz = dua;
  waveMode();
  startwave(hz);
  }
  else if(idn[0] == 's'){
  uint32_t hz;
  timer(hz);
  }
}

//this function tasked to set amplitude value that will be digi-potentiometer input
void amplitude(){
  float amplitude = satu;
  float amptores = amplitude * (10/3.55);
  float wipval=((amptores-wipres)/maxres)* 255;
  dp.setWiper(wipval);
  delay(500);
}
