#include "../include/neuron.hpp"
#include "../include/activation_functions.hpp"

#include <cmath>
#include <iostream>
#define TOL 1e-6


int neuron:: unitTest1 () { // tests constuctors 
    int passed = 0;

    neuron n;
    neuron n1(5);
    neuron n2(n1);

    // default constructor
    if (n.size_X_ == 0 && n.po_ == 0. && n.biais_ == 0. && n.db_ == 0. 
        && n.pf_activation_ == nullptr && n.pf_activation_d_ == nullptr && n.Weight_ == nullptr && n.dWeight_ == nullptr)
        ++passed;    
    //passed = 1
    // constructor taking the size of X
    if (n1.size_X_ == 5 && n1.po_ == 0. && n1.biais_ == 0. && n1.db_ == 0. &&
        n1.pf_activation_ == nullptr && n1.pf_activation_d_ == nullptr && n1.Weight_ != nullptr && n1.dWeight_ != nullptr)
        ++passed;
    //passed = 2    
    int k = 0;
    for (int i=0; i<n1.size_X_; ++i) {
        if (n1.Weight_[i] == 0. && n1.dWeight_[i] == 0.)   
            ++k;
    }
    if (k % n1.size_X_ == 0)
        ++passed;    
    //passed = 3
    // copy constructor
    if (n2.size_X_ == n1.size_X_ && n2.po_ == n1.po_ && n2.biais_ == n1.biais_ &&
        n2.db_ == n1.db_ && n2.pf_activation_ == n1.pf_activation_ && n2.pf_activation_d_ == n1.pf_activation_d_ )
        ++passed;
    //passed = 4
    k = 0;
    for (int i=0; i<n1.size_X_; ++i) {
        if (n2.Weight_[i] == n1.Weight_[i] && n2.dWeight_[i] == n1.dWeight_[i])   
            ++k;
    }
    if (k % n2.size_X_ == 0)
        ++passed;
    //passed = 5

    return passed % 5; // return 0 if all successful
}

int neuron:: unitTest2 () { // tests getters setters
    int passed = 0;

    neuron n(10);
    n.setWeight(1.1, 9);
    n.setDWeight(2.2, 0);
    n.setBiais(3.3);
    n.setDb(4.4);
    double (*pf_s) (double) = &sigma;
    double (*pf_ds) (double) = &dSigma;
    n.setActivationFcts(pf_s, pf_ds);

    if (n.getWeight(9) == 1.1 && n.getDWeight(0) == 2.2 && n.getBiais() == 3.3 && n.getDb() == 4.4 && 
        n.getPo() == 0. && n.getSizeX() == 10)
        ++passed;
    //passed = 1;
    if (n.pf_activation_ == pf_s)
        ++passed;
    //passed = 2    
    if (n.pf_activation_(2.2) - pf_s(2.2) <= TOL)  // sigma(2.2) ~= 0.90024  
        ++passed;
    //passed = 3
    if (n.pf_activation_d_ == pf_ds)
        ++passed;
    //passed = 4
    if (n.pf_activation_d_(2.2) - pf_ds(2.2) <= TOL)  // dSigma(2.2) ~= 0.109459 
        ++passed;  
    //passed = 5

    return passed % 5;
}

int neuron:: unitTest3 () {
    int passed = 0;

    neuron ne(5);
    neuron ne1(6);
    neuron ne2(10);

    // test setWeightOnes
    ne.setWeightsOnes();
    int k = 0;
    for (int i=0; i<5; ++i) {
        if (ne.getWeight(i) == 1)
            ++k;
    }
    if (k % 5 == 0)
        ++passed;
    //passed = 1
    // test setWeightZeros
    ne.setDWeightsZeros();
    k = 0;
    for (int i=0; i<5; ++i) {
        if (ne.getDWeight(i) == 0)
            ++k;
    }
    if (k % 5 == 0)
        ++passed;
    //passed = 2
    // test operator =
    ne1.setWeightsOnes();
    ne1 = ne2;
    if (ne2.size_X_ == ne1.size_X_ && ne2.po_ == ne1.po_ && ne2.biais_ == ne1.biais_ &&
        ne2.db_ == ne1.db_ && ne2.pf_activation_ == ne1.pf_activation_ && ne2.pf_activation_d_ == ne1.pf_activation_d_ )
        ++passed;
    //passed = 3
    k = 0;
    for (int i=0; i<ne1.size_X_; ++i) {
        if (ne2.Weight_[i] == ne1.Weight_[i] && ne2.dWeight_[i] == ne1.dWeight_[i])   
            ++k;
    }
    if (k % ne2.size_X_ == 0)
        ++passed;
    //passed = 4
    // test proper memory allocation
    ne2.setWeight(4, 0); 
    if (ne1.getWeight(0) != ne2.getWeight(0))
        ++passed;
    //passed = 5
    // test setWeightrandom
    ne2.setWeightsRandom();
    ne = ne2;
    k = 0;
    for (int i=0; i<9; ++i) {
        if (ne2.getWeight(i) != ne2.getWeight(i+1))   
            ++k;
    }
    if (k % 9 == 0)
        ++passed;
    //passed = 6
    // test proper memory allocation
    ne2.setWeight(3, 0); 
    if (ne.getWeight(0) != ne2.getWeight(0))
        ++passed;
    //passed = 7
    // test activate
    neuron ne3(4);
    for (int i=0; i<4; ++i) {
        ne3.setWeight(i+1, i);
    }
    double X[4] = {.1, .2, .3, .4};
    double biais = .2;
    ne3.setBiais(biais);
    double (*pf_s) (double) = &sigma;
    double (*pf_ds) (double) = &dSigma;
    ne3.setActivationFcts(pf_s, pf_ds);
    ne3.activate(X);
    if (ne3.getPo() == sigma(3.2))  
        ++passed;
    //passed = 8
    // test operator ==
    neuron neu1(4);
    neuron neu2(4);
    neuron neu3;
    if ( (neu1==neu3) == false && (neu1==neu2) == true )
        ++passed;
    //passed = 9
    neu1.setWeightsOnes();
    neu2.setWeightsOnes();
    if ( (neu1==neu2) == true)
        ++passed;
    //passed = 10
    neu1.setBiais(1) ; neu2.setBiais(1);
    neu1.setActivationFcts(pf_s, pf_ds); neu2.setActivationFcts(pf_s, pf_ds);
    neu1.setWeight(4.1, 2); neu2.setWeight(4.1, 2);
    neu1.setDWeight(3.1, 2); neu2.setDWeight(3.1, 2);
    neu1.activate(X); neu2.activate(X);
    if ( (neu1==neu2) == true)
        ++passed;
    //passed = 11
    neu1.setWeight(5.2, 0);
    if ( (neu1==neu2) == false)
        ++passed;
    //passed = 12
    if ( (neu1!=neu2) == true )
        ++passed;
    //passed = 13
    if (ne3.evaluateFct(5) == sigma(5) && ne3.evaluateFctDerivative(1.1) == dSigma(1.1))
        ++passed;
    //passed = 14

    return passed % 14;
}


int main () {

    return (neuron::unitTest1() + neuron::unitTest2() + neuron::unitTest3());
}