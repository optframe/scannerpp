all: test
	./scannerpp_test

test:
	g++ -g -Wall --std=c++17 -I../include/ -Ithirdparty/ scannerpp.test.cpp -o scannerpp_test --coverage
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

DIR_TL=/home/imcoelho/.conan/data/tl-expected/20190710/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9
DIR_BENCH=/home/imcoelho/.conan/data/benchmark/1.5.3/_/_/package/9ec7e6c55ee108e231bdef75e23776b8b86d821d

bench2:
#	g++-9 -g -D_GLIBCXX_USE_CXX11_ABI=0 -fsanitize=address  -g --std=c++17 -DNDEBUG -fPIC bench.cpp -I${DIR_TL}/include -I${DIR_BENCH}/include -L${DIR_BENCH}/lib -lbenchmark -pthread -lrt #-Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	g++-9 -g -D_GLIBCXX_USE_CXX11_ABI=0 --std=c++17 -DNDEBUG bench.cpp -I${DIR_TL}/include -I${DIR_BENCH}/include -L${DIR_BENCH}/lib -lbenchmark -pthread 

#bench: FCore.bench.cpp
#	g++-10.1 $< $(CXXFLAGS_BCH) -I../src -I../Examples $(GBENCH_DIRS) -o build/FCore_bench $(GBENCH_LIBS) -lpthread

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *_test
