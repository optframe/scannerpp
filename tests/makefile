all: test
	./scannerpp_test

test:
	g++ --std=c++17 scannerpp.test.cpp -o scannerpp_test
	#g++ --std=c++17 -fno-exceptions scannerpp.test.cpp -o scannerpp_test

test-install:
	g++ --std=c++17 test-install.cpp -o scannerpp_test_install
	./scannerpp_test_install