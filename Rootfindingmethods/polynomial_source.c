#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "polynomial.h" 

// Function to initialize a Poly variable
Poly *InitPoly(Poly *P) {
    if(P == NULL) return NULL;
    P->Degree = 0;
    P->Coef = NULL;
    return P;

}

Poly *CreatePoly(Poly *P) {
    if (P == NULL || P->Degree < 0) {
        return NULL;
    }
    P->Coef = (double*)calloc(P->Degree+1, sizeof(double));
    if(P->Coef == NULL){
        printf("Memory allocation failed!\n");
        exit(1);
    }
   return P;

}
Poly *PolyZero(Poly *P, int Degree) {
    if (P == NULL) return NULL;  // Check if P is a valid pointer

    P->Degree = Degree;  // Set the degree of the polynomial

   
    P->Coef = (double*)malloc((Degree + 1) * sizeof(double));
    
    if (P->Coef == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL;
    }

   
    for (int i = 0; i <= Degree; i++) {
        P->Coef[i] = 0.0;
    }

    return P;
}
void PrintPoly(Poly *P) {
    if (!P || !P->Coef) return;
    int firstTerm = 1;
    for (int i = 0; i <= P->Degree; i++) {
        double coeff = P->Coef[i];
        if (coeff != 0.0) {
            if (!firstTerm) 
                printf(coeff < 0 ? " - " : " + ");
            firstTerm = 0;

            printf("%.2f", fabs(coeff));
            if (i < P->Degree) {
                if (i == P->Degree - 1)
                    printf(" x");
                else
                    printf(" x^%d", P->Degree - i);
            }
        }
    }

    if (firstTerm) printf("0");
    printf("\n");
}
double EvalPoly(Poly *P, double x){
    if (P == NULL || P->Coef == NULL || P->Degree < 0) {
        return 0.0;  
    }
    
    double result = P->Coef[0];  // Start with the leading coefficient
    for (int i = 1; i <= P->Degree; i++) {
        result = result * x + P->Coef[i];
    }
    return result;
}
double DeflatePoly(Poly *P, Poly *Q, double x) {
    if (P == NULL || P->Coef == NULL || Q == NULL) {
        return 0.0;  
    }

    if (P->Degree < 1) {
        return P->Coef[0];  
    }

    // Allocate memory for Q
    Q->Degree = P->Degree - 1;
    Q->Coef = (double*)malloc(Q->Degree * sizeof(double));

    double remainder = P->Coef[0]; 
    for (int i = 0; i < Q->Degree; i++) {
        Q->Coef[i] = remainder;  
        remainder = P->Coef[i + 1] + remainder * x; 
    }

    return remainder;  
}
Poly *DerivativePoly(Poly *P, Poly *Q) {
    if (!P || P->Degree == 0) { 
        // If P is NULL or a constant, its derivative is 0
        Q->Degree = 0;
        Q->Coef = (double*)malloc(sizeof(double));
        Q->Coef[0] = 0;
        return Q;
    }

    // Allocate memory for Q to store the derivative
    Q->Degree = P->Degree - 1;
    Q->Coef = (double*)malloc((Q->Degree + 1) * sizeof(double));

    // Compute the derivative coefficients (leading term first)
    for (int i = 0; i < P->Degree; i++) {
        Q->Coef[i] = P->Coef[i] * (P->Degree - i);
    }
    return Q;
}
double EvalPolyNDeriv(Poly *P, int *x) {
    if (!P || !P->Coef || !x) return 0.0;

    int point = x[0];  // x value for evaluation
    int n = x[1];      // Number of derivatives to compute

    // Compute function value P(x)
    double functionValue = EvalPoly(P, point);

    // Prepare storage for derivatives
    Poly Q;
    double tempCoeffs[P->Degree];  // Temporary storage for coefficients
    Q.Coef = tempCoeffs;

    Poly *currentPoly = P;  // Start with the original polynomial

    // Compute n derivatives first (only for internal processing)
    for (int i = 0; i < n; i++) {
        DerivativePoly(currentPoly, &Q);
        currentPoly = &Q;  // Move to the next derivative
        EvalPoly(currentPoly, point);  // Evaluate but do nothing with result
    }

    return functionValue;  // Return only P(x)
}
void DestroyPoly(Poly *P) {
    if (!P) return;  
    free(P->Coef);   
    P->Coef = NULL;  
    P->Degree = 0;   
}
int CopyPoly(Poly *P, Poly *Q) {
    return 0;
}
Poly *ScalarMult(Poly *P, double x) {
    if (!P || !P->Coef) return NULL;  // Validate input

    
    Poly *result = (Poly *)malloc(sizeof(Poly));
    if (!result) {
        fprintf(stderr, "Memory allocation failed for polynomial structure.\n");
        return NULL;
    }

    result->Degree = P->Degree;


    result->Coef = (double *)malloc((P->Degree + 1) * sizeof(double));
    if (!result->Coef) {
        fprintf(stderr, "Memory allocation failed for coefficient array.\n");
        free(result);
        return NULL;
    }

    
    for (int i = 0; i <= P->Degree; i++) {
        result->Coef[i] = P->Coef[i] * x;
    }

    return result;
}
void GenerateData(Poly *P){
    if(P==NULL || P->Coef == NULL){
        printf("invalid\n");
        return;
    }

    float a,b;
    int n;
    char filename[50];

    printf("enter interval: ");
    scanf("%f %f", &a, &b);
    printf("enter number of points: ");
    scanf("%d", &n);
    printf("enter file name (without extension) : ");
    scanf("%s", filename);

    strcat(filename, ".csv");

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("file openning error\n");
        return;
    }

    fprintf(f, "x,y\n");
    float s = (b-a) / (n-1);
    for(int i=0; i<n; i++){
        float x = a + (i*s);
        float y = EvalPoly(P, x);
        printf("%f, %f\n", x,y);
        fprintf(f, "%f,%f\n", x,y);
    }
    fclose(f);
}
Poly *AddPoly(Poly *P, Poly *Q) {
    if (!P || !P->Coef || !Q || !Q->Coef) return NULL;  

    
    int maxDegree = (P->Degree > Q->Degree) ? P->Degree : Q->Degree;
    int minDegree = (P->Degree < Q->Degree) ? P->Degree : Q->Degree;

    
    Poly *result = (Poly *)malloc(sizeof(Poly));
    if (!result) {
        fprintf(stderr, "Memory allocation failed for polynomial structure.\n");
        return NULL;
    }
    result->Degree = maxDegree;

    result->Coef = (double *)calloc(maxDegree + 1, sizeof(double));
    if (!result->Coef) {
        fprintf(stderr, "Memory allocation failed for coefficient array.\n");
        free(result);
        return NULL;
    }

    for (int i = 0; i <= minDegree; i++) {
        result->Coef[maxDegree - i] = P->Coef[P->Degree - i] + Q->Coef[Q->Degree - i];
    }

    if (P->Degree > Q->Degree) {
        for (int i = minDegree + 1; i <= maxDegree; i++) {
            result->Coef[maxDegree - i] = P->Coef[P->Degree - i];
        }
    } else {
        for (int i = minDegree + 1; i <= maxDegree; i++) {
            result->Coef[maxDegree - i] = Q->Coef[Q->Degree - i];
        }
    }

    return result;
}
Poly *SubPoly(Poly *P, Poly *Q) {
    if (!P || !P->Coef || !Q || !Q->Coef) return NULL;  // Validate input

    // Create a negated version of Q
    Poly *negQ = ScalarMult(Q, -1);
    if (!negQ) return NULL;

    // Add P and -Q
    Poly *result = AddPoly(P, negQ);

    // Free allocated memory for negQ
    free(negQ->Coef);
    free(negQ);

    return result;
}
Poly *MultPoly(Poly *P, Poly *Q) {
    if (!P || !P->Coef || !Q || !Q->Coef) return NULL;  // Validate input

    int newDegree = P->Degree + Q->Degree; // Degree of result
    Poly *result = (Poly *)malloc(sizeof(Poly));
    if (!result) {
        fprintf(stderr, "Memory allocation failed for polynomial multiplication.\n");
        return NULL;
    }

    result->Degree = newDegree;
    result->Coef = (double *)calloc(newDegree + 1, sizeof(double)); // Initialize to zero
    if (!result->Coef) {
        fprintf(stderr, "Memory allocation failed for coefficient array.\n");
        free(result);
        return NULL;
    }

   
    for (int i = 0; i <= P->Degree; i++) {
        for (int j = 0; j <= Q->Degree; j++) {
            result->Coef[i + j] += P->Coef[i] * Q->Coef[j];
        }
    }

    return result;
}
Poly *DivPoly(Poly *P, Poly *Q) {
    if (!P || !P->Coef || !Q || !Q->Coef) return NULL;  
    if (Q->Degree < 0 || Q->Coef[0] == 0) {
        fprintf(stderr, "Division by zero polynomial error.\n");
        return NULL;
    }

    if (P->Degree < Q->Degree) {
      
        return PolyZero((Poly *)malloc(sizeof(Poly)), 0);
    }

    int newDegree = P->Degree - Q->Degree;
    Poly *quotient = PolyZero((Poly *)malloc(sizeof(Poly)), newDegree);
    if (!quotient) return NULL;

   
    Poly *remainder = PolyZero((Poly *)malloc(sizeof(Poly)), P->Degree);
    for (int i = 0; i <= P->Degree; i++)
        remainder->Coef[i] = P->Coef[i];

    for (int i = 0; i <= newDegree; i++) {
        int index = remainder->Degree - Q->Degree;
        double coef = remainder->Coef[0] / Q->Coef[0]; 
        quotient->Coef[index] = coef;
        for (int j = 0; j <= Q->Degree; j++) {
            remainder->Coef[j] -= coef * Q->Coef[j];
        }

        
        for (int j = 0; j < remainder->Degree; j++) {
            remainder->Coef[j] = remainder->Coef[j + 1];
        }
        remainder->Coef[remainder->Degree] = 0;
        remainder->Degree--;
    }

    free(remainder->Coef);
    free(remainder);

    return quotient;
}
