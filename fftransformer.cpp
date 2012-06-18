#include "fftransformer.h"

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

FFTransformer::FFTransformer()
{
    frequency = NULL;
    internalData = NULL;
}

FFTData FFTransformer::analyze(AudioData audioData)
{
    float *data = audioData.data;
    int dataLen = audioData.dataLen;
    int sampleRate = audioData.sampleRate;
    if(this->frequency != NULL)
        delete frequency;
    if(this->internalData != NULL)
        delete internalData;

    int internalCount = (int)pow(2, ceil(log2(dataLen)));
    internalCount *= 2;
    internalData = new float[internalCount];
    memset(internalData, 0, internalCount);
    for (int i=0; i < dataLen; i++)
        internalData[i*2] = data[i];

    resolution = ((float)sampleRate) / dataLen * audioData.numChannels;
    FFT(internalData, internalCount);
    countFrequency(internalData, internalCount);

    outputData.data = data;
    outputData.dataLen = dataLen;
    outputData.frequency = frequency;
    outputData.frequencyLen = frequencyLen;
    outputData.resolution = resolution;
    outputData.fundamentalFrequency = fundamentalFrequency;
    return outputData;
}

void FFTransformer::FFT(float data[], int dataLen)
{

    unsigned long n,mmax,m,j,istep,i;
    double wtemp,wr,wpr,wpi,wi,theta,tempr,tempi;

    n=dataLen;
    j=0;
    for (i=0;i<n/2;i+=2) {
        if (j > i) {
            SWAP(data[j],data[i]);
            SWAP(data[j+1],data[i+1]);
            if((j/2)<(n/4)){
                SWAP(data[(n-(i+2))],data[(n-(j+2))]);
                SWAP(data[(n-(i+2))+1],data[(n-(j+2))+1]);
            }
        }
        m=n >> 1;
        while (m >= 2 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    mmax=2;
    while (n > mmax) {
        istep=mmax << 1;
        theta=2*pi/mmax;
        wtemp=sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi=sin(theta);
        wr=1.0;
        wi=0.0;
        for (m=1;m<mmax;m+=2) {
            for (i=m;i<=n;i+=istep) {
                j=i+mmax;
                tempr=wr*data[j-1]-wi*data[j];
                tempi=wr*data[j]+wi*data[j-1];
                data[j-1]=data[i-1]-tempr;
                data[j]=data[i]-tempi;
                data[i-1] += tempr;
                data[i] += tempi;
            }
            wr=(wtemp=wr)*wpr-wi*wpi+wr;
            wi=wi*wpr+wtemp*wpi+wi;
        }
        mmax=istep;
    }
}

void FFTransformer::countFrequency(float data[], int data_len)
{
    frequencyLen = data_len / 4;
    frequency = new float[frequencyLen];
    memset(frequency, 0, frequencyLen);
    float max = -10000;
    int index = 2;
    for (int i = 0; i < frequencyLen; i++){
        frequency[i] = pow(data[i*2],2) + pow(data[i*2+1],2);
        if (frequency[i] > max) {
            max = frequency[i];
            index = i;
        }
    }
    fundamentalFrequency = index * resolution;
}
