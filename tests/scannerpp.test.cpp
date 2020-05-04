#include <iostream>

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
#include "libs/catch2/catch.hpp"

// Some instructions for Catch2
// https://github.com/catchorg/Catch2/blob/master/docs/test-cases-and-sections.md

#include "../src/Scanner.hpp"

using namespace std;
using namespace scannerpp;

TEST_CASE("Scanner++ Test Open File 'test-1.txt'")
{
   File file("testbase/test-1.txt");
   REQUIRE(file.isOpen());
}

TEST_CASE("Scanner++ Test Open File 'test-do-not-exist.txt'")
{
   File file2("testbase/test-do-not-exist.txt");
   REQUIRE(!file2.isOpen());
}

TEST_CASE("Scanner++ Test Open File 'test-1.txt' and move")
{
   File file3("testbase/test-1.txt");
   REQUIRE(file3.isOpen());
   File file4 = std::move(file3);
   // file3 should be closed and file4 open
   REQUIRE(!file3.isOpen());
   REQUIRE(file4.isOpen());
   // filename should be moved
   REQUIRE(file4.filename == "testbase/test-1.txt");
}

TEST_CASE("Scanner++ Test Open File 'test-1.txt' in Scanner")
{
   Scanner scanfile(File("testbase/test-1.txt"));
   REQUIRE(scanfile.filename() == "testbase/test-1.txt");
}

TEST_CASE("Scanner++ Test String Hello World first char")
{
   Scanner scanner("Hello World");
   REQUIRE(*scanner.nextChar() == 'H');
}

TEST_CASE("Scanner++ Test File Hello World first char")
{
   Scanner scanfile(File("testbase/test-2.txt"));
   REQUIRE(*scanfile.nextChar() == 'H');
}
