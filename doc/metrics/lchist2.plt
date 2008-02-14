set term post
set output "lchist2.ps"

a = 7000
b = 0.03
f(x) = a * (1.0 + b/100.0)**x

fit [x=4.83:7.92] f(x) 'lchist.dat' using 4:2 via a, b

set label sprintf("%5.2f\% growth per year", b) at 6, 9750
 
set yrange [6000:10000]
set xrange [3:8]
set title "NDI Size over Time"
set xlabel "Date (years)"
set x2label "Version"
set x2tics out rotate ("1.2.8" 3.50, "1.2.9" 3.92, "1.3.0" \
    4.17, "2.0.0" 4.83, "2.0.1" 5.17, "2.0.2" 5.58, \
    "2.0.3" 5.83, "2.0.4" 6.33, "2.0.5" 6.75, "2.0.6" \
    7.25, "2.0.7" 7.50, "2.0.8" 7.92)
set ylabel "Lines of Code"
set nokey
plot "lchist.dat" using 4:2 with points, f(x)
