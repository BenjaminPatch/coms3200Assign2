#!/bin/bash

arg=$1
echo $arg
sudo -u benpa python3 ./client.py 3000 $1 2
