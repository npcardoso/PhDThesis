llvminstrument
==============

A c/c++ instrumentation tool

Running examples
==============

$ scons
$ ./run_test.sh fractal | xdot -  # shows the graphviz output of the captured data
$ ./run_test.sh webfs &           # runs an instrumented version of webfs in port 8090
$ echo -n "graphviz" | nc 127.0.0.1 12345 | xdot - # connects to library tcp/ip interface at port 12345 and requests graphviz output


