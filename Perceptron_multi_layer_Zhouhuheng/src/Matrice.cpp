#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "dynamiqueArray.h"
#include "Matrice.h"
using namespace std;

/*class Matrice{
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
 *
#endif
 */
double** Matrice::produit(double **A, int m, int n, double k){
    int cnti,cntj;
    for(cnti=0;cnti<m;cnti++){
        for (cntj=0;cntj<n;cntj++){
            A[cnti][cntj] = A[cnti][cntj] * k;
        }
    }
    return A;
}
double** Matrice::newMat(int m, int n){
    double** matrice = allocTab(m,n);
    int cnti;
    int cntj;
    for(cnti=0;cnti<m;cnti++){
        for (cntj=0;cntj<n;cntj++){
            double tmp;
            tmp = 0;
            matrice[cnti][cntj] = tmp;
        }
    }
    return matrice;
}
double** Matrice::randMatrice(int m, int n){
    	double** matrice = allocTab(m,n);
    	int cnti;
        int cntj;
        for(cnti=0;cnti<m;cnti++){
	      for (cntj=0;cntj<n;cntj++){
            double tmp;
            tmp = (double)rand() / (double)RAND_MAX;
            matrice[cnti][cntj] = tmp;
            }
        }
        return matrice; 
}
double** Matrice::idMatrice(int m){
       double** matrice = allocTab(m,m);
       	int cnti;
        int cntj;
        for(cnti=0;cnti<m;cnti++){
	      for (cntj=0;cntj<m;cntj++){
            double tmp = 0;
            if(cnti==cntj)
              tmp = 1;
            matrice[cnti][cntj] = tmp;
            }
        }
        return matrice;
}
double** Matrice::zeroMatrice(int m, int n){
	double** matrice = allocTab(m,n);
    	int cnti;
        int cntj;
        for(cnti=0;cnti<m;cnti++){
	      for (cntj=0;cntj<n;cntj++){
          
            matrice[cnti][cntj] = 0;
            }
        }
        return matrice; 
}
double** Matrice::oneMatrice(int m, int n){
	double** matrice = allocTab(m,n);
    	int cnti;
        int cntj;
        for(cnti=0;cnti<m;cnti++){
	      for (cntj=0;cntj<n;cntj++){
          
            matrice[cnti][cntj] = 1;
            }
        }
        return matrice; 
}
double** Matrice::transposition(double** A, int m, int n){
	double** transpose = allocTab(n,m);
	int cnti;
    int cntj;
    for(cnti=0;cnti<n;cnti++){
	      for (cntj=0;cntj<m;cntj++){
            
            transpose[cnti][cntj] = A[cntj][cnti];
            }
        }
	return transpose;
}

double** Matrice::addition(double** A, double** B, int m, int n){
	double** added = allocTab(m,n);
  	int cnti;
    int cntj;
    for(cnti=0;cnti<m;cnti++){
	for (cntj=0;cntj<n;cntj++){
          
            added[cnti][cntj] = A[cnti][cntj] + B[cnti][cntj];
            }
        }
    return added;
}

double** Matrice::soustraction(double** A, double** B, int m, int n){
	double** deleted = allocTab(m,n);
  	int cnti;
    int cntj;
    for(cnti=0;cnti<m;cnti++){
	  for (cntj=0;cntj<n;cntj++){
          
            deleted[cnti][cntj] = A[cnti][cntj] - B[cnti][cntj];
            }
        }
    return deleted;
	
}
double** Matrice::produitHadamard(double** A, double** B, int m, int n){
	double** hadamard = allocTab(m,n);
  	int cnti;
    int cntj;
    for(cnti=0;cnti<m;cnti++){
	  for (cntj=0;cntj<n;cntj++){
          
            hadamard[cnti][cntj] = A[cnti][cntj] * B[cnti][cntj];
            }
        }
    return hadamard;
	
}

double** Matrice::produitMatriciel(double** A, double** B, int m, int n, int p, int q){
	double** produit = zeroMatrice(m,q);
    if(n != p)
	  printf("Le calcul n'est pas possible\n");
	else
	{
	  int cnti, cntj, cntk;
	  for(cnti=0;cnti<m;cnti++){
	   for(cntj=0;cntj<q;cntj++){
		   for(cntk=0;cntk<p;cntk++){
		    produit[cnti][cntj] += A[cnti][cntk] * B[cntk][cntj];   
	       }
	    }	
	  }
   }
   return produit;
}

