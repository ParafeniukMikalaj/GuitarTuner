#ifndef FFTRANSORMER_H
#define FFTRANSORMER_H

#include <cmath>
#include <cstring>
#include <QDebug>
#include "audiowrapper.h"

#define pi 3.14159265

using namespace std;

typedef struct {
    float* data;
    int dataLen;
    float* frequency;
    int frequencyLen;
    float resolution;
    float fundamentalFrequency;
}FFTData;

class FFTransformer
{
private:
    float* inputData;
    float* internalData;
    float* frequency;
    FFTData outputData;
    int frequencyLen, inputLen;
    float resolution;
    float fundamentalFrequency;
    void countFrequency(float data[], int data_len);
    void FFT (float data[], int dataLen);
public:
    FFTransformer();
    FFTData analyze(AudioData audioData);
};

#endif // FFTRANSORMER_H
