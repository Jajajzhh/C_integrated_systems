#ifndef MATRICE_H_
#define MATRICE_H_

#include <ostream>
using namespace std;

class Matrice{
	private:

	public:
	//Generation de la matrice double aleatoire de m lignes et n colonnes
   double** randMatrice(int m, int n);
   //Generation de la matrice identite de taille mxm
   double** idMatrice(int m);
   //Generation de matrice zero
   double** zeroMatrice(int m, int n);
   //Generation de matrice compris de 1
   double** oneMatrice(int m, int n);
   //Transposer la matrice de mxn a nxm
   double** transposition(double** A, int m, int n);
   //Addition de 2 matrices de taille mxn
   double** addition(double** A, double** B, int m, int n);
   //Sousctraction matrice A - B de taille mxn
   double** soustraction(double** A, double** B, int m, int n);
   //Produit Hadamard de matrice A B de taille mxn
   double** produitHadamard(double** A, double** B, int m, int n);
   //Produit de 2 Matrice AxB de taille mxn pxq
   double** produitMatriciel(double** A, double** B, int m, int n, int p, int q);
    double** produit(double** A, int m, int n, double k);
    double** newMat(int i, int j);
};
#endif
