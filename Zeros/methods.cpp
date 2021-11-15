#include "methods.h"
#include "polynom.h"
#include <QtMath>

Methods::Methods()
{

}

Methods::Methods(Polynom *polynom, QString methodName, double epsilon, double newtonBegin){
    if(methodName == "Newton"){
        //this->Newton(polynom, epsilon);
        Newton(polynom,newtonBegin,0, epsilon);
    }
    else{
        double right = 0;
        double left = 0;
        double euclidianDistance = 0;
        if(polynom->zeroAreas.length()>1){
            //i is zeroPointIndex
            for(int i = 0; i<polynom->zeroAreas.length()-1; i++){

                right = ((polynom->zeroAreas[i].x()+polynom->zeroAreas[i].y())/2 + (polynom->zeroAreas[i+1].x()+polynom->zeroAreas[i+1].y())/2)/2;

                euclidianDistance = (qAbs((polynom->zeroAreas[i].x()+polynom->zeroAreas[i].y()))/2 + qAbs((polynom->zeroAreas[i+1].x()+polynom->zeroAreas[i+1].y())/2))/2;

                left = (polynom->zeroAreas[i].x()+polynom->zeroAreas[i].y())/2 - euclidianDistance;
                if(methodName == "Bisection"){
                    Bisektion(polynom, left,right, i, epsilon);
                }
                else{
                    RegulaFalsi(polynom, left,right, i, epsilon);
                }
            }

            left = ((polynom->zeroAreas[polynom->zeroAreas.length()-1].x()+polynom->zeroAreas[polynom->zeroAreas.length()-1].y())/2 +
                    (polynom->zeroAreas[polynom->zeroAreas.length()-2].x()+polynom->zeroAreas[polynom->zeroAreas.length()-2].y())/2)/2;

            euclidianDistance =(qAbs((polynom->zeroAreas[polynom->zeroAreas.length()-1].x()+polynom->zeroAreas[polynom->zeroAreas.length()-1].y()))/2 +
                    qAbs((polynom->zeroAreas[polynom->zeroAreas.length()-2].x()+polynom->zeroAreas[polynom->zeroAreas.length()-2].y()))/2)/2;

            right = (polynom->zeroAreas[polynom->zeroAreas.length()-1].x()+polynom->zeroAreas[polynom->zeroAreas.length()-1].y())/2 + euclidianDistance;

        }
        else{
            left = (polynom->zeroAreas[0].x()+polynom->zeroAreas[0].y())/2 - 4;
            right =(polynom->zeroAreas[0].x()+polynom->zeroAreas[0].y())/2 + 4;
        }
        if(methodName == "Bisection"){
            Bisektion(polynom, left,right, polynom->zeroAreas.length()-1, epsilon);
        }
        else{
            RegulaFalsi(polynom, left,right, polynom->zeroAreas.length()-1, epsilon);
        }

    }
}


void Methods::Bisektion(Polynom *polynom, double left, double right, int zeroPointIndex, double epsilon){
    polynom->progressLines.push_back(QVector<QPair<QPointF,QPointF>>());
    polynom->zeroPointCandidates.push_back(QVector<QPointF>());
    polynom->zeroPointCandidates[zeroPointIndex].push_back( QPointF(left,polynom->getThePolynomValueAtGivenX(left)));
    polynom->progressLines[zeroPointIndex].push_back( QPair<QPointF,QPointF>(QPointF(left,polynom->getThePolynomValueAtGivenX(left)),QPointF(right,polynom->getThePolynomValueAtGivenX(right))));
    double zeroPoint = right;
    while (qAbs(polynom->getThePolynomValueAtGivenX(zeroPoint))>epsilon) {
        zeroPoint = (left+right)/2;
        if((polynom->getThePolynomValueAtGivenX(left)*polynom->getThePolynomValueAtGivenX(zeroPoint)) < 0){
            right = zeroPoint;
        }
        else{
            left = zeroPoint;
        }
        polynom->zeroPointCandidates[zeroPointIndex].push_back(QPointF(zeroPoint, polynom->getThePolynomValueAtGivenX(zeroPoint)));
        polynom->progressLines[zeroPointIndex].push_back( QPair<QPointF,QPointF>(QPointF(left,polynom->getThePolynomValueAtGivenX(left)),QPointF(right,polynom->getThePolynomValueAtGivenX(right))));
    }
    polynom->zeroPoints.push_back(zeroPoint);
}

