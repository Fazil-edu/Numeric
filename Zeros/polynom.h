#ifndef POLYNOM_H
#define POLYNOM_H

#include <QVector>
#include <QStringList>
#include <QPointF>
class Polynom
{
public:
    QVector<QVector<QPair<QPointF,QPointF>>> progressLines; // it is by Newton the derivation lines and by others is it iteration lines
    QVector<double> zeroPoints;
    QVector<QVector<QPointF>> zeroPointCandidates;
    QVector<QPointF> zeroAreas;

    Polynom(QVector<double> coefficients, QVector<int> variableDegrees);
    Polynom(QString polynomString);
    QVector<double> getCoefficientsOfThePolynom();
    QVector<int> getVariableDegreesOfThePolynom();
    QVector<double> getXOfPolynom();
    QVector<double> getYOfPolynom();
    double getThePolynomValueAtGivenX(double x);
    QChar findSigh(double polynomValue);
    Polynom findDerivation();
    void setDerivationLines(Polynom *derivation, double x,int zeroPointIndex);
private:
    QVector<double> coefficients;
    QVector<int> variableDegrees;
    double constantOfThePolynom = 0;
    QVector<double> y = QVector<double>(1000001);
    QVector<double> x = QVector<double>(1000001);
    void findCodomain();
    double getTheCoefOfTheTerm(QString * termString);
    double getTheDegreeOfTheTerm(QString * termString);
    void setAllTermsOfThePolynom(QString * polynomString, QStringList * allTerms);
};

#endif // POLYNOM_H
