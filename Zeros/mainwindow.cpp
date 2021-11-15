#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "methods.h"
#include <qpair.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->lineEdit->setText("7x^2+x^3-16");
    this->validator = new QRegularExpressionValidator(this->regExp,this);
    ui->spinBox->setValue(9);
    connect(ui->DrawPolynom, SIGNAL(clicked()),this, SLOT(drawThePolynom()));
    connect(ui->Calculate, SIGNAL(clicked()),this, SLOT(Calculate()));
    connect(ui->spinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&MainWindow::setTolaranceValue);
    connect(ui->NewtonRadioButton, SIGNAL(clicked()),this,SLOT(setNewtonSpinBox()));
    connect(ui->BisectionRadioButton, SIGNAL(clicked()),this,SLOT(setNewtonSpinBox()));
    connect(ui->RegulaFalsiRadioButton, SIGNAL(clicked()),this,SLOT(setNewtonSpinBox()));
    ui->BisectionRadioButton->setChecked(true);
    ui->Calculate->setEnabled(false);
    ui->lineEdit->setValidator(validator);
    ui->spinBox->setRange(3,9);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->newtonSpinBox->setVisible(false);
    ui->startPointLabel->setVisible(false);
    ui->newtonSpinBox->setValue(10.5);
    //ui->spinBox->setRange(-10000,10000);
    //ui->spinBox->setMinimum(-10000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setThePolynom(){
    QString polynomString = ui->lineEdit->text();
    if(polynomString.mid(0,1) != "+" || polynomString.mid(0,1) != "-"){
        polynomString="+" + polynomString;
    }
    int pos = 0;

    if(this->validator->validate(polynomString,pos) == QValidator::Acceptable){
       this->polynom = new Polynom(polynomString);
       this->isPolynomSetted = true;
    }
    else{
        this->isPolynomSetted = false;
    }
}

void MainWindow::drawThePolynom(){

    if(!firstDraw){
        ui->customPlot->graph(1)->data().data()->clear();
    }
    firstDraw = false;
    ui->tabWidget->clear();
    this->setGraphs();
    this->setThePolynom();
    if(this->isPolynomSetted){
        ui->customPlot->graph(0)->setData(this->polynom->getXOfPolynom(), this->polynom->getYOfPolynom());
        // give the axes some labels:
        ui->customPlot->xAxis->setLabel("x");
        ui->customPlot->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        ui->customPlot->xAxis->setRange(-100, 100);
        ui->customPlot->yAxis->setRange(-100, 100);
        ui->customPlot->replot();
        ui->customPlot->setInteractions(QCP::iRangeDrag| QCP::iRangeZoom | QCP::iSelectPlottables);
    }
    else{
        QMessageBox msgbx;
        msgbx.setText("Please complet the polynom");
        msgbx.exec();
    }
    ui->Calculate->setEnabled(true);
}

void MainWindow::Calculate(){

    if(ui->NewtonRadioButton->isChecked()){
        ui->DrawPolynom->setEnabled(false);
        ui->Calculate->setEnabled(false);
        Methods(this->polynom,"Newton", this->epsilon,ui->newtonSpinBox->value());
        this->setProgressTable(true);
        this->drawProgress();
        ui->DrawPolynom->setEnabled(true);
    }
    else{
        if(this->polynom->zeroAreas.length() != 0){
            ui->DrawPolynom->setEnabled(false);
            ui->Calculate->setEnabled(false);

            if(ui->BisectionRadioButton->isChecked()){
                Methods(this->polynom, "Bisection", this->epsilon,0);
            }
            else{
                if(ui->RegulaFalsiRadioButton->isChecked()){
                    Methods(this->polynom,"RegulaFalsi", this->epsilon,0);
                }
            }
            this->setProgressTable();
            this->drawProgress();
            ui->DrawPolynom->setEnabled(true);
        }
        else{
            QMessageBox msgbx;
            msgbx.setText("This polynom hast not any zero point");
            msgbx.exec();

        }
    }
}

void MainWindow::drawProgress(){
    for(int i = 0; i<this->polynom->zeroPoints.length(); i++){
        QVector<double> zeroPointsX(i+1), zeroPointsY(i+1);
        for(int k = 0; k<i+1; k++){
            zeroPointsX[k] = this->polynom->zeroPoints[k];
        }
       for(int j = 0; j < this->polynom->progressLines[i].length(); j++){
           QVector<double> x(2), y(2);
           QVector<double> zeroPointCandidatesX(j+1), zeroPointCandidatesY(j+1);
           QPair<QPointF,QPointF> newLine = findTangent(this->polynom->progressLines[i][j]);
           x[0] = newLine.first.x();
           x[1] = newLine.second.x();
           y[0] = newLine.first.y();
           y[1] = newLine.second.y();
           for(int k = 0; k<j+1; k++){
               zeroPointCandidatesX[k] = this->polynom->zeroPointCandidates[i][k].x();
               zeroPointCandidatesY[k] = this->polynom->zeroPointCandidates[i][k].y();
           }
           ui->customPlot->graph(2)->setData(x, y);
           ui->customPlot->graph(3)->setData(zeroPointCandidatesX,zeroPointCandidatesY);
           ui->customPlot->replot();
           this->delay(2);
           if(j == 6){ // drawing max 7 iterations
               break;
           }
       }
       ui->customPlot->graph(1)->setData(zeroPointsX,zeroPointsY);
       ui->customPlot->replot();
    }
       ui->customPlot->graph(2)->data().data()->clear();
       ui->customPlot->graph(3)->data().data()->clear();
       ui->customPlot->replot();
}

