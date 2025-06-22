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

fnData NewtonRootPoly(Poly *P, double a){
    fnData result;
    Poly Q;  
    DerivativePoly(P, &Q);
    int iterations = 0;
    double x = a;  // Initial guess
    double f_x, df_x;

    while (iterations < MAXITERATION) {
        f_x = EvalPoly(P, x);   
        df_x = EvalPoly(&Q, x);  
        if (fabs(f_x) < TOL) {
            result.root = x;
            result.iterations = iterations;
            
            return result;
        }

       
        if (fabs(df_x) < TOL) {
            
            return result;
        }

   
        x = x - (f_x / df_x);

        iterations++;
    }

    // Return best approximation if max iterations reached
    result.root = x;
    result.iterations = iterations;
   
    return result;
}

    
int main() {
    Poly P;
    double a;

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

    printf("Enter initial guess\n");
    scanf("%lf", &a);
    

  

    fnData result = NewtonRootPoly(&P, a);

    if (!isnan(result.root)) {
        printf("Root found at x = %.10lf in %d iterations.\n", result.root, result.iterations);
    } else {
        printf("No root found in the interval.\n");
    }

    free(P.Coef);
    return 0;
}
