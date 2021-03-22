//
//  main.cpp
//  msp
//
//  Created by HuHeng Zhou on 2021/1/2.
//  Copyright © 2021 HuHeng Zhou. All rights reserved.
//

#include <iostream>
#include "Perceptron.hpp"
#include "Matrice.h"
#include "Multi_Perceptron.hpp"
int main(int argc, const char * argv[]) {
    // insert code here...
    Matrice mat = Matrice();
    Perceptron percep = Perceptron(100, 7, 10);
    double data_in[7][10] =
           {{1,0,1,1,0,1,0,1,1,1},
            {1,1,1,1,1,0,1,1,1,1},
            {1,1,0,1,1,1,1,1,1,1},
            {1,0,1,1,0,1,1,0,1,1},
            {1,0,1,0,0,0,1,0,1,0},
            {1,0,0,0,1,1,1,0,1,1},
            {0,0,1,1,1,1,1,0,1,1}};
    double Z_i[10][10] = {-1};
    double** X = mat.newMat(7, 10);
    double** Z = mat.newMat(10, 10);
    for(int i=0;i<7;i++){
        for(int j=0;j<10;j++){
            X[i][j] = data_in[i][j];
            cout<<X[i][j]<<" ";
        }
        cout<<"\n";
    }
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            Z[i][j] = -1;
            if(i==j)
                Z[i][i] = 1;
            cout<<Z[i][j]<<" ";
        }
        cout<<"\n";
    }
    //Entrainement par perceptron single layer
    percep.train(X, Z,10);
    double** weight = percep.getweight();
    for(int i=0;i<7;i++){
        for(int j=0;j<10;j++){
            
            cout<<weight[i][j]<<" ";
        }
        cout<<"\n";
    }
    //Verifier le resultat
    double** predict = percep.predict(X,10);
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cout<<predict[i][j]<<" ";
        }
        cout<<"\n";
    }
     Multi_Perceptron percep_mul = Multi_Perceptron(7, 10, 4);
    //*****************Main pour mlp*****************************
    /*
    double Z_i_mlp[4][10] =
    {{-1,-1,-1,-1,-1,-1 ,-1,-1, 1, 1},
     {-1,-1,-1,-1,1 ,1 , 1, 1,-1,-1},
     {-1,-1,1 ,1 ,-1,-1, 1, 1,-1,-1},
     {-1,1 ,-1,1 ,-1,1 ,-1, 1,-1, 1}};
    Z = mat.newMat(4, 10);
    for(int i=0;i<4;i++){
        for(int j=0;j<10;j++){
            Z[i][j] = Z_i_mlp[i][j];
            cout<<Z[i][j]<<" ";
        }
        cout<<"\n";
    }
    //Multi_Perceptron percep_mul = Multi_Perceptron(7, 10, 4);
    
    percep_mul.train(X, Z, 10, 0.3);
    double** predict = percep_mul.freeforward(X, 10);
    predict = percep_mul.activate(predict);
    for(int i=0;i<4;i++){
        for(int j=0;j<10;j++){
            if(predict[i][j] > 0)
            cout<<" "<<predict[i][j]<<" ";
            else
            cout<<predict[i][j]<<" ";
        }
        cout<<"\n";
    }
     */
    //**********************Fin Main mlp*********************
    //Test pour traduction afficheur 7-segment. On fait appel au function Perceptron::translate() ou Multi_Perceptron::translate()
    double** X_0 = percep_mul.createVector(X, 7, 10, 0);
    double** X_4 = percep_mul.createVector(X, 7, 10, 4);
    double** X_9 = percep_mul.createVector(X, 7, 10, 9);
    int test = percep.translate(X_4);
    //int test = percep_mul.translate(X_9);
    for(int k=0;k<7;k++){ cout<<X_4[k][0]; }
    cout<<"\nPour ce input on peut retrouver: " <<test<<"\n";

    return 0;
}
