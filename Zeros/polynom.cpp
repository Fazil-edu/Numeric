#include "polynom.h"
#include <QtMath>



Polynom::Polynom(QVector<double> coefficients, QVector<int> variableDegrees)
{
    this->coefficients = coefficients;
    this->variableDegrees = variableDegrees;
    findCodomain();
}

Polynom::Polynom(QString polynomString){
    QStringList allTerms;
    setAllTermsOfThePolynom(&polynomString,&allTerms);
    for(short int i = 0; i < allTerms.length(); i++){
        if(!(allTerms[i].contains('x'))){
            this->constantOfThePolynom+=allTerms[i].toDouble();
            allTerms.removeAt(i);
        }
    }
    for(short int i = 0; i < allTerms.length(); i++){
        this->coefficients.push_back(this->getTheCoefOfTheTerm(&allTerms[i]));
        this->variableDegrees.push_back(this->getTheDegreeOfTheTerm(&allTerms[i]));
    }

    findCodomain();
}


double Polynom::getTheCoefOfTheTerm(QString * termString){
    if(termString->contains("-x")|termString->contains("+x")){
        QString tmp = termString->left(1) + "1";
        return tmp.toDouble();
    }
    else {
        short int pos = termString->indexOf('x');
        QString tmp = termString->left(pos);
        return tmp.toDouble();
    }
}
double Polynom::getTheDegreeOfTheTerm(QString * termString){
    if(termString->contains('^')){
        short int pos = termString->indexOf('^');
        QString tmp = termString->mid(pos+1,termString->length() - pos);
        return tmp.toDouble();
    }
    else{
        return 1;
    }
}
void Polynom::setAllTermsOfThePolynom(QString * polynomString, QStringList * allTerms){
    int lengthOfPolynomString = polynomString->length();
    short int posBegin = 0;
    short int posEnd = 0;
    for(short int i = 0; i<lengthOfPolynomString; i++){
        if(polynomString->at(i) == '-' || polynomString->at(i) == '+' ){
            posBegin = i;
            for(short int j = i+1; j < lengthOfPolynomString; j++){
                if(polynomString->at(j) == '-' || polynomString->at(j) == '+' ){
                    posEnd = j;
                    allTerms->push_back(polynomString->mid(posBegin,posEnd-posBegin));
                    i = j-1;
                    break;
                }
            }
        }
    }

    allTerms->push_back(polynomString->right(lengthOfPolynomString-posBegin)); // the last term
}

QVector<double> Polynom::getCoefficientsOfThePolynom(){
    return this->coefficients;
}

QVector<int> Polynom::getVariableDegreesOfThePolynom(){
    return this->variableDegrees;
}

double Polynom::getThePolynomValueAtGivenX(double x){
    double y = this->constantOfThePolynom;
    for(int j = 0; j < this->coefficients.size(); j++){
        y += this->coefficients[j]*qPow(x,this->variableDegrees[j]);
    }
    return y;
}


QVector<double> Polynom::getYOfPolynom(){
    return this->y;
}

QVector<double> Polynom::getXOfPolynom(){
    return this->x;
}

void Polynom::findCodomain(){
    this->x[0] = -10000;
    this->y[0] =getThePolynomValueAtGivenX(this->x[0]);

    for(int i=1; i<1000001; i++){
        this->x[i] = i/50.0 - 10000;
        this->y[i] = getThePolynomValueAtGivenX(x[i]);
        if(y[i-1]*y[i] < 0){
            this->zeroAreas.push_back(QPointF(x[i-1],x[i]));
        }
    }
}

Polynom Polynom::findDerivation(){
    QVector<double> coefficients;
    QVector<int> variableDegrees;
    QVector<int> constants;
    for(int i = 0; i<this->coefficients.length(); i++){
        if(this->variableDegrees[i] !=1){
            coefficients.push_back(this->variableDegrees[i]*this->coefficients[i]);
            variableDegrees.push_back(this->variableDegrees[i]-1);
        }
        else{
            constants.push_back(this->coefficients[i]);
        }
    }
    Polynom derivation (coefficients, variableDegrees);
    for(int i = 0; i < constants.length(); i++){
        derivation.constantOfThePolynom +=constants[i];
    }
    return derivation;
}

void Polynom::setDerivationLines(Polynom *derivation, double x, int zeroPointIndex){
    double slope = derivation->getThePolynomValueAtGivenX(x);
    double n = this->getThePolynomValueAtGivenX(x) - slope*x;
    double x_1 = x+3;
    double x_2 = x-3;
    this->progressLines[zeroPointIndex].push_back(QPair<QPointF,QPointF>(QPointF (x_1,slope*x_1+n),QPointF (x_2,slope*x_2+n)));
}




