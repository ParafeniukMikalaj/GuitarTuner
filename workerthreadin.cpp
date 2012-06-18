#include "workerthreadin.h"

WorkerThreadIn::WorkerThreadIn(FFTransformer *transformer, NoteAnalyzer *analyzer, AudioWrapper *audio)
{
    this->transformer = transformer;
    this->analyzer = analyzer;
    this->audio = audio;
    running = true;
}

void WorkerThreadIn::run()
{
    running = true;
    while(running){
        if (transformer != NULL)
            delete(transformer);
        transformer = new FFTransformer();
        AudioData audioData = audio->getRecordedData();
        FFTData fftData = transformer->analyze(audioData);
        AnalyzerData analyzerData = analyzer->getNote(fftData.fundamentalFrequency);
        emit sendData(&fftData, &analyzerData);
        QThread::msleep(DELAY);
    }
}

void WorkerThreadIn::stopRecording()
{
    running = false;
    audio->stopRecording();
}
