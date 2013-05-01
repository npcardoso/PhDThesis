#include "goodness.h"

#include "../exceptions.h"

#include <boost/random/discrete_distribution.hpp>

void t_goodness_model::label(t_label_id lbl, t_density_model * model) {
  t_density_model_ptr tmp(model);
  label(lbl, tmp);
}

void t_goodness_model::label(t_label_id lbl, t_density_model_ptr model) {
  assert(lbl < densities.size());

}

void t_goodness_model::label(t_density_model * model, t_weight weight) {
  t_density_model_ptr tmp(model);
  label(tmp, weight);
}

void t_goodness_model::label(t_density_model_ptr model, t_weight weight) {
  weights.push_back(weight);
  densities.push_back(model);

  assert(densities.size() == weights.size());
}

const t_density_model & t_goodness_model::label(t_label_id lbl) const {
  assert(lbl < densities.size());
  return *densities[lbl];
}

t_count t_goodness_model::label_count() const {
  return densities.size();
}

const t_goodness_model::t_weights & t_goodness_model::weight() const {
  return weights;
}

t_weight t_goodness_model::weight(t_label_id lbl) const {
  assert(lbl < weights.size());
  return weights[lbl];
}

void t_goodness_model::weight(t_label_id lbl, t_weight weight) {
  assert(lbl < weights.size());
  weights[lbl] = weight;
}

double t_goodness_model::density(const t_observation & obs) const {
  double total = 0;
  for(t_label_id lbl = 0; lbl < label_count(); lbl++)
    total += density(lbl, obs);
  return total;
}

double t_goodness_model::density(t_label_id lbl, const t_observation & obs) const {
  assert(lbl < densities.size());
  return (*densities[lbl])(obs) * weights[lbl];
}

double t_goodness_model::probability(t_label_id lbl, const t_observation & obs) const {
  assert(lbl < densities.size());
  return density(lbl, obs) / density(obs);
}

t_label_id t_goodness_model::generate(const t_observation & obs) const {
  double probabilities[] = { 0.5, 0.1, 0.1, 0.1, 0.1, 0.1 };
  boost::random::discrete_distribution<> dist(probabilities);
  //  boost::random::discrete_distribution<> dist(&weight());
  throw e_not_implemented();
  return dist(gen);
}


