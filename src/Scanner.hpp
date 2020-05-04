// Scanner Class for C++ - Scanner++
// MIT License
#ifndef SCANNERPP_SCANNER_HPP_
#define SCANNERPP_SCANNER_HPP_

#include <algorithm>
#include <iostream>
#include <istream>
#include <optional>
#include <sstream>
#include <string>

#include <map>
#include <vector>

#include <stdlib.h>

using namespace std; // TODO: remove!

namespace scannerpp {

class File final
{
public:
   std::ifstream* file{ nullptr };
   std::string filename;

   // standard constructive: pass 'filename'
   File(std::string _filename) noexcept
     : filename(_filename)
   {
      // check if file exists
      std::fstream fs;
      fs.open(filename.c_str());
      if (fs.is_open()) {
         fs.close();
         // store definitive pointer
         file = new std::ifstream(filename.c_str(), std::ifstream::in);
      }
   }

   /*
   // empty file (not open)
   File()
     : filename("")
   {
   }
*/

   // move constructor
   File(File&& f) noexcept
     : file(f.file)
     , filename(f.filename)
   {
      f.file = nullptr;
      f.filename = "";
   }

   ~File() noexcept
   {
      close();
      if (file)
         delete file;
   }

   // returns 'true' if file is open
   bool isOpen() noexcept
   {
      return file != nullptr;
   }

   int get() noexcept
   {
      return file && file->get();
   }

   bool eof() noexcept
   {
      return file && file->eof();
   }

   void close() noexcept
   {
      if (file)
         file->close();
      filename = "";
   }
};

class Scanner final
{
private:
   istream* input{ nullptr };
   File* inputfile{ nullptr };
   string sep;
   bool isString;

   string discarded;

   string contentString;

public:
   Scanner(File&& f) noexcept
   {
      isString = false;
      this->inputfile = new File(std::move(f));
      this->input = inputfile->file;
      useDefaultSeparators();
   }

   Scanner(std::istream* input) noexcept
   {
      isString = false;
      this->inputfile = nullptr;
      this->input = input;
      useDefaultSeparators();
   }

   Scanner(std::string input) noexcept
   {
      isString = true;
      this->inputfile = nullptr;
      this->contentString = input;
      this->input = new std::istringstream(input);
      useDefaultSeparators();
   }

   // copy constructor
   Scanner(const Scanner& scanner) noexcept
   {
      contentString = scanner.contentString;
      isString = scanner.isString;
      discarded = scanner.discarded;

      if (scanner.inputfile) //for files
      {
         inputfile = new File(scanner.inputfile->filename);
         input = inputfile->file;
      }

      if (isString) // for string
         input = new istringstream(contentString);

      if (!isString) // for cin
         input = scanner.input;

      useDefaultSeparators();
   }

   ~Scanner() noexcept
   {
      if (inputfile) {
         delete inputfile;
         inputfile = nullptr;
         input = nullptr;
      }

      if (input && isString) {
         delete input;
      }
   }

   Scanner& operator=(const Scanner& scanner) noexcept
   {
      if (&scanner == this) // auto ref check
         return *this;

      // ==========
      // destructor
      // ==========

      if (inputfile) {
         delete inputfile;
         inputfile = nullptr;
         input = nullptr;
      }

      if (input && isString) {
         delete input;
      }
      // ==========

      contentString = scanner.contentString;
      isString = scanner.isString;
      discarded = scanner.discarded;

      if (scanner.inputfile) //for files
      {
         inputfile = new File(scanner.inputfile->filename);
         input = inputfile->file;
      }

      if (isString) // for string
         input = new istringstream(contentString);

      if (!isString) // for cin
         input = scanner.input;

      useDefaultSeparators();

      return *this;
   }

private:
   //char nextChar(istream& _input) const noexcept
   bool nextChar(istream& _input, char& c) const noexcept
   {
      int x = _input.get();

      if (x <= 0) {
         //throw ConversionError("char");
         // TODO: use bool on return and char on parameter (better than optional)
         //exit(1);
         return false;
      }

      // returns char on parameter 'c' (no exception is ever thrown)
      c = x;
      //return x;
   }

   void put_back(istream** input, string back) const
   {
      if ((*input)->eof()) {
         // change internal pointer to a renewed istream
         delete (*input);
         (*input) = new istringstream(back);
      } else {
         // just return everything back!

         for (int i = ((int)back.length()) - 1; i >= 0; i--) {
            (*input)->putback(back[i]);
            if ((*input)->fail()) {
               cout << "SCANNER ERROR PUTTING BACK CHAR '" << back[i] << "'" << endl;
               cout << "eof bit: '" << (*input)->eof() << "'" << endl;
               cout << "bad bit: '" << (*input)->bad() << "'" << endl;
               cout << "fail bit: '" << (*input)->fail() << "'" << endl;
               exit(1);
            }
         }
      }
   }

public:
   // useDefaultSeparators: chama o useSeparators para os caracteres:
   // espaco, quebra de linha (\n), tabulacao (\t) e retorno de carro (\r)

