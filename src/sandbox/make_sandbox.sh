#!/bin/bash

TEMPLATE_DIR=templates

[[ $# != 2 ]] && echo "Usage $0 <template_name> <sandbox_name>" && exit 1


[[ ! -d $TEMPLATE_DIR/$1 ]] && echo "Template \"$1\" does not exist" && exit
[[ -d $2 ]] && echo "Sandbos \"$2\" already exists" && exit


cp $TEMPLATE_DIR/$1 $2 -rfv
echo "target_name = \"sandox.$2\"" >> $2/SConscript
cat $TEMPLATE_DIR/SConscript.template >> $2/SConscript
