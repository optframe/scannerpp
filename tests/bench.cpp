#include <chrono>
#include <iostream>
#include <optional>

#include <system_error>

#include <tl/expected.hpp>

#include <benchmark/benchmark.h>

// initializes MAIN
BENCHMARK_MAIN();

using namespace std;

std::optional<int>
foo(int i)
{
   int x = i;
   return x;
}

//tl::expected<int, std::error_code>
tl::expected<int, std::error_code>
//tl::expected<int, int>
bar(int i)
{
   int x = i;
   return x;
}

tl::expected<int, int>
//tl::expected<int, int>
bar_int_int(int i)
{
   int x = i;
   return x;
}

std::optional<int>
anti_foo(int i)
{
   return std::nullopt;
}

tl::expected<int, std::error_code>
anti_bar(int i)
{
   return tl::unexpected<std::error_code>(std::make_error_code(std::errc{}));
}

tl::expected<int, int>
//tl::expected<int, int>
anti_bar_int_int(int i)
{
   return tl::unexpected<int>(0);
}

int
direct(int i)
{
   int x = i;
   return x;
}

/*
int
main()
{
   unsigned MAX_TESTS = 10'000'000;
   //
   auto st = std::chrono::high_resolution_clock::now();
   int add_1 = 0;
   for (long i = 0; i < MAX_TESTS; i++) {
      add_1 += *foo(i);
   }
   auto en = std::chrono::high_resolution_clock::now();
   long t1 = std::chrono::duration_cast<std::chrono::nanoseconds>(en - st).count();
   //
   auto st2 = std::chrono::high_resolution_clock::now();
   int add_2 = 0;
   for (long i = 0; i < MAX_TESTS; i++) {
      add_2 += *bar(i);
   }
   auto en2 = std::chrono::high_resolution_clock::now();
   long t2 = std::chrono::duration_cast<std::chrono::nanoseconds>(en2 - st2).count();
   //
   auto st3 = std::chrono::high_resolution_clock::now();
   int add_3 = 0;
   for (long i = 0; i < MAX_TESTS; i++) {
      add_3 += direct(i);
   }
   auto en3 = std::chrono::high_resolution_clock::now();
   long t3 = std::chrono::duration_cast<std::chrono::nanoseconds>(en3 - st3).count();

   std::cout << add_1 << std::endl;
   std::cout << add_2 << std::endl;
   std::cout << add_3 << std::endl;
   //
   std::cout << t1 << std::endl;
   std::cout << t2 << std::endl;
   std::cout << t3 << std::endl;
   //std::cout << "expected should cost HALF of optional" << std::endl;
   std::cout << t2 / (double)t1 * 100.0 << "%" << std::endl;
   //std::cout << "expected should have zero overhead" << std::endl;
   std::cout << t2 / (double)t3 * 100.0 << "%" << std::endl;

   return 0;
}
*/

//
static void
bench_direct_int(benchmark::State& state)
{
   unsigned N = state.range(0); // get N from benchmark suite
   //double ff = 0;
   for (auto _ : state) {
      double fcost = 0;
      for (long i = 0; i < N; i++)
         benchmark::DoNotOptimize(fcost += direct(i));
   }
   //benchmark::DoNotOptimize(ff = best);
   //benchmark::ClobberMemory();
   //assert(ff == 1);
}
BENCHMARK(bench_direct_int)
  ->Args({ 10 })   // N = 10
  ->Args({ 100 })  // N = 100
  ->Args({ 1000 }) // N = 1000
  ;

//
static void
bench_optional_int(benchmark::State& state)
{
   unsigned N = state.range(0); // get N from benchmark suite
   //double ff = 0;
   for (auto _ : state) {
      double fcost = 0;
      for (long i = 0; i < N; i++) {
         auto out = foo(i);
         if (out)
            benchmark::DoNotOptimize(fcost += *out);
      }
   }
   //benchmark::DoNotOptimize(ff = best);
   //benchmark::ClobberMemory();
   //assert(ff == 1);
}
BENCHMARK(bench_optional_int)
  ->Args({ 10 })   // N = 10
  ->Args({ 100 })  // N = 100
  ->Args({ 1000 }) // N = 1000
  ;

