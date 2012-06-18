#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <string>
#include <QThread>
#include <QDebug>
#include "fftransformer.h"
#include "noteanalyzer.h"
#include "audiowrapper.h"

#define DELAY 50

using namespace std;

class WorkerThreadIn : public QThread
{
    Q_OBJECT
private:
    bool running;
    FFTransformer *transformer;
    NoteAnalyzer *analyzer;
    AudioWrapper *audio;

public:
    WorkerThreadIn(FFTransformer *transformer, NoteAnalyzer *analyzer, AudioWrapper *audio);
    void run();

public slots:
    void stopRecording();
signals:
    void sendData(FFTData *fftData, AnalyzerData *analyzerData);
};

#endif // WORKERTHREAD_H
