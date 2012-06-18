#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "noteanalyzer.h"
#include <QMainWindow>
#include <QWidget>
#include <QDesktopWidget>
#include "logicproxy.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_slider.h>

#define MAX_FREQUENCY 400


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    AnalyzerTableData aData;
    bool showLongNotes, showOctaves;
    QString *originalHtml;
    QwtPlotCurve *sampleCurve, *frequencyCurve;
    int counter;
    LogicProxy* lProxy;
    Ui::MainWindow *ui;
    void init_graph();
    void init_generator();    
    void updateGenerator();
    void center();
    void setNoteHtml(AnalyzerData analyzerDara);
    void showError(QString str);


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void generateFrequency(float frequency);
    
public slots:
    void displayData(FFTData*, AnalyzerData*);
    void reacte(bool checked);
    void reactH(bool checked);
    void reactG(bool checked);
    void reactD(bool checked);
    void reactA(bool checked);
    void reactE(bool checked);
    void generateButtonHandler();
    void generatorTableClickHandler(int row, int column);
    void tunerGeneratorButtonHandler();
    void tunerSettingsButtonHandler();
    void tunerInfoButtonHandler();
    void tabClicked(int index);
    void configChanged();
};

#endif // MAINWINDOW_H
