#include "mpi.h"

#include "types.h"
#include "heuristics/parallelization.h"
#include "utils/time.h"


#include <mpi.h>

#include <cmath>

#include <list>
#include <vector>

using namespace std;
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::structs;
using namespace diagnosis::heuristics;

t_count send_candidates (const t_trie & trie,
                         t_count chunk_size,
                         int destination,
                         int tag,
                         MPI_Comm communicator) {
    t_trie::iterator it = trie.begin();

    t_count buffer_size = 0;
    t_component_id * buffer = new t_component_id[chunk_size];


    while (it != trie.end()) {
        t_candidate::iterator candidate_it = it->begin();

        while (candidate_it != it->end()) {
            if (buffer_size >= chunk_size) {
                MPI_Send(buffer, buffer_size, MPI::UNSIGNED,
                         destination, tag, communicator);
                buffer_size = 0;
            }

            buffer[buffer_size++] = *(candidate_it++);
        }

        if (buffer_size >= chunk_size) {
            MPI_Send(buffer, buffer_size, MPI::UNSIGNED,
                     destination, tag, communicator);
            buffer_size = 0;
        }

        buffer[buffer_size++] = 0;
        it++;
    }

    MPI_Send(buffer, buffer_size, MPI::UNSIGNED,
             destination, tag, communicator);
    delete[] buffer;
    return trie.size();
}

template <class T>
class t_candidate_pool {
    size_t sze;
public:
    typedef std::list<T> t_inner_storage;
    typedef std::vector<t_inner_storage> t_storage;
    t_storage storage;

    inline size_t size () const {
        return sze;
    }

    void add (const t_trie & trie) {
        t_trie::iterator it = trie.begin();


        while (it != trie.end())
            add(*(it++));
    }

    void add (const T & candidate) {
        if (storage.size() < candidate.size())
            storage.resize(candidate.size());

        storage[candidate.size() - 1].push_back(candidate);
        sze++;
    }

    void trie (t_trie & trie) {
        typename t_storage::iterator it = storage.begin();

        while (it != storage.end()) {
            typename t_inner_storage::iterator in_it = it->begin();

            while (in_it != it->end())
                trie.add(*(in_it++), false, true);

            it++;
        }
    }
};

template <class T>
t_count receive_candidates (t_candidate_pool<T> & candidate_pool,
                            t_count chunk_size,
                            int source,
                            int tag,
                            MPI_Comm communicator) {
    t_component_id * buffer = new t_component_id[chunk_size];

    t_count old_size = candidate_pool.size();
    T candidate;
    int buffer_size = 0;


    do {
        MPI_Status status;

        MPI_Recv(buffer, chunk_size, MPI::UNSIGNED,
                 source, tag, MPI_COMM_WORLD, &status);
        source = status.MPI_SOURCE;
        MPI_Get_count(&status, MPI::UNSIGNED, &buffer_size);

        for (t_id i = 0; i < (t_id) buffer_size; i++)
            if (buffer[i])
                candidate.insert(candidate.end(), buffer[i]);
            else {
                candidate_pool.add(candidate);
                candidate.clear();
            }
    } while ((t_count) buffer_size == chunk_size);

    delete[] buffer;
    return candidate_pool.size() - old_size;
}

inline bool is_sender (int rank, int turn) {
    return ((1ul << (turn - 1)) + rank) % (1ul << turn) == 0;
}

inline int get_sender (int rank, int turn) {
    return rank + (1ul << (turn - 1));
}

inline int get_receiver (int rank, int turn) {
    return rank - (1ul << (turn - 1));
}

