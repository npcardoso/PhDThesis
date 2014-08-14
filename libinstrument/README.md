* LibInstrument

Instrumentation Stuff

* Guidelines
** Indentation
 - Use uncrustify to indent code:
   - cd .git/hooks
   - ln -s ../../configs/uncrustify-hook pre-commit
** Documentation
 - Use doxygen to document stuff:
   - doxygen configs/doxygen.conf
   - open doc/html/index.html
* Targets
** C/C++
*** Dependencies
 - llvm/clang (3.4.2)
 - boost (1.55)

** Java
*** Dependencies
 - javassist
 - maven
