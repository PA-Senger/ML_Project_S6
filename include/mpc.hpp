#ifndef MPC_HPP
#define MPC_HPP
#include "../include/activation_functions.hpp"
#include "feed_forward.hpp"
#include <cassert>

template <int n_in, int n_out, int n_layer>
class mpc : public FeedForward<n_in, n_out, n_layer> {
public:
  // * Constructors && Destructors using parent's one
  mpc() : FeedForward<n_in, n_out, n_layer>(){};
  mpc(const mpc &m) : FeedForward<n_in, n_out, n_layer>(m){};
  ~mpc(){}; // nothing new to be deleted

  // * Methods
  void setAllFct(double (*pf_a)(double), double (*pf_da)(double),
                 std::string name = "n/a");
  void setFct(double (*pf_a)(double), double (*pf_da)(double), int i_layer,
              std::string name = "n/a");
  // the build  method is already properly defined in FeedForward, no need to
  // overload it, non pure virtual methods must be first define in parent class

  // * Tests
  static void unitTest();

private:
};

// ! Definitions

template <int n_in, int n_out, int n_layer>
void mpc<n_in, n_out, n_layer>::setAllFct(double (*pf_a)(double),
                                          double (*pf_da)(double),
                                          std::string name) {
  for (int i = 0; i < n_layer + 1; ++i) {
    for (int j = 0; j < this->L_[i].getNbNeurons(); ++j) {
      this->L_[i].setActivationFcts(pf_a, pf_da, j, name);
    }
  }
}

template <int n_in, int n_out, int n_layer>
void mpc<n_in, n_out, n_layer>::setFct(double (*pf_a)(double),
                                       double (*pf_da)(double), int i_layer,
                                       std::string name) {
  for (int j = 0; j < this->L_[i_layer].getNbNeurons(); ++j) {
    this->L_[i_layer].setActivationFcts(pf_a, pf_da, j, name);
  }
}

// ! Tests

template <int n_in, int n_out, int n_layer>
void mpc<n_in, n_out, n_layer>::unitTest() {

  mpc<n_in, n_out, n_layer> m;

  // Test default constructor
  assert(m.nb_total_weights_ == m.getTotalWeights());
  assert(m.L_[0].getNbData() == n_in);
  assert(m.L_[n_layer].getNbNeurons() == n_out);

  for (int i = 1; i < n_layer + 1; ++i) {
    assert(m.L_[i].getNbData() == m.L_[i - 1].getNbNeurons());
  }

  // test copy construtor
  mpc<n_in, n_out, n_layer> m2(m);

  assert(m.nb_total_weights_ == m2.getTotalWeights());
  assert(m.nb_total_weights_ == m2.nb_total_weights_);

  for (int i = 0; i < n_layer + 1; ++i) {
    assert(m.L_[i].getNbData() == m2.L_[i].getNbData());
    assert(m.L_[i].getNbNeurons() == m2.L_[i].getNbNeurons());

    for (int j = 0; j < m.L_[i].getNbNeurons(); ++j) {
      assert(m.L_[i](j).getSizeX() == m2.L_[i](j).getSizeX());
    }
  }

  // Test set activation fcts
  double (*pf_a)(double) = &sigma;
  double (*pf_da)(double) = &dSigma;

  m.setAllFct(pf_a, pf_da, "sigmoid");
  for (int i = 0; i < n_layer + 1; ++i) {
    for (int j = 0; j < m.L_[i].getNbNeurons(); j++) {
      assert(m(i)(j).evaluateFct(2.2) == pf_a(2.2));
      assert(m(i)(j).evaluateFctDerivative(2.2) == pf_da(2.2));
    }
  }

  double (*pf_h)(double) = &inv;

  m.setFct(pf_h, pf_h, 0);
  for (int j = 0; j < m.L_[0].getNbNeurons(); j++) {
    assert(m(0)(j).evaluateFct(2.2) == -2.2);
    assert(m(0)(j).evaluateFctDerivative(-3) == 3);
  }
}

#endif