#include "lib.h"

#include "io/spectra.h"
#include "io/graphviz.h"
#include "interface/pthread.h"
#include "utils/debug.h"

#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <algorithm>
#include <sstream>
using namespace std;

void * networkInterface(void * ptr);

/*****************************************************\
 * TODO:                                             *
 * - Remove datastore locks and relocate them to the *
 *   DataStore class itself                          *
\*****************************************************/

pthread_mutex_t datastore_lock;
DataStore * datastore = NULL;

time_interval_t getTimeInterval(){
  timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec * 10e6 + time.tv_usec;
}


void init() __attribute__((constructor));
void init() {
  pthread_t t;

  debug("Instrumentation Init");

  pthread_mutex_init(&datastore_lock, NULL);
  datastore = new DataStore(getTimeInterval(), pthread_self(), 102400);
  _instr_pthread_register_main(pthread_self());

  pthread_create(&t, NULL, networkInterface, NULL);
}

void finish() __attribute__((destructor));
void finish() {
  Graphviz(cout, *getDataStore());
  delete datastore;
  releaseDataStore();
  pthread_mutex_destroy(&datastore_lock);
  debug("Instrumentation Finalize");
}

DataStore * getDataStore(){
  pthread_mutex_lock(&datastore_lock); //Lock: datastore_lock
  return datastore;
}

void releaseDataStore(){
  pthread_mutex_unlock(&datastore_lock); //Unlock: datastore_lock
}

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


class Server {
  int socket_fd;
  public:
    Server(){
      socket_fd = -1;
    }

    ~Server(){
      if(socket_fd >= 0)
        close(socket_fd);
    }

    bool Bind(int portno) {
      struct sockaddr_in serv_addr;

      if(socket_fd >= 0)
        close(socket_fd);

      socket_fd = socket(AF_INET, SOCK_STREAM, 0);
      if(socket_fd < 0)
        return false;
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = INADDR_ANY;
      serv_addr.sin_port = htons(portno);
      if (bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        return false;
      listen(socket_fd,5);
      return true;
    }

    int getIncomingConnection(){
      struct sockaddr_in cli_addr;
      if(socket_fd < 0)
        return -1;
      socklen_t clilen;
      clilen = sizeof(cli_addr);
      int newsock_fd = accept(socket_fd, (struct sockaddr *) &cli_addr, &clilen);
      if (newsock_fd < 0) return -1;
      return newsock_fd;
    }

    char * readString(int in_socket_fd, size_t max_size, bool strip){
      int n;
      char * buffer = new char[max_size + 1];

      bzero(buffer,max_size + 1);

      n = read(in_socket_fd, buffer, max_size);
      if (n < 0) {
        debug("ERROR reading from socket");
        delete buffer;
        return NULL;
      }

      return buffer;
    }
};




void * networkInterface(void * ptr) {
  pthread_detach(pthread_self());

  Server ni;
  const char * port_str = getenv("INSTR_TCP_PORT");
  int port;
  if(port_str)
    port = atoi(port_str);
  else {
    debug("INSTR_TCP_PORT not defined; not starting TCP/IP interface");
    return NULL;
  }
  if(!ni.Bind(port)) {
    debug("Could not bind to port %d", port);
    return NULL;
  }
  debug("Listening at port %d", port);

  int incoming_socket_fd;
  while((incoming_socket_fd = ni.getIncomingConnection()) >= 0) {
    debug("Got connection on port %d", port);
    stringstream out;

    if(char * in = ni.readString(incoming_socket_fd, 30, true)){
      if(strcasecmp(in, "graphviz") == 0){
        Graphviz(out, *getDataStore());
        releaseDataStore();
      }
      else if(strcasecmp(in, "state") == 0){
        StateSpectra(out, *getDataStore());
        releaseDataStore();
      }
      delete in;
    }


    if (write(incoming_socket_fd, out.str().c_str(), out.str().size()) < 0)
      debug("ERROR writing to socket");
    close(incoming_socket_fd);
    debug("Closed connection on port %d", port);
  }
  return 0;
}
