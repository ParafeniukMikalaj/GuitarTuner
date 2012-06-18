#include "workerthreadout.h"

WorkerThreadOut::WorkerThreadOut(AudioWrapper *audio)
{
    this->audio = audio;
}

void WorkerThreadOut::setFrequency(float frequency)
{
    this->frequency = frequency;
}

void WorkerThreadOut::run()
{    
    audio->startPlaying(frequency);
}

void WorkerThreadOut::stopPlaying()
{
    running = false;
    audio->stopPlaying();
}
