#libdiag

A diagnosis library.

Implements:

 - Count Spectra
 - MHS2
 - Barinel
 - Fuzzinel (confidence/fuzzy error)
 - Ochiai
 - Benchmark framework

Tools:

 - libRdiag: R binding
 - llvmInstrument: An instrumentation tool
 - libinstrument: An instrumentation colection framework


##Building

 - Install dependencies: ./bootstrap.sh
 - Install libRdiag: scons install_libRdiag

##Using Sandboxes
 - cd src/sandox
 - ./make_sandbox.sh <template> <sandbox_name>

## TODO
 - Check if barinel version from commit 6f4f2235259e63b99515bd3d66612a8a429c4071 (barinel_conservative) is worth the extra effort
TODO:
 - NFGE
