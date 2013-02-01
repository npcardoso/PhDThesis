#include "mpi.h"

#include "algorithms/mhs.h"
#include "heuristic/parallelization.h"
#include "heuristic/similarity.h"
#include "heuristic/sort.h"
#include "spectra/count_spectra.h"

#include <mpi.h>
#include <iostream>
#include <fstream>

void send_trie(const t_trie & trie,
              t_count chunk_size,
              int destination,
              int tag,
              MPI_Comm communicator) {

  t_trie::iterator it = trie.begin();

  t_count buffer_size = 0;
  t_component_id * buffer = new t_component_id[chunk_size];

  while(it != trie.end()) {
    t_candidate::iterator candidate_it = it->begin();
    while(candidate_it != it->end()){
      if(buffer_size >= chunk_size) {
        MPI_Send(buffer, buffer_size, MPI::UNSIGNED, 
                 destination, tag, communicator);
        buffer_size = 0;
      }
      buffer[buffer_size++] = *(candidate_it++);
    }
    
    if(buffer_size >= chunk_size) {
      MPI_Send(buffer, buffer_size, MPI::UNSIGNED, 
               destination, tag, communicator);
      buffer_size = 0;
    }
    buffer[buffer_size++] = 0;
    it++;
  }
  
  MPI_Send(buffer, buffer_size, MPI::UNSIGNED, 
           destination, tag, communicator);
  delete[]buffer;
}

void receive_trie(t_trie & trie,
                 t_count chunk_size,
                 int destination,
                 int tag,
                 MPI_Comm communicator) {
  
  t_component_id * buffer = new t_component_id[chunk_size];

  t_candidate candidate;
  int buffer_size = 0;
  do {
    MPI_Status status;

    MPI_Recv(buffer, chunk_size, MPI::UNSIGNED, 
             destination, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI::UNSIGNED, &buffer_size);
    for(t_id i = 0; i < buffer_size; i++)
      if(buffer[i])
        candidate.insert(buffer[i]);
      else {
        trie.add(candidate);
        candidate.clear();
      }

  } while(buffer_size == chunk_size);
  
  delete[]buffer;
}


int mpi_main(int argc, char **argv) {


  /* Initialize MPI */
  MPI_Init(&argc, &argv);
  
  /* Find out my identity in the default communicator */
  int ntasks, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  t_count_spectra count_spectra;
  
  t_heuristic<t_count> heuristic;
  heuristic.push(new t_filter_ochiai<t_count>());
//  heuristic.push(new t_filter_cutoff<t_count>());
  heuristic.push(new t_filter_sort<t_count>());

  t_mhs<t_count> mhs(heuristic);
  mhs.set_heuristic(3, heuristic);

  heuristic.push(new t_filter_divide<t_count>(rank, ntasks));
  mhs.set_heuristic(2, heuristic);
/*
  t_mhs<t_count> mhs(new t_parallel_similarity<t_count>(rank, ntasks));
  mhs.set_heuristic(2, new t_similarity<t_count>());
*/
  t_trie D;
  
  std::ifstream f;
  if(argc > 1)
    f.open(argv[1]);
  else
    f.open("in.mhsbig.txt");
  f >> count_spectra;

  if(rank == 0)  std::cout << count_spectra;

  t_time_interval time_begin = get_time_interval();
  mhs.calculate(count_spectra, D);
  
  t_time_interval time_end_calculate = get_time_interval();
  std::cerr << "Process " << rank << " Calculation Time: " << (time_end_calculate - time_begin) << std::endl;
  
  if(rank == 0) {
    for(t_count i = 1; i < ntasks; i++)
      receive_trie(D, 102400, i, 0, MPI_COMM_WORLD);
    D.print(std::cout);
  } else
      send_trie(D, 102400, 0, 0, MPI_COMM_WORLD);
  t_time_interval time_end_transfer = get_time_interval();
  std::cerr << "Process " << rank << " Transfer Time: " << (time_end_transfer - time_end_calculate) << std::endl;

  /* Shut down MPI */
  

  MPI_Finalize();
  return 0;
}


