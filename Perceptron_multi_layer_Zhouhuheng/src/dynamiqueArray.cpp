#include <stdio.h>
#include <iostream>
using namespace std;
#include "dynamiqueArray.h"
// Allocation pour le premier temps i====Row j====colomn
double** allocTab (int i, int j){

  double **ptr; 
  int count;
  ptr = new double*[i];
  for(count=0;count<i;count++){
	  ptr[count] = new double[j];
	  }
  if (ptr == NULL)
    cout<<"erreur allocation\n";
  //else
    //cout<<"Creation success\n";
  return ptr;

}


void giveValue (int i, int j, double** ptr){
  int cnti;
  int cntj;
  for(cnti=0;cnti<i;cnti++){
	  for (cntj=0;cntj<j;cntj++){
         double tmp;
         cout<<"Enter a number: ";
         cin>>tmp;
         ptr[cnti][cntj] = tmp;
      }
  } 
}

void printValue (int i, int j, double** ptr){

  int cnti,cntj;
  for(cnti=0;cnti<i;cnti++){
	  printf("\n");
	  for (cntj=0;cntj<j;cntj++){
          printf("%lf ", ptr[cnti][cntj]);
      }
  } 
}
// Deallocation du tableau
void freeTableau (double** ptr, int i){

  int cnti;
  for (cnti=0;cnti<i;++cnti){
     delete[] ptr[cnti];
   }
  delete[] ptr;
}
