#ifndef DYNAMIQUEARRAY_H_
#define DYNAMIQUEARRAY_H_
//Allocation de tableau de ixj: i Row j Colonnes
double** allocTab(int i, int j);
//Donner les valeurs pour la matrice
void giveValue (int i, int j, double** ptr);
//Afficher le contenu de tableau
void printValue (int i, int j, double** ptr);
//Supprimer la tableau
void freeTableau (double** ptr, int i);
#endif

