all: test
	./scannerpp_test

test:
	g++ -g -Wall --std=c++17 scannerpp.test.cpp -o scannerpp_test --coverage
	#g++ --std=c++17 -fno-exceptions scannerpp.test.cpp -o scannerpp_test

test-install:
	g++ -g --std=c++17 test-install.cpp -o scannerpp_test_install
	./scannerpp_test_install

test-coverage:
	mkdir -p reports
	lcov --directory . --capture --output-file reports/app.info
	(cd reports && genhtml app.info)

bench:
	mkdir -p build
	g++ -fno-exceptions -std=c++17 -Ofast -I../libs/ bench.cpp -o build/app_bench

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *_test
