#!/bin/sh

buildndi.sh
testndi.sh
echo "Continue with install? [y/n]:  "
read answer
if [ "$answer" = "y" ]; then
    installndi.sh
    echo "DONE"
fi
