set terminal postscript eps
set output "packet-loss-plot.eps"
set isosamples 25,25
set hidden3d
set xrange [25:525]
set yrange [0:10]
set ytics ("0.5" 0.5, "1" 1, "1.5" 1.5, "2" 2, "2.5" 2.5, "3" 3, "5" 5, "10" 10) font "Helvetica,11"
set xtics font "Helvetica,13"
set xlabel "Group size [members]"
set ylabel "Average Packet Loss [%]"
set pointsize 1
set grid

plot "packet-loss.dset" using 1:(100*$2/$4):xtic(1)\
     title "Unicast PoC" linetype 5 linecolor rgb "green" lw 3 with points, \
     "packet-loss.dset" using 1:(100*$3/$4) title "Multicast PoC"\
     linecolor rgb "blue" lw 3 with points,\
     2 linecolor rgb "red" lw 3 title "MOS level 3 boundary" with lines
# EOF
