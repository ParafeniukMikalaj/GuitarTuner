#ifndef WORKERTHREADOUT_H
#define WORKERTHREADOUT_H
#include <QThread>
#include <QDebug>
#include "audiowrapper.h"

class WorkerThreadOut : public QThread
{
    Q_OBJECT
private:
    AudioWrapper *audio;
    float frequency;
    bool running;
public:
    void setFrequency (float frequency);
    WorkerThreadOut(AudioWrapper *audio);
    void run();

public slots:
    void stopPlaying();
};

#endif // WORKERTHREADOUT_H
