# como passar arquivo por parametro:
# gnuplot -e "ARQ='arquivo.txt'"
clear
reset
set key off
set border 3

if (!exists("ARQ")) exit

stats ARQ

# Add a vertical dotted line at x=0 to show centre (mean) of distribution.
set yzeroaxis

bin_width = 5000;

# Each bar is half the (visual) width of its x-range.
set boxwidth bin_width/2 absolute
set style fill solid 1.0 noborder


bin_number(x) = floor(x/bin_width)

rounded(x) = bin_width * ( bin_number(x) + 0.5 )

xsize = STATS_mean_y + ( 2 * STATS_stddev_y)

set xrange [0:xsize]

plot ARQ using (rounded($1)):(1) smooth frequency with boxes
pause -1
