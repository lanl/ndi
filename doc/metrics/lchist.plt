set term post
set output "lchist.ps"

set title "NDI Size over Time"
set timefmt "%m/%y"
set format x "%m/%y"
set xdata time
set x2data time
set xlabel "Date"
set xrange ["01/03":"01/08"]
set x2range ["01/03":"01/08"]
set x2label "Version"
set x2tics out rotate ("1.2.8" "07/03", "1.2.9" "12/03", "1.3.0" \
    "03/04", "2.0.0" "11/04", "2.0.1" "03/05", "2.0.2" "08/05", \
    "2.0.3" "11/05", "2.0.4" "05/06", "2.0.5" "10/06", "2.0.6" \
    "04/07", "2.0.7" "07/07", "2.0.8" "12/07")
set ylabel "Lines of Code"
set nokey
plot "lchist.dat" using 3:2 with fsteps
