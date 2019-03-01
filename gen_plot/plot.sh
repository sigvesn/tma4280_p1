#!/bin/bash
gnuplot << EOF
set terminal png nocrop medium size 800,600
set output '/dev/null'
load 'graph_template.plg'
set output '$1.png'
set title "$1 error and duration against p and of n"
replot
EOF
