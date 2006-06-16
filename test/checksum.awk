/ZAID COUNT/ {
     if (count)
          count = count "," $4;
     else
          count = $4;
}

END { print count }
