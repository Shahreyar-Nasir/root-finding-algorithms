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

fnData RegulaFalsiRootPoly(Poly *P, double a, double b){
    fnData result;
    
    double f_a = EvalPoly(P, a);
    double f_b = EvalPoly(P, b);
    
    // Ensure f(a) and f(b) have opposite signs
    if (f_a * f_b > 0) {
        result.root = NAN;
        result.iterations = 0;
        
        return result;
    }
    
    // If one of the endpoints is already a root
    if (fabs(f_a) < TOL) {
        result.root = a;
        result.iterations = 0;
        
        return result;
    }
    if (fabs(f_b) < TOL) {
        result.root = b;
        result.iterations = 0;
       
        return result;
    }
    
    double c;
    int iterations = 0;
    
    while (iterations < MAXITERATION) {
        // Compute the new approximation using the Regula Falsi formula
        c = (a * f_b - b * f_a) / (f_b - f_a);
        double f_c = EvalPoly(P, c);
    
        if (fabs(f_c) < TOL) {
            result.root = c;
            result.iterations = iterations;
            return result;
        }
    
        
        if (f_a * f_c < 0) {
            b = c;
            f_b = f_c;
        } else {
            a = c;
            f_a = f_c;
        }
    
        iterations++;
    }
    
    result.root = c;
    result.iterations = iterations;
    
    return result;
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

    fnData result = RegulaFalsiRootPoly(&P, a, b);

    if (!isnan(result.root)) {
        printf("Root found at x = %.10lf in %d iterations.\n", result.root, result.iterations);
    } else {
        printf("No root found in the interval.\n");
    }

    free(P.Coef);
    return 0;
}
