#/bin/bash

cd programas
make dynamic_recursive
cd ..
./programas/dynamic_recursive input/input_1 input/input_2 /tmp/dynamic_recursive/gold
./watchdog.py config_files/dynamic_recursive.conf
