#/bin/bash
echo "Testing Data layer\n"
make clean -C data/test
make test -C data/test
echo "Testing Devices layer\n"
make clean -C players/test
make test -C players/test
echo "Testing Controls layer\n"
make clean -C controls/test
make test -C controls/test
echo "Testing Modules layer\n"
make clean -C modules/test
make test -C modules/test