void mhs2_reduce_normal (t_trie & trie,
                         size_t buffer_size, t_stats & stats) {
    int ntasks, rank;
    t_time_interval time_begin = time_interval();


    typedef t_candidate_pool<t_candidate> t_candidates;
    t_candidates candidate_pool;


    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank) {
        stats.items_sent += send_candidates(trie, buffer_size, 0, 0, MPI_COMM_WORLD);

        stats.total_comm += time_interval() - time_begin;
        time_begin = time_interval();
    }
    else {
        for (t_count i = 1; i < ntasks; i++)
            stats.items_recv += receive_candidates(candidate_pool, buffer_size, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD);

        stats.total_comm += time_interval() - time_begin;
        time_begin = time_interval();

        candidate_pool.add(trie);
        trie.clear();
        candidate_pool.trie(trie);

        stats.total_merge += time_interval() - time_begin;
        time_begin = time_interval();
    }
}

void mhs2_reduce_hierarchical (t_trie & trie,
                               size_t buffer_size, t_stats & stats) {
    int ntasks, rank;
    t_time_interval time_begin = time_interval();


    typedef t_candidate_pool<t_candidate> t_candidates;
    t_candidates candidate_pool;

    t_count i = 1;


    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    while (i < sqrt(ntasks) + 1 && !is_sender(rank, i)) {
        if (get_sender(rank, i) < ntasks)
            stats.items_recv += receive_candidates(candidate_pool, buffer_size, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD);

        i++;
    }

    stats.total_comm += time_interval() - time_begin;
    time_begin = time_interval();

    if (candidate_pool.size()) {
        candidate_pool.add(trie);
        trie.clear();
        candidate_pool.trie(trie);
    }

    stats.total_merge += time_interval() - time_begin;
    time_begin = time_interval();

    if (rank)
        stats.items_sent += send_candidates(trie, buffer_size, get_receiver(rank, i), 0, MPI_COMM_WORLD);

    stats.total_comm += time_interval() - time_begin;
    time_begin = time_interval();
}

void mhs2_reduce (t_trie & trie,
                  bool hierarchical,
                  size_t buffer_size, t_stats & stats) {
    t_time_interval time_begin = time_interval();


    if (hierarchical)
        mhs2_reduce_hierarchical(trie, buffer_size, stats);
    else
        mhs2_reduce_normal(trie, buffer_size, stats);

    stats.total_transfer = (time_interval() - time_begin);
}

void mhs2_heuristic_setup (t_mhs & mhs,
                           t_count mpi_level,
                           t_count mpi_stride) {
    int ntasks, rank;


    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (!mpi_level)
        mpi_level = 1;

    mhs.set_heuristic(mpi_level + 1, mhs.get_heuristic_ptr(mpi_level));

    t_ptr<t_heuristic> heuristic(new t_heuristic(mhs.get_heuristic(mpi_level)));

    if (mpi_stride)
        heuristic->push(new heuristics::t_divide(rank, ntasks, mpi_stride));
    else {
        int seed = time(NULL);
        MPI_Bcast(&seed, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
        heuristic->push(new heuristics::t_random_divide(rank, ntasks, seed));
    }

    mhs.set_heuristic(mpi_level, heuristic);
}

void mhs2_map (const t_mhs & mhs,
               const t_spectra & spectra,
               t_trie & D,
               t_stats & stats,
               const t_spectra_filter * filter) {
    int ntasks, rank;
    t_time_interval time = time_interval();


    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    mhs(spectra, D, filter);

    stats.items_generated = D.size();
    stats.total_calc = (time_interval() - time);
    time = time_interval();
}

void mhs2_collect_stats (ostream & out,
                         const t_trie & D,
                         t_stats & stats) {
    int ntasks, rank;


    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        MPI_Status status;

        for (t_count i = ntasks; --i;) {
            t_stats tmp_stats;
            MPI_Recv(&tmp_stats, sizeof(t_stats), MPI::BYTE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            tmp_stats.print(out, i, false);
        }

        stats.print(out, 0, true);
        out << "Candidates: " << D.size() << std::endl;
    }
    else {
        MPI_Send(&stats, sizeof(t_stats), MPI::BYTE, 0, 1, MPI_COMM_WORLD);
    }
}