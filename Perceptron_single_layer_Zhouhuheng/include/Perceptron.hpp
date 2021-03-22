//
//  Perceptron.hpp
//  msp
//
//  Created by HuHeng Zhou on 2021/1/6.
//  Copyright © 2021 HuHeng Zhou. All rights reserved.
//

#ifndef Perceptron_hpp
#define Perceptron_hpp

#include <stdio.h>

class Perceptron{
    public:
    //creation de matrice de poids de wi * wj.
    //epochs le nombre de fois que l'on loop l'algorithme d'apprentissage

        Perceptron(int epochs, int wi, int wj);
        double** activation(double** data_in, int n);
        //Calculer Wt * P puis passer par activation signe
        double** predict(double** data_in, int n);
        void train(double** X, double** Y, int n);
        double** getweight();
        int translate(double** X);
    private:
        int weight_i, weight_j;
        double** weight;
        int epochs;
        double learning_rate = 0.1;
};
#endif /* Perceptron_hpp */
