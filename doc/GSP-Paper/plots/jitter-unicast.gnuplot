set terminal postscript eps
set output "jitter-unicast-plot.eps"
set isosamples 25,25
set hidden3d
set xrange [0:125]
set yrange [0:11]
set xlabel "time [s]"
set ylabel "jitter [ms]"
set pointsize 1
set grid

plot "jitter.dset" using 1:2:xtic(1):ytic(2) smooth csplines\
      notitle with lines,\
     "jitter.dset" using 1:2 title "Jitter (unicast)"\
      linecolor rgb("green") with points

# EOF
