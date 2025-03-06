############################################################
## This file has been modified to optimize MatrixMultiplier
## Please DO NOT edit it.
## David Berrazueta & Byron Navas
############################################################
set_directive_pipeline "matrixmul"
set_directive_array_partition -type complete -dim 1 "matrixmul" a
set_directive_array_partition -type complete -dim 2 "matrixmul" a
set_directive_array_partition -type complete -dim 1 "matrixmul" b
set_directive_array_partition -type complete -dim 2 "matrixmul" b
set_directive_array_partition -type complete -dim 1 "matrixmul" res
set_directive_array_partition -type complete -dim 2 "matrixmul" res
