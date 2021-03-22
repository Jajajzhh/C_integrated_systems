//
//  Multi_Perceptron.hpp
//  msp
//
//  Created by HuHeng Zhou on 2021/1/11.
//  Copyright © 2021 HuHeng Zhou. All rights reserved.
//

#ifndef Multi_Perceptron_hpp
#define Multi_Perceptron_hpp

#include <stdio.h>
class Multi_Perceptron{
private:
    //Les dimensions de poids
    int m_i,m_j,m_m,m_n, m_x;
    //Le poid de couche output
    double** weight_w;
    //Le poid de couche input
    double** weight_v;
    //Ys Zc Yc pour stocker ces valeurs temporaires
    double** Ys;
    double** Zc;
    double** Yc;
    //Le taux d'apprentissage
    double taux_apprentisage = 0.1;
public:
    //i le nombre de nodes input layer, j le nombre de nodes hidden layer, n le nombre de nodes output layer
    Multi_Perceptron(int i, int j, int n);
    double getTaux();
    //Depuis Matrice P on generer la vecotr Pn
    double** createVector(double** V, int i, int j, int n);
    //Pour chaque Xij, si Xij < 0 Xij = -1 sinon Xij = 1
    double** activate(double** X);
    //Calculer l'erreur entre Matrice Z et Za
    double getErreur(double** Z, double** Za, int i, int j);
    //Function d'activation tanh
    double** activation_tanh(double** X, int i, int j);
    //tanh' = 1 - tanh ^ 2
    double** tanh_prime(double** X, int i, int j);
    //Algoritheme freeforward
    double** freeforward(double** X, int x);
    //Mettre a jour les poids
    void backwards(double** P, double** Z, double** Za);
    //Algoritheme entrainement
    void train(double** P, double** Za, int x, double err_rate);
    double** getW();
    double** getV();
    int translate(double** X);
    
    
    
    
};

#endif /* Multi_Perceptron_hpp */
