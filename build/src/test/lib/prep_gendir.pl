 
use Cwd;
 
# To call prep_gendir.pl:
#     perl prep_gendir.pl
#
#
# Input for file spec would be:
#     mendf5                                    (for one file in the current directory)
#     /usr/projects/data/nuclear/sn/mendf5_gen  (for one file)
#     /usr/projects/data/nuclear/sn/mendf*      (for files beginning with mendf)
#     /usr/projects/data/nuclear/sn/*           (for all files in directory)
#  To skip the current file, enter an asterisk (*) only.
#
# Input for library name is the name to be included on the gendir line;
# for example, mendf5  or rmoxsct.
#
 
print "Enter file spec: ";
my $filespec = <STDIN>;
my $curr_dir = cwd();
 
chomp($filespec);
 
if ( $filespec =~ /^\.\// ) {
    $filespec =~ s/^\.\//$curr_dir\//;
} elsif ( $filespec !~ /^\// ) {
    $filespec = $curr_dir . "/" . $filespec;
}
 
($sec, $min, $hour, $day, $mon, $year, $wd, $yd, $isdst) = localtime;
$mon += 1;
$year += 1900;
my $file_date = $mon . "/" . $day . "/" . $year;
my $lib_name = "";
my $nlib = "";
my $plib = "";
my $alib = "";
my $nlib = "";
my $ntlib = "";
my $trlib = "";
my $pdlib = "";
my $rclib = "";
my $ndoslib = "";
my $lib_type = "nm";
my @nfile = ();
my @pfile = ();
my @afile = ();
my @tnfile = ();
my @trfile = ();
my @pdfile = ();
my @rcfile = ();
my $nm_index = 0;
my $pm_index = 0;
my $tn_index = 0;
my $tr_index = 0;
my $pd_index = 0;
my $rc_index = 0;
 
