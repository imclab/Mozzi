/*  Example of filtering a wave,
 *  using Mozzi sonification library.
 *
 *  Demonstrates LowPassFilter().
 *
 *  Circuit: Audio output on digital pin 9 (on a Uno or similar), or 
 *  check the README or http://sensorium.github.com/Mozzi/
 *
 *  Mozzi help/discussion/announcements:
 *  https://groups.google.com/forum/#!forum/mozzi-users
 *
 *  Tim Barrass 2012.
 *  This example code is in the public domain.
 */

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/chum9_int8.h> // recorded audio wavetable
#include <tables/cos2048_int8.h> // for filter modulation
#include <LowPassFilter.h>
#include <mozzi_rand.h> // for rand()
#include <utils.h>

#define CONTROL_RATE 64 // powers of 2 please

Oscil<CHUM9_NUM_CELLS, AUDIO_RATE> aCrunchySound(CHUM9_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);

LowPassFilter lpf;

void setup(){
  setPin13Out();
  aCrunchySound.setFreq(2.f);
  kFilterMod.setFreq(1.3f);
  lpf.setResonance(200);
  startMozzi(CONTROL_RATE);
}

void loop(){
  audioHook();
}

void updateControl(){
  // change the modulation frequency now and then
  //if (lowByte(xorshift96()) < 5){
  if (rand((byte)CONTROL_RATE/2) == 0){ // about once every half second
    kFilterMod.setFreq((float)lowByte(xorshift96())/64);
  }
  // map the modulation into the filter range (0-255)
  unsigned char cutoff_freq = 100 + kFilterMod.next()/2;
  lpf.setCutoffFreq(cutoff_freq);
}

int updateAudio(){
  char asig = lpf.next(aCrunchySound.next());
  return (int) asig;
}






