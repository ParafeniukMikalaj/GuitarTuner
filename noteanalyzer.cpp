#include "noteanalyzer.h"

NoteAnalyzer::NoteAnalyzer()
{
}

float const NoteAnalyzer::notes[NOTES_COUNT][OCTAVES_COUNT] = {
    {0.00,   32.70,	65.41,	130.82,	261.63,	523.25,	1046.50,	2093.00,	4186.00},
    {0.00,   34.65,	69.30,	138.59,	277.18,	554.36,	1108.70,	2217.40,	4434.80},
    {0.00,   36.95,	73.91,	147.83,	293.66,	587.32,	1174.60,	2349.20,	4698.40},
    {0.00,   38.88,	77.78,	155.56,	311.13,	622.26,	1244.50,	2489.00,	4978.00},
    {20.61,	41.21,	82.41,	164.81,	329.63,	659.26,	1318.50,	2637.00,	5274.00},
    {21.82,	43.65,	87.31,	174.62,	349.23,	698.46,	1396.90,	2793.80,	0.00},
    {23.12,	46.25,	92.50,	185.00,	369.99,	739.98,	1480.00,	2960.00,	0.00},
    {24.50,	49.00,	98.00,	196.00,	392.00,	784.00,	1568.00,	3136.00,	0.00},
    {25.95,	51.90,	103.80,	207.00,	415.30,	830.60,	1661.20,	3332.40,	0.00},
    {27.50,	55.00,	110.00,	220.00,	440.00,	880.00,	1720.00,	3440.00,	0.00},
    {29.13,	58.26,	116.54,	233.08,	466.16,	932.32,	1864.60,	3729.20,	0.00},
    {30.87,	61.74,	123.48,	246.96,	493.88,	987.75,	1975.50,	3951.00,	0.00}
};

string const NoteAnalyzer::octaves[OCTAVES_COUNT] = {
    "Sub-contr", "Contr", "Big", "Small", "1.0", "2.0", "3.0", "4.0", "5.0"
};

string const NoteAnalyzer::shortNotes[NOTES_COUNT] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "B", "H"
};

string const NoteAnalyzer::longNotes[NOTES_COUNT] = {
    "do", "do#", "re", "re#", "mi", "fa", "fa#", "sol", "sol#", "la", "sib", "si"
};

float NoteAnalyzer::getPrevNote(int row, int column)
{
    row--;
    if(row < 0){
        column--;
        row = NOTES_COUNT - 1;
    }
    return notes[row][column];
}

float NoteAnalyzer::getNextNote(int row, int column)
{
    row++;
    if(row > NOTES_COUNT) {
        column++;
        row = 0;
    }
    return notes[row][column];
}

AnalyzerTableData NoteAnalyzer::getData()
{
    AnalyzerTableData *tableData = new AnalyzerTableData();
    tableData->octavesCount = OCTAVES_COUNT;
    tableData->notesCount = NOTES_COUNT;
    for(int i = 0; i < NOTES_COUNT; i++){
        tableData->shortNotes[i] = shortNotes[i];
        tableData->longNotes[i] = longNotes[i];
        for(int j = 0; j < OCTAVES_COUNT; j++) {
            if (i == 0)
                tableData->octaves[j] = octaves[j];
            tableData->notes[i][j] = notes[i][j];
        }
    }
    return *tableData;
}

//TODO remove linear search
void NoteAnalyzer::getNoteIndeces(float frequency, int &row, int &column)
{
    float min = 100000;
    for(int i = 0; i < NOTES_COUNT; i++)
        for(int j = 0; j < OCTAVES_COUNT; j++){
            float delta = abs(frequency - notes[i][j]);
            if(delta < min){
                row = i;
                column = j;
                min = delta;
            }
        }
}

AnalyzerData NoteAnalyzer::getNote(float frequency)
{
    int row, column;
    if(frequency < MIN_FREQ || frequency > MAX_FREQ) {
        analyzerData.cents = 0;
        analyzerData.octave = "-";
        analyzerData.note = "-";
    }
    getNoteIndeces(frequency, row, column);
    float current = notes[row][column];
    float prev = getPrevNote(row, column);
    float next = getNextNote(row, column);
    float left = (prev + current)/2;
    float right = (current + next)/2;
    if (frequency < current)
        analyzerData.cents = MAX_CENTS * (frequency - current )/(current - left);
    else
    analyzerData.cents = MAX_CENTS * (frequency - current)/(right - current);
    analyzerData.longNote = longNotes[row];
    analyzerData.note = shortNotes[row];
    analyzerData.octave = octaves[column];
    return analyzerData;
}