for my $curr_file ( glob($filespec) ) {
 
    next if -d $curr_file;
    print "\n\n***FILE: $curr_file\n";
 
my $userlibset = 0;
my $userdateset = 0;
 
my $dateset = 0;
my $libnameset = 0;
 
#   Include file?
    print "  Include file? [n to skip]";
#    $process_file_flag = <STDIN>;
    $process_file_flag = "yes";
    next if ( $process_file_flag =~ /^\s*no?\s*$/i);
 
##       Process date
#    print "   Enter date ";
#    if ( $file_date ne "" ) {
#	print " [\'\*\' to skip; default=$file_date]: ";
#    } else {
#	print ": ";
#    }
#    $in_date = <STDIN>;
#    #If '*' is entered for in_date skip to next file
#    next if ( $in_date =~ /^\s*\*+\s*$/ );
#    if ( $in_date !~ /^\s*$/ ) {
#	$file_date = $in_date;
#	chomp($file_date);
#    }
 
 
    if ( open( CURR_LIB, $curr_file ) ) {
	$zaid = "";
	$zaid_ptr = "";
	$zaid_line = -1;
	$line_counter = 0;
	$fptr = -1;
	$file_type = "asc";
	$temperature = -1;
	$num_grps = -1;
	$sig_0 = -1;
	$at_wgt = -1;
	$awr = -1;
        $date_processed = "";
        $library_name = "";
	$line_len = 0;
	$out_line = "";
 
#       Cycle through each line of the current file
	$fptr = tell CURR_LIB;
	while ( <CURR_LIB> ) {
	
	    $line_counter++;
#           Process new zaid
	    if (/^\s*zaid\s*$/) {
		$zaid_ptr = $fptr;
		$zaid_line = $line_counter;
		while ( $zaid = <CURR_LIB> ) {
		    #Skip blank lines
		    $line_counter++;
		    if ( $zaid !~ /^\s*$/ ) {
			last;
		    }
		}
		$zaid =~ s/^\s*(.*?)\s*$/$1/;
	#	print "zaid=$zaid\n";
		if ( $dot_index = index($zaid, ".") >= 0 ) {
		    if ($zaid =~ /([A-Za-z]{2,3})\s*$/) {
			if ( $1 eq nm ) {
			    $lib_type = nm;
			} elsif ( $1 eq pm ) {
			   $ lib_type = pm;
			} elsif ( $1 eq am ) {
			    $lib_type = am;
			} elsif ( $1 eq "ztn" ) {
			    $lib_type = tn;
			} elsif ( $1 eq "zpd" ) {
			    $lib_type = pd;
			} elsif ( $1 eq "zrc" ) {
			    $lib_type = rc;
			} elsif ( $1 eq "ny" ) {
			    $lib_type = ndos;
 			} elsif ( $1 eq "ztr" ) {
			    $lib_type = tnr;
 			} else {
                            print "UNKNOWN ZAID TYPE $zaid.\n";
			    #unknown zaid type
			}
                    }
                }
	    }
 
	    #Process temperature
	    if (/^\s*temp\s*$/) {
		while ( $temperature = <CURR_LIB> ) {
		    #Skip blank lines
		    $line_counter++;
		    if ( $temperature !~ /^\s*$/ ) {
			last;
		    }
		}
		$temperature =~ s/^\s*(.*?)\s*$/$1/;
	    }
 
	    #Process number of groups
	    if (/^\s*num_grps\s*$/) {
		while ( $num_grps = <CURR_LIB> ) {
		    #Skip blank lines
		    $line_counter++;
		    if ( $num_grps !~ /^\s*$/ ) {
			last;
		    }
		}
		$num_grps =~ s/^\s*(.*?)\s*$/$1/;
	    }
	
	    #Process sigma zero
	    if (/^\s*sig_0\s*$/) {
		while ( $sig_0 = <CURR_LIB> ) {
		    #Skip blank lines
		    $line_counter++;
		    if ( $sig_0 !~ /^\s*$/ ) {
			last;
		    }
		}
		$sig_0 =~ s/^\s*(.*?)\s*$/$1/;
	    }
	
	    #Process atomic weight
	    if (/^\s*at_wgt\s*$/) {
		while ( $at_wgt = <CURR_LIB> ) {
		    #Skip blank lines
		    $line_counter++;
		    if ( $at_wgt !~ /^\s*$/ ) {
			last;
		    }
		}
		$at_wgt =~ s/^\s*(.*?)\s*$/$1/;
	    }
	
	    #Process atomic weight ratio
	    if (/^\s*awr\s*$/) {
		while ( $awr = <CURR_LIB> ) {
		    #Skip blank lines
		    $line_counter++;
		    if ( $awr !~ /^\s*$/ ) {
			last;
		    }
		}
		$awr =~ s/^\s*(.*?)\s*$/$1/;
	    }
	
	    #Process date_processed
	    if (/^\s*date_processed\s*$/) {
		while ( $date_processed = <CURR_LIB> ) {
		    #Skip blank lines
		    $line_counter++;
		    if ( $date_processed !~ /^\s*$/ ) {
			last;
		    }
		}
		$date_processed =~ s/^\s*(.*?)\s*$/$1/;
	    }
	
	    #Process library name
	    if (/^\s*library_name\s*$/) {
 		while ( $library_name = <CURR_LIB> ) {
		    #Skip blank lines
		    $line_counter++;
		    if ( $library_name !~ /^\s*$/ ) {
			last;
		    }
		}
		$library_name =~ s/^\s*(.*?)\s*$/$1/;
 	    }
	
	    #END of current isotope
	
	    if (/^\s*end\s*$/) {
		
                 if ( $library_name ne "" ) {
                    $print_lib_name = $library_name;
                } elsif ( $userlibset == 0 ) {
                    $userlibset = &get_lib_name($curr_file, $lib_name );
                    $print_lib_name = $lib_name;
                } else {
                    $print_lib_name = $lib_name;
                }
 
                if ( $date_processed ne "" ) {
                   $print_date = $date_processed;
               } elsif ( $userdateset == 0 ) {
                    $userdateset = &get_file_date($curr_file, $file_date );
                    $print_date = $file_date;
                } else {
                    $print_date = $file_date;
                }
 
		&add_to_string( $out_line, "z", $zaid );
		&add_to_string( $out_line, "d", $print_date );
		&add_to_string( $out_line, "l", $print_lib_name );
		&add_to_string( $out_line, "f", $curr_file );
		&add_to_string( $out_line, "ft", "asc" );
		&add_to_string( $out_line, "ln", $zaid_line );
		&add_to_string( $out_line, "o", $zaid_ptr );
		&add_to_string( $out_line, "ng", $num_grps );
		&add_to_string( $out_line, "t", $temperature );
		&add_to_string( $out_line, "s0", $sig_0 );
		&add_to_string( $out_line, "aw", $at_wgt );
		&add_to_string( $out_line, "awr", $awr );
		$out_line .= " end";
		
                 print " libtype $lib_type\n";
 
		if ( $lib_type eq "nm" ) {
		    $nfile[$nm_index] = $out_line;
		    $nm_index++;
		} elsif ( $lib_type eq "pm" ) {
		    $pfile[$pm_index] = $out_line;
		    $pm_index++;
		} elsif ( $lib_type eq "am" ) {
		    $afile[$am_index] = $out_line;
		    $am_index++;
		}  elsif ( $lib_type eq "tn" ) {
		    $tnfile[$tn_index] = $out_line;
		    $tn_index++;
		}  elsif ( $lib_type eq "tnr" ) {
		    $trfile[$tr_index] = $out_line;
		    $tr_index++;
		}  elsif ( $lib_type eq "pd" ) {
		    $pdfile[$pd_index] = $out_line;
		    $pd_index++;
		}  elsif ( $lib_type eq "rc" ) {
		    $rcfile[$rc_index] = $out_line;
		    $rc_index++;
		}  elsif ( $lib_type eq "ndos" ) {
		    $ndosfile[$ndos_index] = $out_line;
		    $ndos_index++;
		}
		$zaid = "";
		$zaid_ptr = "";
		$zaid_line = -1;
		$fptr = tell CURR_LIB;
		$file_type = "asc";
		$temperature = -1;
		$num_grps = -1;
		$sig_0 = -1;
		$at_wgt = -1;
		$awr = -1;
		$line_len = 0;
		$out_line = "";
 
	    }
	
	    $fptr = tell CURR_LIB;
	}
	close(CURR_LIB);
    } else {
	print "No >$curr_file<\n";
    }
 
}
 
