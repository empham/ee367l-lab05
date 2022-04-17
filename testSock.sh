# a bash script to automate testing socket
#!/bin/bash

make sockTest
./server 3523 &
./client 3523

