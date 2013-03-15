#include "component.h"

#include "../exceptions.h"

#include <boost/math/distributions/normal.hpp>
#include <boost/random/discrete_distribution.hpp>
  
t_density_constant::t_density_constant(t_density density): density(density) {
}

t_density t_density_constant::operator ()(const t_observation & obs) const {
  return density;
}

t_density t_kernel_uniform::operator ()(const t_observation & obs) const {
  if(obs * obs <= 1)
    return 0.5;
  return 0;
}

t_density t_kernel_gaussian::operator ()(const t_observation & obs) const {
  return(boost::math::pdf(boost::math::normal(), obs));
}

t_density t_density_kernel_single::operator ()(const t_observation & obs) const {
  t_density density = 0;
  
  for(const_iterator it = begin(); it != end(); it++)
    density += (*kernel)((obs - *it) / bandwidth);
  return  density / (bandwidth * size());
}

t_density_kernel_single::t_density_kernel_single(t_density_model * kernel, double bandwidth):kernel(kernel), bandwidth(bandwidth) {

}

t_density_kernel_single::t_density_kernel_single(t_density_kernel_single::t_density_model_ptr kernel, double bandwidth):kernel(kernel), bandwidth(bandwidth) {

}
  
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


t_component::t_component(t_goodness_model * g_model):g_model(g_model) {

}

t_component::t_component(boost::shared_ptr<t_goodness_model> g_model):g_model(g_model) {

}

const t_goodness_model & t_component::goodness_model() const {
  return *g_model;
}

void t_component::metadata(std::string key, std::string value) {
  mdata[key] = value; 
}

const char * t_component::metadata(std::string key) const {
  t_metadata::const_iterator pos = mdata.find(key);
  if(pos == mdata.end())
    return NULL;
  return pos->second.c_str();
}

