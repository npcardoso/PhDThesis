#libdiag

A diagnosis library.

Implements:

 - Count Spectra
 - MHS2
 - Fuzzinel
 - Ochiai

Tools:

 - libRdiag: R binding
 - llvmInstrument: An instrumentation tool
 - libinstrument: An instrumentation colection framework

TODO:
 - NFGE

##Building

 - Install dependencies: ./bootstrap.sh
 - Install libRdiag: scons install_libRdiag

##Using Sandboxes
 - cd src/sandox
 - ./make_sandbox.sh <template> <sandbox_name>
