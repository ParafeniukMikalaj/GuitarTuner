#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    counter = 0;
    originalHtml = NULL;
    showLongNotes = false;
    showOctaves = true;
    ui->setupUi(this);

    ui->centsSlider->setRange(-50, 50, 1.0);
    init_generator();
    init_graph();
    center();
    ui->lineEditCents->setDisabled(true);
    ui->lineEditHertz->setDisabled(true);
    lProxy = new LogicProxy();
    connect(lProxy,SIGNAL(sendData(FFTData*, AnalyzerData*)),
            this, SLOT(displayData(FFTData*, AnalyzerData*)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(generateFrequency(float)), lProxy, SLOT(generateSignal(float)));
    connect(ui->eNote, SIGNAL(toggled(bool)), this, SLOT(reacte(bool)));
    connect(ui->HNote, SIGNAL(toggled(bool)), this, SLOT(reactH(bool)));
    connect(ui->GNote, SIGNAL(toggled(bool)), this, SLOT(reactG(bool)));
    connect(ui->DNote, SIGNAL(toggled(bool)), this, SLOT(reactD(bool)));
    connect(ui->ANote, SIGNAL(toggled(bool)), this, SLOT(reactA(bool)));
    connect(ui->ENote, SIGNAL(toggled(bool)), this, SLOT(reactE(bool)));
    connect(ui->generateSignalButton, SIGNAL(clicked()), this, SLOT(generateButtonHandler()));
    connect(ui->notesTable, SIGNAL(cellClicked(int,int)), this, SLOT(generatorTableClickHandler(int,int)));
    connect(ui->generatorButton, SIGNAL(clicked()), this, SLOT(tunerGeneratorButtonHandler()));
    connect(ui->settingsButton, SIGNAL(clicked()), this, SLOT(tunerSettingsButtonHandler()));
    connect(ui->mainTabContainer, SIGNAL(currentChanged(int)), this, SLOT(tabClicked(int)));

    connect(ui->shortRadioButton, SIGNAL(clicked()), this, SLOT(configChanged()));
    connect(ui->longRadioButton, SIGNAL(clicked()), this, SLOT(configChanged()));
    connect(ui->octaveCheckBox, SIGNAL(clicked()), this, SLOT(configChanged()));
    //lProxy->test(300, 44100);
    //lProxy->teestAnalyzer(139);
}

void MainWindow::init_graph()
{
    ui->samplesPlot->setAxisScale(ui->samplesPlot->yLeft, -0.1, 0.1);
    ui->samplesPlot->enableAxis(ui->samplesPlot->yLeft, false);
    ui->samplesPlot->enableAxis(ui->samplesPlot->xBottom, false);

    ui->frequencyPlot->setAxisScale(ui->frequencyPlot->yLeft, 0, 150000);
    ui->frequencyPlot->enableAxis(ui->frequencyPlot->yLeft, false);

    QPen samplePen = QPen(QColor(0, 157, 0));
    QPen frequencyPen = QPen(Qt::blue);

    sampleCurve = new QwtPlotCurve();
    sampleCurve->setPen(samplePen);
    sampleCurve->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    sampleCurve->attach(ui->samplesPlot);

    frequencyCurve = new QwtPlotCurve;
    frequencyCurve->setPen(frequencyPen);
    frequencyCurve->attach(ui->frequencyPlot);
}

void MainWindow::init_generator()
{
    aData = NoteAnalyzer::getData();
    ui->notesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->notesTable->setRowCount(aData.notesCount);
    ui->notesTable->setColumnCount(aData.octavesCount);
    for (int i = 0; i < aData.notesCount; i++)
        for (int j = 0; j < aData.octavesCount; j++){
            QString st = QString::number(aData.notes[i][j]);
            ui->notesTable->setItem(i, j, new QTableWidgetItem(st));
        }
}

void MainWindow::updateGenerator()
{
    for (int i = 0; i < aData.octavesCount; i++){
        QString *st = new QString(aData.octaves[i].c_str());
        ui->notesTable->setHorizontalHeaderItem(i, new QTableWidgetItem(*st));
    }
    for (int i = 0; i < aData.notesCount; i++){
        QString *st = NULL;
        if (showLongNotes)
            st = new QString(aData.longNotes[i].c_str());
        else
            st = new QString(aData.shortNotes[i].c_str());
        ui->notesTable->setVerticalHeaderItem(i, new QTableWidgetItem(*st));
    }
}

