/ZAID COUNT/ {
     if (count)
          count = count "," $4;
     else
          count = $4;
}

END { printf("%s\t%s\t%s\t%s\n", d, m, p, count) }
