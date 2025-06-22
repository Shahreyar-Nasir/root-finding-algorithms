#ifndef POLYNOMIAL_H_INCLUDED
#define POLYNOMIAL_H_INCLUDED

typedef struct Polynomial{
    int Degree;
    double *Coef;
} Poly;


Poly *InitPoly(Poly * P); // Remember to call immediately after declaration of a Poly variable
Poly *CreatePoly(Poly *P); // Ensure that P is a polynomial with no memory allocated for coeff
Poly *PolyZero(Poly *P, int Degree); // Creates a zero polynomial of degree
void PrintPoly(Poly *P); // Prints a valid polynomial or does nothing
double EvalPoly(Poly *P, double x); // An invalid polynomial evaluates to zero
double DeflatePoly(Poly *P, Poly *Q, double x); // Original P, Deflated Q, return value P(r)
Poly *DerivativePoly(Poly *P, Poly *Q); // Original P, Derivative Q
double EvalPolyNDeriv(Poly *P, int *x); // Returns the function value and the first n derivatives
void DestroyPoly(Poly *P); // Frees the memory allocated to a polynomial
void GenerateData(Poly *P); // Takes the interval as floats and the number of points to be 
                            // plotted, and the file name in the current directory, and
                            // generates (x, y) pairs as a csv file. The file extension is .csv

int CopyPoly(Poly *P, Poly *Q); // Assumes that P exists, but Q does not
// Implement or OMIT until needed
Poly *ScalarMult(Poly *P, double x); // Scaler multiply
Poly *AddPoly(Poly *P, Poly *Q); // Addition
Poly *SubPoly(Poly *P, Poly *Q); // Subtraction
// TO IMPLEMENT
Poly *MultPoly(Poly *P, Poly *Q); // Multiplication
Poly *DivPoly(Poly *P, Poly *Q); // Division

#endif // POLYNOMIAL_H_INCLUDED