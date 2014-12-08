#/bin/bash
echo -e "----------------------------------------------------------------\n"
echo -e "-------------------------- NEXT TEST ---------------------------\n"
echo -e "----------------------------------------------------------------\n"
echo -e "\n\t===== Testing Data layer =====\n"
make clean -C data/test
make test -C data/test
echo -e "\n\t ===== Testing Devices layer =====\n"
make clean -C players/test
make test -C players/test
echo -e "\n\t ===== Testing Controls layer =====\n"
make clean -C controls/test
make test -C controls/test
echo -e "\n\t ===== Testing Module layer =====\n"
make clean -C modules/test
make test -C modules/test
