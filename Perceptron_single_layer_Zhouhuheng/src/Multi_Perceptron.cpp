//
//  Multi_Perceptron.cpp
//  msp
//
//  Created by HuHeng Zhou on 2021/1/11.
//  Copyright © 2021 HuHeng Zhou. All rights reserved.
//

#include "Multi_Perceptron.hpp"
#include "Matrice.h"
#include <math.h>
#include <iostream>
using namespace std;
Multi_Perceptron::Multi_Perceptron(int i, int j, int n){
    Matrice mat = Matrice();
    m_i = i;
    m_j = j;
    m_m = m_j;
    m_n = n;
    weight_v = mat.randMatrice(m_i, m_j);
    weight_w = mat.randMatrice(m_m, m_n);
    
}
double** Multi_Perceptron::createVector(double **V, int i, int j, int n){
    Matrice mat = Matrice();
    double** vec = mat.newMat(i, 1);
    for(int cnti = 0; cnti<i; cnti++){
        vec[cnti][0] = V[cnti][n];
    }
    return vec;
}
double Multi_Perceptron::getTaux(){return taux_apprentisage;}
double** Multi_Perceptron::activation_tanh(double** X, int i, int j){
    for(int cnti = 0; cnti<i; cnti++){
        for(int cntj = 0; cntj<j; cntj++){
            X[cnti][cntj] = tanh( X[cnti][cntj] );
            //Activation added
            
        }
    }
    return X;
}
double Multi_Perceptron::getErreur(double **Z, double **Za, int i, int j){
    double error = 0;
    double sum = 0;
    for(int cntj = 0; cntj<10; cntj++){
        sum = 0;
        for(int cnti = 0; cnti<4; cnti++){
            //double val = 1;
            double diff = (Z[cnti][cntj] - Za[cnti][cntj]);
            sum = sum + pow(diff, 2);
        }
        sum = sqrt(sum);
        sum = sum / 10;
        cout<<" "<<sum;
        error += sum;
    }
    
    return error;
}
double** Multi_Perceptron::activate(double **X){
    for(int i=0;i<4;i++){
        for(int j=0;j<10;j++){
            if(X[i][j] > 0)
            X[i][j]=1;
            else
            X[i][j]=-1;
        }
    }
    return X;
}
double** Multi_Perceptron::tanh_prime(double **X, int i, int j){
    for(int cnti = 0; cnti<i; cnti++){
        for(int cntj = 0; cntj<j; cntj++){
            //tanh' = 1 - tanh^2
            X[cnti][cntj] = 1 - pow(tanh( X[cnti][cntj] ), 2) ;
        }
    }
    return X;

}
//X = (i,x) Output Z = (n,x)
double** Multi_Perceptron::freeforward(double** X, int x){
    //N'en ajout pas le 1 input nueron a input layer et hidden layer
    Matrice mat = Matrice();
    m_x = x;
    double** v_transpose = mat.transposition(weight_v, m_i, m_j);
    double** Yint = mat.produitMatriciel(v_transpose, X, m_j, m_i, m_i, x);
    double** Zint = activation_tanh(Yint, m_j, x);
    double** w_transpose = mat.transposition(weight_w, m_m, m_n);
    Ys = mat.produitMatriciel(w_transpose, Zint, m_n, m_m, m_j, x);
    double** Z = activation_tanh(Ys, m_n, x);
    Zc = Zint;
    Yc = Yint;
    //Ys = Ys;
    return Z;
}

void Multi_Perceptron::backwards(double** P, double **Z, double** Za){
    Matrice mat = Matrice();
    double** phis = mat.soustraction(Za, Z, m_n, m_x);
    phis = mat.produitHadamard(phis, tanh_prime(Ys, m_n, m_x), m_n, m_x);
    double** phic = mat.produitMatriciel(weight_w, phis, m_m, m_n, m_n, m_x);
    //On a toujour m_m = m_j
    phic = mat.produitHadamard(phic, tanh_prime(Yc, m_j, m_x), m_m, m_x);
    double** phis_trans = mat.transposition(phis, m_n, m_x);
    double** phic_trans = mat.transposition(phic, m_m, m_x);
    double** err_w = mat.produitMatriciel(Zc, phis_trans, m_m, m_x, m_x, m_n);//m,n
    double** err_v = mat.produitMatriciel(P, phic_trans, m_i, m_x, m_x, m_j);//i,j
    err_w = mat.produit(err_w, m_m, m_n, getTaux());
    err_v = mat.produit(err_v, m_i, m_j, getTaux());
    weight_v = mat.addition(weight_v, err_v, m_i, m_j);
    weight_w = mat.addition(weight_w, err_w, m_m, m_n);
}

void Multi_Perceptron::train(double** P,double** Za, int x, double err_rate){
    double** Z = freeforward(P, x);
    double error = getErreur(Z, Za, m_n, m_x);
    int test = 0;
    while (error > err_rate) {
        for(int cnt = 0; cnt<x;cnt++){
            //Create each vector V(i,1)
            double** vec = createVector(P, 7, 10, cnt);
            //Freeforward
            Z = freeforward(vec, 1);
             //Backwards
            double** vecZa = createVector(Za, 4, 10, cnt);
            backwards(vec, Z, vecZa);
        }
        Z = freeforward(P, x);
        for(int i=0;i<4;i++){
            for(int j=0;j<10;j++){
                cout<<Z[i][j]<<" ";
            }
            cout<<"\n";
        }
        error = getErreur(Z, Za, m_n, m_x);
        cout<< "\n Error: " << error<< "\n";
        test++;
    }
}
int Multi_Perceptron::translate(double **X){
    double** Z = freeforward(X, 1);
    int out = 0;
    for(int i=0;i<4;i++){
        if(Z[i][0]>0){
            out = out + pow(2,(3-i));
        }
    }
    return out;
}

