set terminal postscript eps
set output "jitter-multicast-plot.eps"
set isosamples 25,25
set hidden3d
set xrange [0:125]
set yrange [0:3]
set xlabel "time [s]"
set ylabel "jitter [ms]"
set pointsize 1
set grid

plot "jitter.dset" using 1:3:xtic(1):ytic(3) smooth csplines\
      notitle with lines,\
     "jitter.dset" using 1:3 title "Jitter (multicast)"\
      linecolor rgb("green") with points

# EOF