void MainWindow::delay(int sec)
{
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::setProgressTable(){
    ui->tabWidget->clear();

    for(int i = 0; i < this->polynom->progressLines.length(); i++){
        QTableWidget *tab = new QTableWidget(this->polynom->progressLines[i].length(),4,ui->tabWidget);

        tab->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QTableWidgetItem *header_item_x1 = new QTableWidgetItem;
        QTableWidgetItem *header_item_x2 = new QTableWidgetItem;
        QTableWidgetItem *header_item_y1 = new QTableWidgetItem;
        QTableWidgetItem *header_item_y2 = new QTableWidgetItem;

        header_item_x1->setText("x1");
        header_item_y1->setText("y1");
        header_item_x2->setText("x2");
        header_item_y2->setText("y2");

        tab->setHorizontalHeaderItem(0,header_item_x1);
        tab->setHorizontalHeaderItem(1,header_item_y1);
        tab->setHorizontalHeaderItem(2,header_item_x2);
        tab->setHorizontalHeaderItem(3,header_item_y2);

        for(int j = 0; j < this->polynom->progressLines[i].length();j++){

            QTableWidgetItem *item_x1 = new QTableWidgetItem;
            QTableWidgetItem *item_x2 = new QTableWidgetItem;
            QTableWidgetItem *item_y1 = new QTableWidgetItem;
            QTableWidgetItem *item_y2 = new QTableWidgetItem;

            item_x1->setText(QString::number(this->polynom->progressLines[i][j].first.x()));
            item_y1->setText(QString::number(this->polynom->progressLines[i][j].first.y()));
            item_x2->setText(QString::number(this->polynom->progressLines[i][j].second.x()));
            item_y2->setText(QString::number(this->polynom->progressLines[i][j].second.y()));

            tab->setItem(j,0,item_x1);
            tab->setItem(j,1,item_y1);
            tab->setItem(j,2,item_x2);
            tab->setItem(j,3,item_y2);
        }
        ui->tabWidget->addTab(tab,QString::number(this->polynom->zeroPoints[i]));
    }
}


void MainWindow::setProgressTable(bool newton){
        ui->tabWidget->clear();

        QTableWidget *tab = new QTableWidget(this->polynom->zeroPointCandidates[0].length(),2,ui->tabWidget);

        tab->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QTableWidgetItem *header_item_x = new QTableWidgetItem;
        QTableWidgetItem *header_item_y = new QTableWidgetItem;

        header_item_x->setText("x");
        header_item_y->setText("y");

        tab->setHorizontalHeaderItem(0,header_item_x);
        tab->setHorizontalHeaderItem(1,header_item_y);


        for(int j = 0; j < this->polynom->zeroPointCandidates[0].length();j++){

            QTableWidgetItem *item_x = new QTableWidgetItem;
            QTableWidgetItem *item_y = new QTableWidgetItem;


            item_x->setText(QString::number(this->polynom->zeroPointCandidates[0][j].x()));
            item_y->setText(QString::number(this->polynom->zeroPointCandidates[0][j].y()));

            tab->setItem(j,0,item_x);
            tab->setItem(j,1,item_y);

        }
        ui->tabWidget->addTab(tab,QString::number(this->polynom->zeroPoints[0]));

}


QPair<QPointF,QPointF> MainWindow::findTangent(QPair<QPointF,QPointF> line){
    double m = (line.first.y() - line.second.y())/(line.first.x() - line.second.x());
    double n = line.first.y() - m*line.first.x();
    double c = qSqrt(qPow((line.first.x()-line.second.x()),2) + qPow((line.first.y()-line.second.y()),2));

    double x_right = 0;
    double x_left = 0;
    double y_left = 0;
    double y_right = 0;
    if(m>0){
        x_right = qSqrt(qPow(c+10,2)/(qPow(m,2)+1)) + line.first.x();
        x_left = line.first.x() - qAbs(x_right - line.second.x());
    }
    else{
        x_left = -qSqrt(qPow(c+10,2)/(qPow(m,2)+1)) + line.second.x();
        x_right = line.second.x() + qAbs(x_left - line.first.x());
    }
    y_left = x_left*m +n;
    y_right = x_right*m +n;

    QPair<QPointF,QPointF> output(QPointF(x_left,y_left),QPointF(x_right,y_right));
    return output;
}

void MainWindow::setTolaranceValue(int value){
    int a = ui->spinBox->value();
    QString epsilon = "0.";

    for(int i = 0; i < a; i++){
        epsilon+="0";
    }
    epsilon += "1";
   ui->epsilon->setText(epsilon);
   this->epsilon = epsilon.toDouble();
}

void MainWindow::setGraphs(){
    for(int i = 0; i < 4; i++){
        ui->customPlot->addGraph();
    }
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));                       // it is graph for zero points
    ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);  // it is graph for zero points
    ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);              // it is graph for zero points

    ui->customPlot->graph(2)->setPen(QPen(Qt::green)); // it graph for progress lines

    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
}

void MainWindow::setNewtonSpinBox(){
    if(ui->NewtonRadioButton->isChecked()){
        ui->newtonSpinBox->setVisible(true);
        ui->startPointLabel->setVisible(true);
    }
    else{
        ui->newtonSpinBox->setVisible(false);
        ui->startPointLabel->setVisible(false);
    }

}


