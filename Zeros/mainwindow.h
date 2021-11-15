#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "polynom.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;
    Polynom *polynom;
    const QRegularExpression regExp = QRegularExpression("([+-]?(?:(?:\\d{0,5}x\\^\\d{1,2})|(?:\\d{1,5}x)|(?:\\d{1,5})|(?:x)))+"); //es funktioniert okay
    QValidator *validator;
    double epsilon = 0.0000000001;
    bool isPolynomSetted = false;
    bool firstDraw = true;

    void delay(int sec);
    void setThePolynom();
    void drawProgress();
    void setProgressTable();
    void setProgressTable(bool newton);
    QPair<QPointF,QPointF>  findTangent(QPair<QPointF,QPointF> line);
    double getToleranceValue(QSpinBox spnbx);
    void setGraphs();

public slots:
    void drawThePolynom();
    void Calculate();
    void setTolaranceValue(int value);
    void setNewtonSpinBox();
};

#endif // MAINWINDOW_H
