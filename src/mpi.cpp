#include <mpi.h>
#include <iostream>


class t_mpi {
  bool ready;
  t_mpi();
public:
  ~t_mpi();
  static bool init(int argc, char **argv);
  static t_mpi mpi_environment;
};

t_mpi t_mpi::mpi_environment;

t_mpi::t_mpi(){
  std::cout << "MPI construct\n"; 
}

t_mpi::~t_mpi(){
  std::cout << "MPI destruct\n"; 
  if(ready)
    MPI_Finalize();
}

  /* Initialize MPI */

bool t_mpi::init(int argc,  char**argv){
  std::cout << "MPI init\n"; 
  if(!MPI_Init(&argc, &argv))
    mpi_environment.ready = true;
  return mpi_environment.ready;
}