//
static void
bench_expected_int(benchmark::State& state)
{
   unsigned N = state.range(0); // get N from benchmark suite
   //double ff = 0;
   for (auto _ : state) {
      double fcost = 0;
      for (long i = 0; i < N; i++) {
         auto out = bar(i);
         if (out)
            benchmark::DoNotOptimize(fcost += *out);
      }
   }
   //benchmark::DoNotOptimize(ff = best);
   //benchmark::ClobberMemory();
   //assert(ff == 1);
}
BENCHMARK(bench_expected_int)
  ->Args({ 10 })   // N = 10
  ->Args({ 100 })  // N = 100
  ->Args({ 1000 }) // N = 1000
  ;

//
static void
bench_expected_int_int(benchmark::State& state)
{
   unsigned N = state.range(0); // get N from benchmark suite
   //double ff = 0;
   for (auto _ : state) {
      double fcost = 0;
      for (long i = 0; i < N; i++) {
         auto out = bar_int_int(i);
         if (out)
            benchmark::DoNotOptimize(fcost += *out);
      }
   }
   //benchmark::DoNotOptimize(ff = best);
   //benchmark::ClobberMemory();
   //assert(ff == 1);
}
BENCHMARK(bench_expected_int_int)
  ->Args({ 10 })   // N = 10
  ->Args({ 100 })  // N = 100
  ->Args({ 1000 }) // N = 1000
  ;

//
static void
bench_anti_optional_int(benchmark::State& state)
{
   unsigned N = state.range(0); // get N from benchmark suite
   //double ff = 0;
   for (auto _ : state) {
      double fcost = 0;
      for (long i = 0; i < N; i++) {
         auto out = anti_foo(i);
         if (out)
            benchmark::DoNotOptimize(fcost += *out);
      }
   }
   //benchmark::DoNotOptimize(ff = best);
   //benchmark::ClobberMemory();
   //assert(ff == 1);
}
BENCHMARK(bench_anti_optional_int)
  ->Args({ 10 })   // N = 10
  ->Args({ 100 })  // N = 100
  ->Args({ 1000 }) // N = 1000
  ;

//
static void
bench_anti_expected_int(benchmark::State& state)
{
   unsigned N = state.range(0); // get N from benchmark suite
   //double ff = 0;
   for (auto _ : state) {
      double fcost = 0;
      for (long i = 0; i < N; i++) {
         auto out = anti_bar(i);
         if (out)
            benchmark::DoNotOptimize(fcost += *out);
      }
   }
   //benchmark::DoNotOptimize(ff = best);
   //benchmark::ClobberMemory();
   //assert(ff == 1);
}
BENCHMARK(bench_anti_expected_int)
  ->Args({ 10 })   // N = 10
  ->Args({ 100 })  // N = 100
  ->Args({ 1000 }) // N = 1000
  ;

//
static void
bench_anti_expected_int_int(benchmark::State& state)
{
   unsigned N = state.range(0); // get N from benchmark suite
   //double ff = 0;
   for (auto _ : state) {
      double fcost = 0;
      for (long i = 0; i < N; i++) {
         auto out = anti_bar_int_int(i);
         if (out)
            benchmark::DoNotOptimize(fcost += *out);
      }
   }
   //benchmark::DoNotOptimize(ff = best);
   //benchmark::ClobberMemory();
   //assert(ff == 1);
}
BENCHMARK(bench_anti_expected_int_int)
  ->Args({ 10 })   // N = 10
  ->Args({ 100 })  // N = 100
  ->Args({ 1000 }) // N = 1000
  ;