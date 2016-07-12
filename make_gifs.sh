#!/bin/bash

set -e

TMP_DIR=`mktemp -d`
echo $TMP_DIR

cd `dirname $0`

function remove_alpha() {
    convert -monitor -alpha remove -background white -antialias  $*
}

function to_gif() {
    convert -monitor -loop 0 -strip -layers OptimizePlus -delay 50  -antialias $*

}

remove_alpha -density 50 thesis/main.pdf $TMP_DIR/thesis_raster.pdf

pdfnup $TMP_DIR/thesis_raster.pdf {},1- -o $TMP_DIR/thesis_nup.pdf

to_gif $TMP_DIR/thesis_nup.pdf thesis.gif


remove_alpha presentation/main.pdf $TMP_DIR/presentation_raster.pdf

to_gif -density 300 $TMP_DIR/presentation_raster.pdf presentation.gif


rm -rfv $TMP_DIR
