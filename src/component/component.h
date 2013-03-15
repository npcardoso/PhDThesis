#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "../types.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

typedef double t_observation;

class t_density_model {
public:
  virtual t_density operator ()(const t_observation & obs) const = 0;
};

class t_kernel_uniform : public t_density_model {
public:
  virtual t_density operator ()(const t_observation & obs) const;
};

class t_kernel_gaussian : public t_density_model {
public:
  virtual t_density operator ()(const t_observation & obs) const;
};

class t_density_constant : public t_density_model {
private:
  t_density density;
public:
  t_density_constant(t_density density);

  virtual t_density operator ()(const t_observation & obs) const;
};

class t_density_kernel_single : public t_density_model, public std::vector<t_observation> {
private:
  typedef boost::shared_ptr<t_density_model> t_density_model_ptr;
  t_density_model_ptr kernel;
  
  double bandwidth;
public:
  
  t_density_kernel_single(t_density_model * kernel, double bandwidth);
  t_density_kernel_single(t_density_model_ptr kernel, double bandwidth);
  
  virtual t_density operator ()(const t_observation & obs) const;
};

class t_goodness_model {
private:
  static boost::mt19937 gen;
  
  typedef boost::shared_ptr<t_density_model> t_density_model_ptr;
  typedef std::vector <t_density_model_ptr> t_densities;
  t_densities densities;

  typedef std::vector <t_weight> t_weights;
  t_weights weights;

public:
  virtual void label(t_label_id lbl, t_density_model * model);
  virtual void label(t_label_id lbl, t_density_model_ptr model);
  virtual void label(t_density_model * model, t_weight weight);
  virtual void label(t_density_model_ptr model, t_weight weight);
  virtual const t_density_model & label(t_label_id lbl) const;
  virtual t_count label_count() const;
  
  virtual const t_weights & weight() const;
  virtual t_weight weight(t_label_id label) const;
  virtual void weight(t_label_id label, t_weight weight);

  virtual double density(const t_observation & obs) const;
  virtual double density(t_label_id label, const t_observation & obs) const;

  virtual double probability(t_label_id label, const t_observation & obs) const;

  t_label_id generate(const t_observation & obs) const;
};



class t_component {
private:
  boost::shared_ptr <t_goodness_model> g_model;
  
  typedef std::map <std::string, std::string> t_metadata;
  t_metadata mdata;

public:
  t_component(t_goodness_model * g_model);
  t_component(boost::shared_ptr<t_goodness_model> g_model);

  const t_goodness_model & goodness_model() const;

  const char * metadata(std::string key) const;
  void metadata(std::string key, std::string value);
};
#endif
