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

fnData VarStepRootPoly(Poly *P, double a, double b) {
    double FACTOR =1.5;
   // Check if the root is exactly at the endpoints
   if (fabs(EvalPoly(P,a)) <= TOL) {
       return (fnData){a, 0};
   }
   if (fabs(EvalPoly(P,b)) <= TOL) {
       return (fnData){b, 0};
   }
   double x = a, d = (b - a) / 100.0;  
   double prev_x, prev_y, y;
   int iteration = 0;

   prev_x = x;
   prev_y = EvalPoly(P,prev_x);

   while (fabs(d) > TOL && x >= a && x <= b && iteration < MAXITERATION) {
       x += d;
       y = EvalPoly(P,x);

       // Check if we found a root within tolerance
       if (fabs(y) <= TOL) {
           printf("Root found at x = %.18lf, f(x) = %.18lf\n", x, y);
           return  (fnData){x, iteration};
       }

       // If sign change detected, refine and reverse step
       if ((prev_y * y) < 0) {  
           d = -d /FACTOR; 
           x += d; 
       } else {
           d *= FACTOR;  
       }

       // Prevent floating-point underflow in step size
       if (fabs(d) < 1e-17) d = (b - a) / 1e6;  

       prev_x = x;
       prev_y = y;
       iteration++;
   }
       printf("Root not found with given tolerance.\n");
        return (fnData){x, iteration};
   

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

    fnData result = VarStepRootPoly(&P, a, b);

    if (!isnan(result.root)) {
        printf("Root found at x = %.10lf in %d iterations.\n", result.root, result.iterations);
    } else {
        printf("No root found in the interval.\n");
    }

    free(P.Coef);
    return 0;
}
