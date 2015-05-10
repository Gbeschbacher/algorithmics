set terminal png enhanced

set xlabel "searchword length"
set ylabel "time in nanosecs"

plot \
"data.dat" using 2:6:8:xticlabel(2)  with errorlines title "kmp", \
"data.dat" using 2:10:12:xticlabel(2)  with errorlines title "naive"