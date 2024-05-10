rm -r bin/receiver 2> /dev/null

gcc -Wall -pthread -o bin/receiver receiver.c -lhiredis -levent