void Methods::RegulaFalsi(Polynom * polynom, double left, double right, int zeroPointIndex, double epsilon){
    polynom->progressLines.push_back(QVector<QPair<QPointF,QPointF>>());
    polynom->zeroPointCandidates.push_back(QVector<QPointF>());
    polynom->zeroPointCandidates[zeroPointIndex].push_back( QPointF(left,polynom->getThePolynomValueAtGivenX(left)));
    polynom->progressLines[zeroPointIndex].push_back( QPair<QPointF,QPointF>(QPointF(left,polynom->getThePolynomValueAtGivenX(left)),QPointF(right,polynom->getThePolynomValueAtGivenX(right))));
    double zeroPoint = right;
    while (qAbs(polynom->getThePolynomValueAtGivenX(zeroPoint))>epsilon) {
        zeroPoint = left -
                (polynom->getThePolynomValueAtGivenX(left)*(right-left))/
                (polynom->getThePolynomValueAtGivenX(right) - polynom->getThePolynomValueAtGivenX(left));


        if((polynom->getThePolynomValueAtGivenX(left)*polynom->getThePolynomValueAtGivenX(zeroPoint)) < 0){
            right = zeroPoint;
        }
        else{
            left = zeroPoint;
        }
        polynom->zeroPointCandidates[zeroPointIndex].push_back(QPointF(zeroPoint, polynom->getThePolynomValueAtGivenX(zeroPoint)));
        polynom->progressLines[zeroPointIndex].push_back( QPair<QPointF,QPointF>(QPointF(left,polynom->getThePolynomValueAtGivenX(left)),QPointF(right,polynom->getThePolynomValueAtGivenX(right))));
    }
    polynom->zeroPoints.push_back(zeroPoint);
}

//void Methods::Newton(Polynom *polynom,double epsilon){
//    //double zeroPoint = 0;
//    ////i is zeroPointIndex
//    //for(int i = 0; i<polynom->zeroAreas.length(); i++){
//    //    zeroPoint = (polynom->zeroAreas[i].x()+polynom->zeroAreas[i].y())/2;
//    //    Newton(polynom,zeroPoint,i, epsilon);
//    //}
//
//}
void Methods::Newton(Polynom * polynom, double zeroPoint, int zeroPointIndex, double epsilon){
    polynom->progressLines.push_back(QVector<QPair<QPointF,QPointF>>());
    polynom->zeroPointCandidates.push_back(QVector<QPointF>());
    Polynom derivation = polynom->findDerivation();
    polynom->setDerivationLines(&derivation,zeroPoint,zeroPointIndex);
    polynom->zeroPointCandidates[zeroPointIndex].push_back(QPointF(zeroPoint, polynom->getThePolynomValueAtGivenX(zeroPoint)));
    while (qAbs(polynom->getThePolynomValueAtGivenX(zeroPoint)) > epsilon) {
        zeroPoint = zeroPoint - polynom->getThePolynomValueAtGivenX(zeroPoint)/derivation.getThePolynomValueAtGivenX(zeroPoint);
        polynom->setDerivationLines(&derivation,zeroPoint,zeroPointIndex);
        polynom->zeroPointCandidates[zeroPointIndex].push_back(QPointF(zeroPoint, polynom->getThePolynomValueAtGivenX(zeroPoint)));
    }
    polynom->zeroPoints.push_back(zeroPoint);
}


