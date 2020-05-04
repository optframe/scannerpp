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

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *_test