open( OUTFILE, ">gendir.out" ) || die "Unable to open nd output file";
if ( $nm_index != 0 ) {
    print OUTFILE "multigroup_neutron\n";
    foreach (@nfile) {
	print OUTFILE "$_\n";
    }
    print OUTFILE "end\n\n";
}
if ( $pm_index != 0 ) {
    print OUTFILE "multigroup_photon\n";
    foreach (@pfile) {
	print OUTFILE "$_\n";
    }
    print OUTFILE "end\n\n";
}
if ( $am_index != 0 ) {
    print OUTFILE "multigroup_alpha\n";
    foreach (@afile) {
	print OUTFILE "$_\n";
    }
    print OUTFILE "end\n\n";
}
if ( $tn_index != 0 ) {
    print OUTFILE "tndata\n";
    foreach (@tnfile) {
	print OUTFILE "$_\n";
    }
    print OUTFILE "end\n\n";
}
if ( $tr_index != 0 ) {
    print OUTFILE "tn_reactions\n";
    foreach (@trfile) {
	print OUTFILE "$_\n";
    }
    print OUTFILE "end\n\n";
}
if ( $pd_index != 0 ) {
    print OUTFILE "neutron_depletion\n";
    foreach (@pdfile) {
	print OUTFILE "$_\n";
    }
    print OUTFILE "end\n\n";
}
if ( $rc_index != 0 ) {
    print OUTFILE "radchem_chain\n";
    foreach (@rcfile) {
	print OUTFILE "$_\n";
    }
    print OUTFILE "end\n\n";
}
if ( $ndos_index != 0 ) {
    print OUTFILE "dosimetry_neutron\n";
    foreach (@ndosfile) {
	print OUTFILE "$_\n";
    }
    print OUTFILE "end\n\n";
}
 
close(OUTFILE);
print "File gendir.out written.\n";
 
###########################################################
 
sub get_lib_name {
 
#   first argument is string giving file name
#   second argument is variable for library name
 
    my $in_lib;
 
    print "   Enter $_[0] library name";
    if ( $_[1] ne "" ) {
	print " [default= $_[1]]: ";
    } else {
	print ": ";
    }
    $in_lib = <STDIN>;
    if ( $in_lib !~ /^\s*$/ ) {
	$_[1] = $in_lib;
	chomp( $_[1]);
    }
    -1;
 
}
 
###########################################################
 
sub get_file_date {
 
#   first argument is string giving file name
#   second argument is variable for date
 
    my $in_date;
 
    print "   Enter $_[0] date";
    if ( $_[1] ne "" ) {
	print " [default= $_[1]]: ";
    } else {
	print ": ";
    }
    $in_date = <STDIN>;
    if ( $in_date !~ /^\s*$/ ) {
	$_[1] = $in_date;
	chomp( $_[1]);
    }
    -1;
 
}
 
###########################################################
 
sub add_to_string {
 
#   1st argument ($_[0]) is string to add to
#   2nd argument ($_[1]) is gendir keyword (e.g. "z", "ln",... )
#   x3rd argument ($_[2]) is keyword value (e.g. "92235.600nm" )
 
    my $temp_str = "";
    my $line_len = length($_[0]) - rindex( $_[0], "\n" );
    my $str2 = "=";
 
    #Check if these are empty strings
    if ( ( $_[1] eq "z" ) ||   #zaid
	( $_[1] eq "d" ) ||    #file date
	( $_[1] eq "l" ) ||    #library name
	( $_[1] eq "f" ) ) {   #file
	if ( $_[2] =~ /^\s*$/ ) {
	    print "  Error: value for keyword $_[1] is empty string.\n";
	    return $_[0];
	}
    } elsif ( ( $_[1] eq "ln" ) ||
	     ( $_[1] eq "o" ) ||
	     ( $_[1] eq "ng" ) ||
	     ( $_[1] eq "t" ) ||
	     ( $_[1] eq "s0" ) ||
	     ( $_[1] eq "aw" ) ||
	     ( $_[1] eq "awr" ) ) {
	if ( $_[2] < 0 ) {
	    return $_[0];
	}
    } elsif ( $_[1] eq "ft" ) {
	if ( $_[2] ne "asc" ) {
	    print "Warning: non-ASCII file found.\n";
	}
    } else {
	print "Warning: unknown keyword $_[1].\n";
    }
 
    #Add keyword, "=", and value to create temp. string
    $temp_str = "  " . $_[1] . $str2 . $_[2];
 
    #See if adding new temp string will make line length > 90
    if ( (length ($temp_str) + $line_len ) >= 90 ) {
	$_[0] .= "\n  ";
    }
 
    #Add temp string to string
    $_[0] .= $temp_str;
    $line_len = length($_[0]) - rindex( $_[0], "\n" );
 
    return $_[0];
}
 
###########################################################
 
#  Make sure .. are handled in file names.
 
###########################################################
 
