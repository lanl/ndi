BEGIN {
     perms = "-rw-r[-w]-r--|drw[xsStT]rw[xsStT]r-[xsStT]";
     group = "pdata";
     result = 1;
}

NF > 4 {
     if (! ($1 ~ perms && $4 ~ group)) {
          print $0;
          result = 0;
     }
}

END {
     if (result) {
          printf("Permissions and groups under %s okay.\n", p);
     } else {
          printf("Permissions and group under %s broken.\n", p);
     }
     exit (! result);
}

          
