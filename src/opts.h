#ifndef __OPTS_H__
#define __OPTS_H__

#include "algorithms/mhs.h"
#include "common/types.h"
#include "heuristic/heuristic.h"
#include "heuristic/similarity.h"
#include "heuristic/sort.h"

#include <getopt.h>

#include <cstdlib>
#include <iostream>
#include <fstream>

template <class T_ACTIVITY>
struct t_options {
  bool mpi;
  t_count mpi_level;
  t_count mpi_stride;

  char * input;
  char * output;

  t_mhs<T_ACTIVITY> mhs;
  
  static t_heuristic<T_ACTIVITY> default_heuristic() {
    t_heuristic<T_ACTIVITY> heuristic;
    heuristic.push(new t_filter_ochiai<t_count>());
    heuristic.push(new t_filter_sort<t_count>());
    return heuristic;
  }

  inline t_options():mhs(default_heuristic()){
    mpi = false;
    input = NULL;
    output = NULL;
    mpi_level = 0;
    mpi_stride = 0;
  }
};

template <class T_ACTIVITY>
std::ostream & operator<< (std::ostream & out, const t_options<T_ACTIVITY> & options) {
  out << "(MPI: " << options.mpi;
  out << " MPI_level: " << options.mpi_level;
  out << " MPI_stride: " << options.mpi_stride;
  out << " Cardinality: " << options.mhs.max_candidate_size;
  out << " Candidates: " << options.mhs.max_candidates;
  out << " Time: " << options.mhs.max_time << ")";
  return out;
}

template <class TYPE>
bool verb_strtof(char * buf, TYPE & dest, bool non_negative = false) {
  char * end_ptr;
  double tmp;
  tmp = strtod(buf, &end_ptr);
  if(end_ptr == buf || *end_ptr || (non_negative && tmp < 0)) {
    std::cerr << "Invalid "<< (non_negative?"non-negative ":"") << "float number: '" << buf << "'" << std::endl;
    return true;
  }
  dest = tmp;
  return false;
}

template <class TYPE>
bool verb_strtoi(char * buf, TYPE & dest, bool non_negative = false) {
  char * end_ptr;
  long tmp;
  tmp = strtol(buf, &end_ptr, 10);
  if(end_ptr == buf || *end_ptr || (non_negative && tmp < 0)) {
    std::cerr << "Invalid "<< (non_negative?"non-negative ":"") << "integer number: '" << buf << "'" << std::endl;
    return true;
  }
  dest = tmp;
  return false;
}
#define MPI_LEVEL 0
#define MPI_STRIDE 1

template <class T_ACTIVITY>
bool configure(int argc, char ** argv, t_options<T_ACTIVITY> & options){
  int long_option = 0;
  static struct option long_options[] = {
    {"mpi", 0, 0, 'm'},
    {"mpi-level", 1, &long_option, MPI_LEVEL},
    {"mpi-stride", 1, &long_option, MPI_STRIDE},
    {"time", 1, 0, 't'},
    {"candidates", 1, 0, 'd'},
    {"cardinality", 1, 0, 'c'},
    {"filter", 1, 0, 'f'},
    {"input", 1, 0, 'i'},
    {"output", 1, 0, 'o'},
    {NULL, 0, NULL, 0}
  };

  int c;
  int option_index = 0;
  bool errors = false;
  while ((c = getopt_long(argc, argv, "mt:c:s:d:f:i:o:",
                          long_options, &option_index)) != -1) {
    switch (c) {
    case 0:
      switch(long_option) {
        case MPI_LEVEL:
          errors |= verb_strtoi(optarg, options.mpi_level, true);
          break;
        case MPI_STRIDE:
          errors |= verb_strtoi(optarg, options.mpi_stride, true);
          break;
      }
    case 'm':
      options.mpi = true;
      break;
    case 't':
      errors |= verb_strtof(optarg, options.mhs.max_time, true);
      break;
    case 'd':
      errors |= verb_strtoi(optarg, options.mhs.max_candidates, true);
      break;
    case 'c':
      errors |= verb_strtoi(optarg, options.mhs.max_candidate_size, true);
      break;
    case 'i':
      options.input = optarg;
      break;
    case 'o':
      options.output = optarg;
      break;
    case 'f':
      printf ("Filter on: %s\n", optarg);
      break;
    default:
//      printf ("?? getopt returned character code 0%o ??\n", c);
      ;
    }
  }
  if (optind < argc) {
    printf ("non-option ARGV-elements: ");
    while (optind < argc)
      printf ("%s ", argv[optind++]);
    printf ("\n");
  }
  return errors;
}


#endif
