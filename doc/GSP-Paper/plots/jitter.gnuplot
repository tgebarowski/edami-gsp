set terminal postscript eps
set output "jitter-plot.eps"
set isosamples 25,25
set hidden3d
set xrange [0:125]
set yrange [0:130]
set xlabel "time [s]"
set ylabel "jitter [ms]"
set ytics font "Helvetica,13"
set xtics font "Helvetica,13"
set pointsize 1
set grid

plot "jitter.dset" using 1:2:xtic(1):ytic(2) smooth csplines\
      notitle linecolor rgb("red") lw 3 with lines,\
     "jitter.dset" using 1:2 title "Jitter (unicast)"\
      linecolor rgb("red") with points,\
      "jitter.dset" using 1:3:xtic(1):ytic(3) smooth csplines\
      notitle linecolor rgb("blue") lw 3 with lines,\
     "jitter.dset" using 1:3 title "Jitter (multicast)"\
      linecolor rgb("blue") with points

# EOF
