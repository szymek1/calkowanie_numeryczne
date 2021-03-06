﻿#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rk4.h"

double E_e;//przechowuje wartosc energi obliczonej w metodzie eulera

void rhs_fun(double t, double* X, double* F);
void veuler(double t, double* X, double h, int n, void (*fun)(double, double*, double*), double* X1);
double energia(double t, double* X, double* F);


int main()
{
	double l = 1, m = 0.05, g = 9.81;
	FILE* wynik = fopen("wynik.txt", "w");
	FILE* energia_e = fopen("energia_e.txt", "w");
	FILE* energia_rk = fopen("energia_rk.txt", "w");
	double t, tk, a0, w0, h;
	tk = 10;
	a0 = 0.2; //0.2 radiana!!!!
	w0 = 0.0;
	h = 1e-4;
	int n = 2;
	double* X, * X1, * Xr, * X1r;
	X = (double*)malloc(n * sizeof(double));
	X1 = (double*)malloc(n * sizeof(double));
	Xr = (double*)malloc(n * sizeof(double));
	X1r = (double*)malloc(n * sizeof(double));
	X[0] = w0;
	X[1] = a0;
	Xr[0] = w0;
	Xr[1] = a0;
	fprintf(wynik, "wE\taE\twRK\taRK\n");
	for (t = 0; t <= tk; t += h)
	{
		veuler(t, X, h, n, rhs_fun, X1);
		vrk4(t, Xr, h, n, rhs_fun, X1r);
		fprintf(energia_e,"%lf\n",E_e);
		printf("%lf\t%lf\t%lf\n", t, X[0], X[1]);
		fprintf(wynik, "%lf\t%lf\t%lf\t%lf\n", X[0], X[1], Xr[0], Xr[1]);
		fprintf(energia_rk, "%lf\n", (0.5 * m * pow(l, 2) * pow(Xr[0], 2)) + (m * g * l * (1 - cos(Xr[1]))));
		X[0] = X1[0];
		X[1] = X1[1];
		Xr[0] = X1r[0];
		Xr[1] = X1r[1];
	}
	free(X);
	free(X1);
	free(Xr);
	free(X1r);
	fclose(wynik);
	fclose(energia_e);
	fclose(energia_rk);
	return 0;
}

void rhs_fun(double t, double* X, double* F)
{
	double g = 9.81;
	int l = 1.0;
	F[0] = (-1) * (g / l) * sin(X[1]);
	F[1] = X[0];
}
void veuler(double t, double* X, double h, int n, void (*fun)(double, double*, double*), double* X1)
{
	double* F = (double*)malloc(n * sizeof(double));
	fun(t, X, F);
	X1[0] = X[0] + (h * F[0]) ;
	X1[1] = X[1] + (h * F[1]);
	E_e = energia(t, X, F);
	free(F);
}
double energia(double t, double* X, double* F)
{
	double l = 1, m = 0.05, g = 9.81;
	//wymiar liny w metrach, ciężar m w kg
	double E;
	E = (0.5 * m * pow(l, 2) * pow(F[1], 2)) + (m * g * l * (1 - cos(X[1])));
	printf("\nEnergia: %lf\t", E);
	return E;
}