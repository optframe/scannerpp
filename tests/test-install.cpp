#include <iostream>
#include <scannerpp/Scanner.hpp>

using namespace scannerpp;

int
main()
{
   Scanner scanner("Hello World");
   std::cout << scanner.next() << endl; // Hello
   std::cout << scanner.next() << endl; // World

   return 0;
}