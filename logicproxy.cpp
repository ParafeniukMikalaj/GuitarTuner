#include "logicproxy.h"

LogicProxy::LogicProxy()
{
    transformer = new FFTransformer();
    analyzer = new NoteAnalyzer();
    audio = new AudioWrapper();
    workerIn = new WorkerThreadIn(transformer, analyzer, audio);
    workerOut = new WorkerThreadOut(audio);
    connect(this, SIGNAL(stopPlayingSignal()),workerOut, SLOT(stopPlaying()), Qt::QueuedConnection);
    connect(this, SIGNAL(stopRecordingSignal()), workerIn, SLOT(stopRecording()), Qt::QueuedConnection);
    connect(workerIn, SIGNAL(sendData(FFTData*, AnalyzerData*)),
            this, SLOT(getData(FFTData*, AnalyzerData*)), Qt::QueuedConnection);
    startRecording();
}

void LogicProxy::dispose()
{
    audio->dispose();
    delete audio;
    delete transformer;
}

void LogicProxy::startRecording()
{
    workerIn->start();
    audio->startRecording();
}

void LogicProxy::stopRecording()
{
    emit stopRecordingSignal();
}

void LogicProxy::startPlaying(float frequency)
{
    workerOut->setFrequency(frequency);
    workerOut->start();
}

void LogicProxy::stopPlaying()
{
    emit stopPlayingSignal();
}

void LogicProxy::getData(FFTData *fftData, AnalyzerData *analyzerData)
{
    emit sendData(fftData, analyzerData);
}

void LogicProxy::generateSignal(float frequency)
{
    audio->stopRecording();
    audio->startPlaying(frequency);
}
