#ifndef __GOODNESS_H__
#define __GOODNESS_H__

#include "density.h"

#include <boost/random/mersenne_twister.hpp>


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

#endif
