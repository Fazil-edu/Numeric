#ifndef METHODS_H
#define METHODS_H

#include "polynom.h"
#include <qobject.h>

class Methods:public QObject
{
public:
    Methods(Polynom *polynom, QString methodName, double epsilon, double newtonBegin);
    explicit Methods();
    void Bisektion(Polynom * polynom, double left, double right, int zeroPointIndex, double epsilon);
    void RegulaFalsi(Polynom * polynom, double left, double right, int zeroPointIndex, double epsilon);
    void Newton(Polynom *polynom, double epsilon);
    void Newton(Polynom * polynom, double x, int zeroPointIndex,double epsilon);
private:
    void findZeroAreas(Polynom * polynom);
};

#endif // METHODS_H
