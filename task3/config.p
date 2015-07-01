set terminal png enhanced

set xlabel "number of characters (n)"
set ylabel "milliseconds"

k = 0.00335

f(x) = k * x

plot\
"main.dat" using 2:4:6:xticlabel(2)  with errorlines title "processed Characters", \
"main.dat" using 2:(f($2)) with linespoints title "test"
