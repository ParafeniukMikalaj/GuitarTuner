#include "portaudio.h"
#include "stdio.h"
#include <cmath>
#include <QErrorMessage>
#include <QDebug>

#ifndef AUDIOWRAPPER_H
#define AUDIOWRAPPER_H
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (512)
#define NUM_CHANNELS (2)
#define BUF_SIZE 32768
#define TABLE_SIZE 44100

#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"

typedef struct
{
     SAMPLE *data;
     int dataLen;
     int sampleRate;
     int numChannels;
} AudioData;

typedef struct
{
    SAMPLE firstBuf[BUF_SIZE];
    SAMPLE secondBuf[BUF_SIZE];
    bool makeSnapshot;
    int firstPos;
}InputData;

typedef struct
{
    int currentPos;
    float frequency;
}OutputData;

class AudioWrapper
{
private:
    PaStream *outStream, *inStream;
    InputData inputData;
    OutputData outputData;
    AudioData audioData;
    float sinData[TABLE_SIZE];
    static int paoutCallback  ( const void *inputBuffer, void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData );
    static int painCallback   ( const void *inputBuffer, void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData );
    void openOutputStream();
    void closeOutputStream();
    void openInputStream();
    void closeInputStream();
    void showPaError(int error);
    void showError(const char* error);

public:
    AudioWrapper();
    void init();
    void startRecording();
    void stopRecording();
    void startPlaying(float frequency);
    void stopPlaying();
    AudioData getRecordedData();
    void dispose();
};

#endif // AUDIOWRAPPER_H
