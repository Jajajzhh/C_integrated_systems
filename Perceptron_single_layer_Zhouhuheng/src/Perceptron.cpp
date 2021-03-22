//
//  Perceptron.cpp
//  msp
//
//  Created by HuHeng Zhou on 2021/1/6.
//  Copyright © 2021 HuHeng Zhou. All rights reserved.
//
#include <iostream>
#include "Perceptron.hpp"
#include "Matrice.h"
using namespace std;

Perceptron::Perceptron(int epoch, int wi, int wj){
    epochs = epoch;
    //Initialisation of epoch and weight
    Matrice mat = Matrice();
    weight = mat.newMat(wi , wj);
    weight_i = wi;
    weight_j = wj;
}
double** Perceptron::activation(double** X, int n){
    Matrice mat = Matrice();
    double** weight_t = mat.transposition(weight, weight_i, weight_j);
    double** Y = mat.produitMatriciel(weight_t, X, weight_j, weight_i, weight_i, n);//Output (j,n)
    return Y;
    
}
// weight jModification here
double** Perceptron::predict(double** X, int n){
    //X = singe(W(transpose) * X)
    X =  activation(X,n);
    for(int i=0; i<weight_j;i++){
        for(int j=0;j<n;j++){
            if(X[i][j]<0)
            X[i][j]=-1;
            else
            X[i][j]=1;
        }
    }
    return X;
}
double** Perceptron::getweight(){ return weight; }

void Perceptron::train(double** X, double** Z, int n){
    Matrice mat = Matrice();
    double** dw = 0;
     //Mettre a jour la matrice poids
    for(int i=0; i< epochs; i++){
        double** diff = mat.soustraction(Z, predict(X,n), weight_j, n);
        diff = mat.transposition(diff, weight_j, n);
        dw = mat.produitMatriciel(X, diff, weight_i, n, n, weight_j);
        dw = mat.produit(dw,weight_i,weight_j,learning_rate);
        weight = mat.addition(weight, dw, weight_i, weight_j);
    }
}
int Perceptron::translate(double **X){
    double** Z = predict(X,1);
    for(int i=0;i<10;i++){
        if(Z[i][0] > 0){
            return i;
        }
    }
    return 0;
}

