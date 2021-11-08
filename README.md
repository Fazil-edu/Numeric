## Numerische Verfahren

# 

- **Bisektion**
Das [Bisektionsverfahren](https://de.wikipedia.org/wiki/Bisektion) ist sehr einfaches Verfahern um Nullstellen einer Funktion zu berechnen. Der Pseudocode dafür ist:

``` 
  Bisektion(f, left, right){
    epsilon <- 1/10^10)
    zeroPoint <- right
    while abs(abs(f(zeroPoint))>epsilon) > epsilon
        zeroPoint <- (left+right)/2
        if f(left)*pf(zeroPoint) < 0
            right <- zeroPoint        
        else
            left <- zeroPoint
    return zeroPoint
``` 
- **Regula falsi**
Das [Regula falsi Verfahren](https://de.wikipedia.org/wiki/Regula_falsi) ist auch ein enfaches Verfahren. Es gilt dennoch als langsames Verfahren. Der Pseudocode dafür ist:
``` 
RegulaFalsi(f, left, right)
    epsilon <- 1/10^10
    zeroPoint <- right
    while (abs(f(zeroPoint))>epsilon)
        zeroPoint <- left -(f(left)*(right-left))/(f(right) - f(left))             
        if (f(left)*f(zeroPoint)) < 0)
            right <- zeroPoint
        else
            left <- zeroPoint        
    return zeroPoint
``` 
- **Newton Verfahren**
Das [Newton Verfahren](https://de.wikipedia.org/wiki/Newtonverfahren) ist ein etwas schwieriges dennoch ein schnelles Verfahren. Der Pseudocode dafür ist:

``` 
Newton(f, x_0)
    epsilon <- 1/10^10
    x_1 <- x_0 - f(x_0)/f'(x_1)
    while (abs(f(x_1))>epsilon)
        x_0 <- x_1
        x_1 <- x_0 - f(x_0)/f'(x_1)
    return x_1  
    
    Newton(f, x)
    epsilon <- 1/10^10
    while (abs(f(x))>epsilon)
         x <- x - f(x)/f'(x)
    return x      
``` 
