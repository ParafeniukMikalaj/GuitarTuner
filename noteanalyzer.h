#ifndef NOTEANALYZER_H
#define NOTEANALYZER_H


//TODO remove stdlib for abs
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>

#define NOTES_COUNT 12
#define OCTAVES_COUNT 9
#define MIN_FREQ 28
#define MAX_FREQ 5200
#define MAX_CENTS 50

using namespace std;

typedef struct {
    string note;
    string longNote;
    string octave;
    int cents;
} AnalyzerData;

typedef struct {
    int octavesCount;
    int notesCount;
    float notes[NOTES_COUNT][OCTAVES_COUNT];
    string octaves[OCTAVES_COUNT];
    string shortNotes[NOTES_COUNT];
    string longNotes[NOTES_COUNT];
} AnalyzerTableData;

class NoteAnalyzer
{
private:
    AnalyzerData analyzerData;
    static float const notes[NOTES_COUNT][OCTAVES_COUNT];
    static string const octaves[OCTAVES_COUNT];
    static string const shortNotes[NOTES_COUNT];
    static string const longNotes[NOTES_COUNT];
    void getNoteIndeces(float frequency, int &row, int &column);
    float getPrevNote(int row, int column);
    float getNextNote(int row, int column);
public:
    NoteAnalyzer();
    AnalyzerData getNote(float frequency);
    static AnalyzerTableData getData();
};

#endif // NOTEANALYZER_H
