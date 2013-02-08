#include "mpi.h"



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