   void useDefaultSeparators()
   {
      useSeparators(string("\n\r\t "));
   }

   // useSeparators: equivalente ao useDelimiter de Java
   // a diferenca e que Java trata a string como uma
   // expressao regular, e neste caso o useSeparators
   // apenas considera cada caractere da string separadamente
   // como um separador.

   void useSeparators(string s)
   {
      sep = s;
   }

   bool inSeparators(char c) const noexcept
   {
      for (unsigned int i = 0; i < sep.length(); i++)
         if (sep[i] == c)
            return true;
      return false;
   }

   // returns filename, if some file is open
   string filename() const noexcept
   {
      return inputfile ? inputfile->filename : "";
   }

   string getDiscarded() const
   {
      return discarded;
   }

public:
   // public interfaces: nextChar and hasNextChar

   bool hasNextChar() const noexcept
   {
      if (input->eof())
         return false;

      int x = input->peek();

      if (input->fail()) {
         //cout << "WARNING::SCANNER FAILED!" << endl;
         return false;
      }

      if (x > 0)
         return true;

      if (x == 0)
         return false;

      return false;
   }

   std::optional<char> nextChar()
   {
      int x = input->get();

      if (x <= 0)
         //throw ConversionError("char");
         return std::nullopt;

      return std::make_optional(x);
   }

   bool nextCharIs(char c) const
   {
      stringstream ss;
      ss << c;
      string s = ss.str();

      return nextCharIn(s);
   }

   bool nextCharIn(string s) const
   {
      if (!hasNextChar())
         return false;

      bool r = false;

      int x = input->get();

      if (x > 0) {
         char c = x;

         for (unsigned i = 0; i < s.length(); i++)
            if (c == s.at(i)) {
               r = true;
               break;
            }
      }

      input->putback((char)x);

      return r;
   }

private:
   void trimInput() noexcept
   {
      string s = " \t\n";

      if (!hasNextChar())
         return;

      int x = input->get();

      while (x > 0) {
         char c = x;

         bool t = false;

         for (unsigned i = 0; i < s.length(); i++)
            if (c == s.at(i)) {
               t = true;
               break;
            }

         if (!t) {
            input->putback((char)x);
            return;
         }

         if (!hasNextChar())
            return;

         x = input->get();
      }
   }

public:
   std::string peekNext() const;
   std::string next();
   std::string nextLine();

   int nextInt();
   long nextLong();
   float nextFloat();
   double nextDouble();

   static int parseInt(string s)
   {
      Scanner scanner(s);
      return scanner.nextInt();
   }

   static double parseDouble(string s)
   {
      Scanner scanner(s);
      return scanner.nextDouble();
   }

   static bool trimChar(char c)
   {
      return (c == ' ') || (c == '\t') || (c == '\n');
   }

   static string trim(string word)
   {
      if (word.length() == 0)
         return "";

      int i = 0;
      char c = word.at(i);
      string aux_word = "";

      //removing initial spaces
      while (trimChar(c) && (i < ((int)word.length()) - 1)) {
         i++;
         c = word.at(i);
      }

      if (trimChar(c)) // last char
         i++;

      while (i < ((int)word.length())) {
         aux_word += word.at(i);
         i++;
      }

      word = aux_word;

      // may be empty at this point
      if (word.length() == 0)
         return "";

      //removing final spaces
      i = ((int)word.length()) - 1;
      c = word.at(i);

      while (trimChar(c) && (i > 0)) {
         i--;
         c = word.at(i);
      }

      aux_word = "";

      for (int j = 0; j <= i; j++)
         aux_word += word.at(j);

      return aux_word;
   }

   pair<string, map<string, string>> nextXMLTag();

   bool hasNext() const;

   bool hasNextLine() const
   {
      return hasNext();
   }

   bool hasNextInt() const
   {
      int x;
      if (hasNextX(x)) {
         double d;
         if (hasNextX(d))
            return (x == d);
      }
      return false;
   }

   bool hasNextLong() const
   {
      long x;
      if (hasNextX(x)) {
         double d;
         if (hasNextX(d))
            return (x == d);
      }
      return false;
   }

   bool hasNextFloat() const
   {
      float x;
      return hasNextX(x);
   }

   bool hasNextDouble() const
   {
      double x;
      return hasNextX(x);
   }

   template<class X>
   inline bool hasNextX(X& x) const
   {
      string s = peekNext();

      if (s == "")
         return false;

      std::istringstream ss(s);

      return bool(ss >> x);
   }

   string rest(); // Returns the rest of the input as string
};

} // namespace scannerpp

#endif /*SCANNERPP_SCANNER_HPP_*/
