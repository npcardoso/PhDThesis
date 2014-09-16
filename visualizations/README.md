# Visualizations
Visualization Stuff

## Guidelines
### Indentation
#### Use uncrustify to indent code:
```
cd .git/hooks

ln -s ../../configs/uncrustify-hook pre-commit
```


### Javascript Compilation
Before the each product realease, all javascript code should be minified and compiled using [Google Closure Compiler](http://dl.google.com/closure-compiler/compiler-latest.zip). 
####  Use this command:
```
java -jar compiler.jar --js_output_file=crowbar.min.js pathJsFile1.js pathJsFile2.js pathJsFile3.js
```
## Dependencies
