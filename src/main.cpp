#include <Arduino.h>
#include "DaisyDuino.h"
#include <MIDI.h>
#include "MidiSync.h"
#include <math.h>

#include "Controls.h"
#include "Beatrepeat.h"
#include "Config.h"



#define BPM 200


void AudioCallback(float **in, float **out, size_t size);




MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);












//clean up audio callback, move beat repeat into effect slot
void AudioCallback(float **in, float **out, size_t size)
{
  CountAudioSamples(size);

  for (size_t i = 0; i < size; i++)
  {
  
      out[0][i] = in[0][i];
      out[1][i] = in[1][i];
  }
}
 
void setup()
{

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleStart(HandleStart);
  MIDI.setHandleClock(HandleClock);
  MIDI.setHandleStop(HandleStop);
  Serial.begin(115200);


  DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  DAISY.begin(AudioCallback);
  InitControls();

}

void loop()
{
  MIDI.read();   
  HandleJoyStick();
  SamplesPerClock();

}


