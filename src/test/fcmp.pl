#! /usr/bin/perl
# fcmp - compare ascii files with approximate numerical comparison

sub fcmp {			# approximately equal
    # See Knuth V2 pg. 234
    my ($u, $v, $e1, $e2) = @_;	# get parameters

    my $diff = $u - $v;		# difference
    my $delta = $e1 * (abs($u) > abs($v) ? abs($u) : abs($v)) + $e2;

    if ($diff > $delta) {
	return +1;		# u > v
    }
    elsif ($diff < -$delta) {
	return -1;		# u < v
    }
    else {			# -delta <= diff <= delta
	return 0;		# u ~ v
    }
}

if (scalar(@ARGV) != 2) { die("Usage: fcmp.pl file1 file2\n"); }

$file1 = $ARGV[0];
$file2 = $ARGV[1];
$number = "[+-]?[0-9]*[.][0-9]*[eE][+-]?[0-9]+";

open(FILE1, "tr -s ' \t' '\n\n' < $file1 |") or 
    die "Cannot open file: $file1\n";

open(FILE2, "tr -s ' \t' '\n\n' < $file2 |") or 
    die "Cannot open file: $file2\n"; 

while(chop($word1 = <FILE1>) && chop($word2 = <FILE2>)) {
    if ($word1 =~ /^$number$/o && $word2 =~ /^$number$/o){
	if (&fcmp($word1, $word2, 1.0e-5, 1.0e-10) != 0) {
	    exit 1;
	}
    }
    else {
	if ($word1 ne $word2) {
	    exit 1;
	}
    }
}

exit 0;
