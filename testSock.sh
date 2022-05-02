# a bash script to automate testing socket
#!/bin/bash

make sockTest
./server "wiliki.eng.hawaii.edu" 3523 &
./client "wiliki.eng.hawaii.edu" 3523

