#! /usr/bin/awk -f
#  ========================================================================  #
# 
# 	Author:	Mark G. Gray
# 		Los Alamos National Laboratory
# 	Date:	Fri Apr 11 14:34:28 MDT 2003
# 
# 	Copyright (c) 2003 U. S. Department of Energy. All rights reserved.
# 
#  	$Id$
# 
#  ========================================================================  #

BEGIN {
    printf("       real,             parameter :: NDI_REAL = 8\n");
    printf("       integer,          parameter :: NDI_INT  = 4\n");
}

NF == 3 && $1 == "#define" {
    printf("       integer(NDI_INT), parameter :: %-32s = %d\n", $2, $3);
}

#  ========================================================================  #
