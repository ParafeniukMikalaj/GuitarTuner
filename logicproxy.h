#include <QObject>
#include <QDebug>
#include "fftransformer.h"
#include "audiowrapper.h"
#include "noteanalyzer.h"
#include "workerthreadin.h"
#include "workerthreadout.h"

#ifndef LOGIC_H
#define LOGIC_H

class LogicProxy : public QObject
{
    Q_OBJECT

private:
    FFTransformer *transformer;
    AudioWrapper *audio;
    NoteAnalyzer *analyzer;
    WorkerThreadIn *workerIn;
    WorkerThreadOut *workerOut;

public:
    LogicProxy();
    void dispose();
    void startRecording();
    void stopRecording();
    void startPlaying(float frequency);
    void stopPlaying();

signals:
    void sendData(FFTData*, AnalyzerData*);
    void stopPlayingSignal();
    void stopRecordingSignal();

public slots:
    void getData(FFTData*, AnalyzerData*);
    void generateSignal(float frequency);
};

#endif // LOGIC_H
