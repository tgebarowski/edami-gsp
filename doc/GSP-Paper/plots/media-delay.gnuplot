set terminal postscript eps
set output "media-delay-plot.eps"
set isosamples 25,25
set hidden3d
set xrange [0:525]
set yrange [0:400]
set ytics font "Helvetica,13"
set xtics font "Helvetica,13"
set xlabel "Group size [members]"
set ylabel "Media Packet Delay [ms]"
set pointsize 1
set grid
set ytics 100

plot "media-delay.dset" using 1:4 title ""\
              linecolor rgb("blue") lw 3 with points

#plot "media-delay.dset" using 1:2:3:xtic(1)\
#      notitle lw 2 with yerrorbars,\
#     "media-delay.dset" using 1:2 title "First Media Packet Delay (unicast)"\
#      linecolor rgb("green") lw 3 with points,\
#     "media-delay.dset" using 1:4:5:xtic(1)\
#      notitle linecolor rgb("blue") lw 2 with yerrorbars,\
#     "media-delay.dset" using 1:4 title "First Media Packet Delay (multicast)"\
#      linecolor rgb("blue") lw 3 with points
# EOF
