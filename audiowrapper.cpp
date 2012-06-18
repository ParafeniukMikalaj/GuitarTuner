#include "audiowrapper.h"

AudioWrapper::AudioWrapper()
{
    outputData.frequency = 0;
    audioData.dataLen = BUF_SIZE;
    audioData.sampleRate = SAMPLE_RATE;
    audioData.numChannels = NUM_CHANNELS;
    audioData.data = inputData.secondBuf;
    for(int i = 0; i < TABLE_SIZE; i++)
        sinData[i] = (float) sin( ((float)i/(float)TABLE_SIZE) * M_PI * 2. );
    init();
}

void AudioWrapper::showPaError(int error)
{
    if (error != paNoError) {
        QErrorMessage errorMessage;
        errorMessage.showMessage(Pa_GetErrorText(error));
        errorMessage.exec();
        throw error;
    }
}

void AudioWrapper::showError(const char *error)
{
    QErrorMessage errorMessage;
    errorMessage.showMessage(error);
    errorMessage.exec();
    throw error;
}

void AudioWrapper::init()
{
    memset(inputData.firstBuf, 0, sizeof(inputData.firstBuf));
    PaError err = Pa_Initialize();
    showPaError(err);
    int count = Pa_GetDeviceCount();
    if(count == 0)
        showError("Zero audio devices");
    openOutputStream();
    openInputStream();
}

void AudioWrapper::dispose()
{
    closeOutputStream();
    closeInputStream();
    PaError err = Pa_Terminate();
    showPaError(err);
}

AudioData AudioWrapper::getRecordedData()
{
    inputData.makeSnapshot = true;
    return audioData;
}

int AudioWrapper::painCallback(const void *inputBuffer,
                           void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData)
{
    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    unsigned long i;


    InputData *inData = (InputData*) userData;
    int pos = inData->firstPos;
    const SAMPLE* inBuffer = (const SAMPLE*) inputBuffer;

    if (inData->makeSnapshot) {
        for (int i = 0; i < BUF_SIZE; i++)
            inData->secondBuf[i] = inData->firstBuf[(i+pos) % BUF_SIZE];
        inData->makeSnapshot = false;
    }

    if( inputBuffer == NULL ) {
        for( i=0; i<framesPerBuffer; i++ )
        {
            inData->firstBuf[++pos % BUF_SIZE] = SAMPLE_SILENCE;  /* left */
            if( NUM_CHANNELS == 2 ) inData->firstBuf[++pos % BUF_SIZE] = SAMPLE_SILENCE; /* right */
        }
    }
    else {
        for( i=0; i<framesPerBuffer; i++ )
        {
           inData->firstBuf[pos++ % BUF_SIZE] = *inBuffer++;   /* left */
           if( NUM_CHANNELS == 2 ) inData->firstBuf[pos++ % BUF_SIZE] = *inBuffer++;  /* right */
        }
    }
    inData->firstPos = pos % BUF_SIZE;
    return paContinue;
}

void AudioWrapper::startRecording()
{
    if (Pa_IsStreamActive(inStream))
        return;
    PaError err = Pa_StartStream(inStream);
    showPaError(err);
}

void AudioWrapper::stopRecording()
{
    if (Pa_IsStreamStopped(inStream))
        return;
    PaError err = Pa_StopStream(inStream);
    showPaError(err);
}

void AudioWrapper::openInputStream()
{
    PaStreamParameters inputParameters;
    inputParameters.device =Pa_GetDefaultInputDevice(); /* default output device */
    if (inputParameters.device == paNoDevice)
      showError("Error: No default input device.\n");
    inputParameters.channelCount = NUM_CHANNELS;       /* stereo output */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE; /* 32 bit floating point output */
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowOutputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    PaError err = Pa_OpenStream(
                                   &inStream,
                                   &inputParameters,
                                   NULL,
                                   SAMPLE_RATE,
                                   paFramesPerBufferUnspecified,
                                   paClipOff,
                                   painCallback,
                                   &inputData);
    showPaError(err);
}

void AudioWrapper::closeInputStream()
{
    PaError err = Pa_CloseStream( inStream );
    showPaError(err);
}

int AudioWrapper::paoutCallback(const void *inputBuffer,
                            void *outputBuffer,
                            unsigned long
                            framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{    
    OutputData *outData = (OutputData*)userData;
    float frequency = outData->frequency;
    int pos = outData->currentPos;
    float *out = (float*)outputBuffer;
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;
    float k = frequency*2*M_PI/SAMPLE_RATE;
    for (unsigned int i = 0; i < framesPerBuffer; i++){
       pos++;
       *out++ = sin((pos)*k);
       *out++ = sin((pos)*k);
    }
    outData->currentPos = pos;
    return 0;
}

void AudioWrapper::openOutputStream()
{    
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice)
      showError("Error: No default output device.\n");
    outputParameters.channelCount = NUM_CHANNELS;       /* stereo output */
    outputParameters.sampleFormat = PA_SAMPLE_TYPE; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaError err = Pa_OpenStream(
                                   &outStream,
                                   NULL,
                                   &outputParameters,
                                   SAMPLE_RATE,
                                   paFramesPerBufferUnspecified,
                                   paClipOff,
                                   paoutCallback,
                                   &outputData);
    showPaError(err);
}

void AudioWrapper::closeOutputStream()
{
    PaError err = Pa_CloseStream( outStream );
    showPaError(err);
}

void AudioWrapper::startPlaying(float frequency)
{
    outputData.frequency = frequency;
    if (Pa_IsStreamActive(outStream))
        return;
    PaError err = Pa_StartStream(outStream);
    showPaError(err);
}

void AudioWrapper::stopPlaying()
{
    if (Pa_IsStreamStopped(outStream))
        return;
    PaError err = Pa_StopStream(outStream);
    showPaError(err);
}