void MainWindow::center()
{
  int x, y;
  int screenWidth;
  int screenHeight;

  int WIDTH = frameGeometry().width();
  int HEIGHT = frameGeometry().height();

  QDesktopWidget *desktop = QApplication::desktop();

  screenWidth = desktop->width();
  screenHeight = desktop->height();

  x = (screenWidth - WIDTH) / 2;
  y = (screenHeight - HEIGHT) / 2;

  setGeometry(x, y, WIDTH, HEIGHT);
}

void MainWindow::displayData(FFTData *fftData, AnalyzerData *analyzerData)
{
    int sampleSize = 1000;
    int step = fftData->dataLen/sampleSize;
    double xSamples[sampleSize];
    double ySamples[sampleSize];
    float* samples = fftData->data;
    if (samples == NULL)
        return;
    for(int i = 0 ; i < sampleSize; i++) {
        xSamples[i] = i;
        ySamples[i] = *(samples+i*step);
    }
    sampleCurve->setSamples(xSamples, ySamples, sampleSize);
    ui->samplesPlot->replot();

    float width = fftData->resolution;
    float* frequency = fftData->frequency;
    int approximateLen = MAX_FREQUENCY / width;
    int frequencySize = fftData->frequencyLen < approximateLen ? fftData->frequencyLen : approximateLen;
    double xFrequency[frequencySize];
    double yFrequency[frequencySize];
    for(int i = 0 ; i < frequencySize; i++) {
        xFrequency[i] = i*width;
        yFrequency[i] = *(frequency + i);
    }
    frequencyCurve->setSamples(xFrequency, yFrequency, frequencySize);

    ui->frequencyPlot->replot();

    ui->lineEditCents->setText(QString::number(analyzerData->cents, 'f', 0));
    ui->lineEditHertz->setText(QString::number(fftData->fundamentalFrequency, 'f', 2));
    ui->centsSlider->setValue(analyzerData->cents);
    setNoteHtml(*analyzerData);

}

void MainWindow::setNoteHtml(AnalyzerData analyzerData)
{
    if(originalHtml == NULL) {
        originalHtml = new QString();
        *originalHtml = ui->noteTextEdit->toHtml();
    }
    QString html = *originalHtml;
    if (showLongNotes)
        html.replace("not", analyzerData.longNote.c_str());
    else
        html.replace("not", analyzerData.note.c_str());
    if(showOctaves)
        html.replace("octave", analyzerData.octave.c_str());
    else
        html.replace("octave", "");
    ui->noteTextEdit->setHtml(html);
}


void MainWindow::reacte(bool checked)
{
    if (checked)
        emit generateFrequency(329.63);
}

void MainWindow::reactH(bool checked)
{
    if (checked)
        emit generateFrequency(246.96);
}

void MainWindow::reactG(bool checked)
{
    if (checked)
        emit generateFrequency(196.00);
}

void MainWindow::reactD(bool checked)
{
    if (checked)
        emit generateFrequency(147.83);
}

void MainWindow::reactA(bool checked)
{
    if (checked)
        emit generateFrequency(110.00);
}

void MainWindow::reactE(bool checked)
{
    if (checked)
        emit generateFrequency(82.41);
}

void MainWindow::generateButtonHandler()
{
    bool *ok  = new bool();
    QString st = ui->frequencyLineEdit->text();
    float frequency = st.toFloat(ok);
    if (!ok){
        showError("Input is not in vali format");
        return;
    }
    emit generateFrequency(frequency);
}

void MainWindow::generatorTableClickHandler(int row, int column)
{
    QString st = ui->notesTable->item(row, column)->text();
    float frequency = st.toFloat();
    emit generateFrequency(frequency);
}

void MainWindow::tunerGeneratorButtonHandler()
{
    lProxy->stopRecording();
    ui->mainTabContainer->setCurrentIndex(1);
}

void MainWindow::tunerSettingsButtonHandler()
{
    lProxy->stopRecording();
    ui->mainTabContainer->setCurrentIndex(2);
}

void MainWindow::tunerInfoButtonHandler()
{
    lProxy->stopRecording();
    ui->mainTabContainer->setCurrentIndex(3);
}

void MainWindow::tabClicked(int index)
{
    lProxy->stopPlaying();
    if(index != 0)
        lProxy->stopRecording();
    if(index == 0)
        lProxy->startRecording();
}

void MainWindow::configChanged()
{
    showOctaves = false;
    showLongNotes = true;
    if(ui->shortRadioButton->isChecked())
        showLongNotes = false;
    if(ui->octaveCheckBox->isChecked())
        showOctaves = true;
    updateGenerator();
}

void MainWindow::showError(QString str)
{
    QErrorMessage errorMessage;
    errorMessage.showMessage(str);
    errorMessage.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
    lProxy->dispose();
    delete lProxy;
}
