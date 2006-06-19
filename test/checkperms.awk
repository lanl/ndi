BEGIN {
     perms = "-rw-rw-r--|drwxrwxr-x";
     group = "pdata";
     result = 1;
}

NF > 4 {
     result = result && ($1 ~ perms && $4  group);
}

END {
     if (result) {
          printf("Permissions and groups under %s okay.\n", p);
     } else {
          printf("Permissions and group under %s broken.\n", p);
     }
     exit (! result);
}

          
