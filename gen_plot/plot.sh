#!/bin/bash
# Generates a plot from the files created by aggregate_plotdata.sh using
# gnuplot and the graph_template.plg template file.

DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"

gnuplot << EOF
set terminal png nocrop medium size 800,600
set output '/dev/null'
load '$DIR/graph_template.plg'
set output '$1.png'
set title "$1 error and duration against p and n"
replot
EOF
