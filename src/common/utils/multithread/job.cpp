#include "job.h"
/*
 * class t_job {
 * public:
 *  void operator () ();
 *  virtual std::string get_type () const = 0;
 *  virtual bool operator < (const t_job & job) const = 0;
 *
 *  t_time_interval get_init_time() const;
 *  t_time_interval get_start_time() const;
 *  t_time_interval get_end_time() const;
 *
 *  bool started () const;
 *  bool finished () const;
 *
 *
 *  inline virtual ~t_job () {};
 * protected:
 *  void execute () const = 0;
 * };
 *
 * t_job::t_job () {
 *  reset();
 * }
 *
 * void t_job::reset() {
 *  init = time_interval();
 *  start = NAN;
 *  end = NAN;
 * }
 *
 * void t_queued_job::operator() () {
 *  assert(isnan(start));
 *  assert(isnan(end));
 *
 *  start = time_interval();
 *  execute();
 *  end = time_interval();
 * }
 *
 * t_time_interval t_job::get_init_time() const {
 *  return init;
 * }
 *
 * t_time_interval t_job::get_start_time() const {
 *  return start;
 * }
 *
 * t_time_interval t_job::get_end_time() const {
 *  return end;
 * }
 *
 * bool t_job::started () const {
 *  return !isnan(start);
 * }
 *
 * bool t_job::finished () const {
 *  return !isnan(end);
 * }
 */