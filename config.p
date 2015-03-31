set terminal png enhanced

set xlabel "fib(n)"
set ylabel "Cycles"

set logscale x

plot \
"fib2.dat" using 2:4:6:xticlabel(2) with errorlines title "Iterative", \
"fib3.dat" using 2:4:6:xticlabel(2) with errorlines title "Matrix (Bits)", \
"fib4.dat" using 2:4:6:xticlabel(2) with errorlines title "Matrix (Helper)", \
"fib5.dat" using 2:4:6:xticlabel(2) with errorlines title "Matrix (Optimized)"