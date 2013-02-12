#include "mpi.h"

#include "../../common/types.h"

#include <mpi.h>

#include <list>
#include <vector>

void send_candidates(const t_trie & trie,
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

template <class T>
class t_candidate_pool {
public:
  typedef std::list <T> t_inner_storage;
  typedef std::vector<t_inner_storage> t_storage; 
  t_storage storage;

  void add(const t_trie & trie){
    t_trie::iterator it = trie.begin();
    while(it != trie.end())
      add(*(it++));
  }

  void add(const T & candidate){
    if(storage.size() < candidate.size())
      storage.resize(candidate.size());
    storage[candidate.size() - 1].push_back(candidate);
  }

  void trie(t_trie & trie) {
    typename t_storage::iterator it = storage.begin();
    while(it != storage.end()) {
      typename t_inner_storage::iterator in_it = it->begin();
      while(in_it != it->end())
        trie.add(*(in_it++), false, true);
      it++;
    }
  }

};

template <class T>
void receive_candidates(t_candidate_pool<T> & candidate_pool,
                        t_count chunk_size,
                        int source,
                        int tag,
                        MPI_Comm communicator) {

  t_component_id * buffer = new t_component_id[chunk_size];

  T candidate;
  int buffer_size = 0;
  do {
    MPI_Status status;

    MPI_Recv(buffer, chunk_size, MPI::UNSIGNED, 
             source, tag, MPI_COMM_WORLD, &status);
    source = status.MPI_SOURCE;
    MPI_Get_count(&status, MPI::UNSIGNED, &buffer_size);
    for(t_id i = 0; i < (t_id)buffer_size; i++)
      if(buffer[i])
        candidate.insert(candidate.end(), buffer[i]);
      else {
        candidate_pool.add(candidate);
        candidate.clear();
      }

  } while((t_count) buffer_size == chunk_size);

  delete[]buffer;
}

inline bool is_sender(int rank, int turn) {
  return ((1ul << (turn - 1)) + rank) % (1ul << turn) == 0;
}

inline int get_sender(int rank, int turn) {
  return rank + (1ul << (turn - 1));
}

inline int get_receiver(int rank, int turn) {
  return rank - (1ul << (turn - 1));
}


void mpi_reduce_trie(t_trie & trie) {
  int ntasks, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  typedef t_candidate_pool <t_candidate> t_candidates;

  t_candidates candidate_pool;
  candidate_pool.add(trie);
  
  t_count i = 1;
  while(i < sqrt(ntasks) + 1 && !is_sender(rank, i)) {
    if(get_sender(rank, i) < ntasks)
      receive_candidates(candidate_pool, 10, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD);
    i++;
  }

  trie.clear();
  candidate_pool.trie(trie);
  if(rank)
    send_candidates(trie, 10, get_receiver(rank, i), 0, MPI_COMM_WORLD);
}
