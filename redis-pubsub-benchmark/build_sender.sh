rm -r bin/sender 2> /dev/null

gcc -Wall -pthread -o bin/sender sender.c -lhiredis
