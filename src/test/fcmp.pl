#! /usr/bin/perl

=head1 NAME

fcmp - compare two ascii files with approximate numerical comparison

=head1 SYNOPSIS

B<fcmp [-s -r rel_err -a abs_err]> I<file1> I<file2>

=head1 DESCRIPTION

The I<fcmp> utility compares two ascii files token by token, using an
approximate comparison for numeric values,  and writes the 
results to the standard output.  By default, I<fcmp> is silent if the files 
are the same; if they differ, the first difference is reported.

The following options are available:

    -s Print nothing for differing files; return exit status only.

    -r Set relative error tolerance for numeric comparison.

    -a Set absolute error tolerance for numeric comparison.

The I<fcmp> utility exits with one of the following values:

0    The files are identical.

1    The files are different.

>1   An error occurred.

=head1 AUTHOR

Mark G. Gray

=head1 SEE ALSO

cmp(1)

=cut

use Getopt::Std;		# option parser

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

our ($opt_s, $opt_r, $opt_a);	# declare options
$opt_s = 0; 
$opt_r = 1.0e-5;
$opt_a = 1.0e-10;

getopts('sr:a:');		# get options

if (scalar(@ARGV) != 2) { die("Usage: fcmp.pl [-s -r # -a #] file1 file2\n"); }

$file1 = $ARGV[0];
$file2 = $ARGV[1];

$number = "[+-]?[0-9]*[.][0-9]*[eE][+-]?[0-9]+";

open(FILE1, "tr -s ' \t' '\n\n' < $file1 |") or 
    die "Cannot open file: $file1\n";

open(FILE2, "tr -s ' \t' '\n\n' < $file2 |") or 
    die "Cannot open file: $file2\n";

while(chop($word1 = <FILE1>) . chop($word2 = <FILE2>)) {
    if ($word1 =~ /^$number$/o && $word2 =~ /^$number$/o){
	if (&fcmp($word1, $word2, $opt_r, $opt_a) != 0) {
	    if (! $opt_s) { 
		printf("$file1 $file2 differ: %g !~ %g\n", $word1, $word2); 
	    }
	    exit 1;
	}
    }
    else {
	if ($word1 ne $word2) {
	    if (! $opt_s) { 
		printf("$file1 $file2 differ: %s ne %s\n", $word1, $word2);
	    }
	    exit 1;
	}
    }
}

exit 0;
