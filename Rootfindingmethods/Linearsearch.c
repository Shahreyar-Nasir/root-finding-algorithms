#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "polynomial.h"
#define TOL 0.5e-6
#define MAXITERATION 10000


typedef struct {
    double root;      // Estimated root of the polynomial
    int iterations;    
} fnData;

fnData LinSearchRootPoly(Poly *P, double a, double b) {
    double fa, fb, x, y;
    int COUNT = 0;

    x = a;
    fa = EvalPoly(P, x);
    
    if (fabs(fa) < TOL) {
        return (fnData){x, COUNT};  
    }

    double step = (b - a) / MAXITERATION;

    while (x < b && COUNT < MAXITERATION) {
        y = x + step;
        if (y > b) y = b; 
        
        fb = EvalPoly(P, y);

       
        if (fa * fb <= 0 || fabs(fb) < TOL) {
            return (fnData){(x+y)/2.0, COUNT};
        }

        x = y;
        fa = fb;  
        COUNT++;
    }

    printf("Root not found with given tolerance.\n");
    return (fnData){NAN, COUNT};
}
int main() {
    Poly P;
    double a, b;

    printf("Enter Degree of polynomial: ");
    scanf("%d", &P.Degree);

    P.Coef = (double *)malloc((P.Degree + 1) * sizeof(double));
    if (!P.Coef) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter Coef (from constant term to highest degree):\n");
    for (int i = 0; i <= P.Degree; ++i) {
        printf("Coef of x^%d: ", i);
        scanf("%lf", &P.Coef[i]);
    }

    printf("Enter interval [a, b] to search for root:\n");
    printf("a: ");
    scanf("%lf", &a);
    printf("b: ");
    scanf("%lf", &b);

    if (a > b) {
        double temp = a;
        a = b;
        b = temp;
    }

    fnData result = LinSearchRootPoly(&P, a, b);

    if (!isnan(result.root)) {
        printf("Root found at x = %.10lf in %d iterations.\n", result.root, result.iterations);
    } else {
        printf("No root found in the interval.\n");
    }

    free(P.Coef);
    return 0;
}
