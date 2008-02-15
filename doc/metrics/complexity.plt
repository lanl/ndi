set term post
set output "complexity.ps"
set yrange [0.9:100]
set xrange [0.9:1000]
set logscale xy
set nokey
set label "24% of code > 10" at 200, 70
set label "15% of code > 20" at 200, 60
set title "NDI 2.0.8"
set ylabel "Number of Functions"
set xlabel "McCabe Cyclomatic Complexity"
plot "complexity.hist" with impulses
