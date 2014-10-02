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

## Instrumentation-c++
### Dependencies
 - llvm/clang (3.4.2)
 - boost (1.55)

## Visualizations
### Dependencies
### Javascript Compilation
Before the each product realease, all javascript code should be minified and compiled using [Google Closure Compiler](http://dl.google.com/closure-compiler/compiler-latest.zip).
####  Use this command:
```
java -jar compiler.jar --js_output_file=crowbar.min.js pathJsFile1.js pathJsFile2.js pathJsFile3.js
```
