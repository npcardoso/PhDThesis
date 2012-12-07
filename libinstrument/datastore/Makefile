TREE_DIR := lib/datastore
LEVEL := ../..
CFLAGS := -fPIC
LFLAGS := -lpthread -shared


SOURCES := state.cpp \
  observation.cpp \
  threadinfo.cpp \
  transaction.cpp

OBJS := $(SOURCES:.cpp=.o)

include $(LEVEL)/Makefile.common

all: $(OBJS)
	echo $(OBJS)





