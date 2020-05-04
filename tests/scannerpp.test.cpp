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

TEST_CASE("Scanner++ Test String Hello World")
{
   Scanner scanner("Hello World");
   REQUIRE(scanner.next() == "Hello");
   REQUIRE(scanner.next() == "World");
}

TEST_CASE("Scanner++ Test File Hello World")
{
   Scanner scanfile(File("testbase/test-2.txt"));
   REQUIRE(scanfile.next() == "Hello");
   REQUIRE(scanfile.next() == "World");
}

TEST_CASE("Scanner++ Test String Hello World in Loop")
{
   Scanner scanner("Hello World");
   std::string s;
   while ((s = scanner.next()) != "")
      REQUIRE(s.length() == 5); // "Hello", then "World"
   //
   std::stringstream ss;
   Scanner scanner2("Hello World");
   while ((s = scanner2.next()) != "")
      ss << s; // "Hello", then "World"
   REQUIRE(ss.str() == "HelloWorld");
}

TEST_CASE("Scanner++ Test nextInt() with basic separators")
{
   Scanner scanner("1    2\t3\n4  \t \r\n5");
   REQUIRE(*scanner.nextInt() == 1);
   REQUIRE(*scanner.nextInt() == 2);
   REQUIRE(*scanner.nextInt() == 3);
   REQUIRE(*scanner.nextInt() == 4);
   REQUIRE(*scanner.nextInt() == 5);
}

TEST_CASE("Scanner++ Test nextInt() with custom separators")
{
   Scanner scanner("1  , X 2\t3\n4.5");
   scanner.useSeparators(" ,X\t.\n");
   REQUIRE(*scanner.nextInt() == 1);
   REQUIRE(*scanner.nextInt() == 2);
   REQUIRE(*scanner.nextInt() == 3);
   REQUIRE(*scanner.nextInt() == 4);
   REQUIRE(*scanner.nextInt() == 5);
}

TEST_CASE("Scanner++ Test nextInt() negative and positive")
{
   Scanner scanner("0 -1 9999 1");
   REQUIRE(*scanner.nextInt() == 0);
   REQUIRE(*scanner.nextInt() == -1);
   REQUIRE(*scanner.nextInt() == 9999);
   REQUIRE(*scanner.nextInt() == 1);
}

TEST_CASE("Scanner++ Test nextInt() with float cast")
{
   Scanner scanner("3.14 4.7 6");
   REQUIRE(*scanner.nextInt() == 3); // int(3.14)
   REQUIRE(*scanner.nextInt() == 4); // int(4.7)
   REQUIRE(*scanner.nextInt() == 6); // int(6)
}

TEST_CASE("Scanner++ Test nextFloat()")
{
   Scanner scanner("2.5 4");
   REQUIRE(*scanner.nextFloat() == 2.5);
   REQUIRE(*scanner.nextFloat() == 4);
}

TEST_CASE("Scanner++ Test nextLong()")
{
   Scanner scanner("2147483647 2147483648 8589934592");
   REQUIRE(*scanner.nextInt() == 2147483647);   // no overflow
   REQUIRE(scanner.nextInt() == std::nullopt);  // overflow
   REQUIRE(*scanner.nextLong() == 8589934592L); // 2^33
}

TEST_CASE("Scanner++ Test nextDouble()")
{
   Scanner scanner("9 0.125");
   REQUIRE(*scanner.nextDouble() == 9);
   REQUIRE(*scanner.nextDouble() == 0.125);
}

TEST_CASE("Scanner++ Test trim()")
{
   REQUIRE(Scanner::trim(" \t \n  Hello  World \n \t ") == "Hello  World");
}

TEST_CASE("Scanner++ Test read from non-existing file")
{
   Scanner scanner(File("testbase/test-do-not-exist.txt"));
   // file should not be open
   REQUIRE(scanner.filename() == "");
   REQUIRE(!scanner.fileIsOpen());
   // trying to get 'next' anyway! it should be empty and not sigfault
   std::string s = scanner.next();
   REQUIRE(s == "");
}
