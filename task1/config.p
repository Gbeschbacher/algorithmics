set terminal png enhanced

set xlabel "Fib(x) "
set ylabel "Nanoseconds"

set autoscale
set logscale

plot \
"fib2.dat" using 2:(1000 * $4):(1000 * $6):xticlabel(2) with errorlines title "Iterative", \
"fib3.dat" using 2:(1000 * $4):(1000 * $6):xticlabel(2) with errorlines title "Matrix (Bits)", \
"fib4.dat" using 2:(1000 * $4):(1000 * $6):xticlabel(2) with errorlines title "Matrix (Helper)", \
"fib5.dat" using 2:(1000 * $4):(1000 * $6):xticlabel(2) with errorlines title "Matrix (Optimized)"