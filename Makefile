all:
	scons -j 8 --random -k

doxygen:
	doxygen ./configs/doxygen.conf
