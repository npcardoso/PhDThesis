# libcrowbar

## Guidelines
### Indentation
 - Use uncrustify to indent code:
   - cd .git/hooks
   - ln -s ../../configs/uncrustify-hook pre-commit
### Documentation
 - Use doxygen to document stuff:
   - doxygen configs/doxygen.conf
   - open doc/html/index.html

## diagnostic-native
### Dependencies
 - boost (1.55)
 - libgmp
 - libmpfr
 - scons

## Instrumentation-java
### Dependencies
 - javassist
 - maven
