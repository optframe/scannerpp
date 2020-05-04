// Scanner Class for C++ - Scanner++
// MIT License
#ifndef SCANNERPP_SCANNER_HPP_
#define SCANNERPP_SCANNER_HPP_

#include <algorithm>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

#include <map>
#include <vector>

#include <stdlib.h>

using namespace std; // TODO: remove!

namespace scannerpp {

class File
{
public:
   std::ifstream* file{ nullptr };
   std::string filename;

   // standard constructive: pass 'filename'
   File(std::string _filename)
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

   // empty file (not open)
   File()
     : filename("")
   {
   }

   // move constructor
   File(File&& f)
     : file(f.file)
     , filename(f.filename)
   {
      f.file = nullptr;
      f.filename = "";
   }

   virtual ~File()
   {
      close();
      if (file)
         delete file;
   }

   // returns 'true' if file is open
   bool isOpen()
   {
      return file != nullptr;
   }

   int get()
   {
      return file && file->get();
   }

   bool eof()
   {
      return file && file->eof();
   }

   void close()
   {
      if (file)
         file->close();
   }
};

class ConversionError
{
private:
   bool whatInfo;
   std::string what;
   std::string type;

public:
   ConversionError(std::string _type)
     : whatInfo(false)
     , what("")
     , type(_type)
   {
   }

   ConversionError(std::string _what, std::string _type)
     : whatInfo(true)
     , what(_what)
     , type(_type)
   {
   }

   std::string getType() const
   {
      return type;
   }

   std::string getWhat() const
   {
      return what;
   }

   std::string getMessage() const
   {
      stringstream ss;
      if (whatInfo)
         ss << "value '" << what << "' is not of '" << type << "' type";
      else
         ss << "conversion error for '" << type << "' type";
      return ss.str();
   }
};

class Scanner
{
private:
   istream* input;
   File* inputfile;
   string sep;
   bool isString;

   string discarded;

   string contentString;

private:
   char nextChar(istream& _input) const
   {
      int x = _input.get();

      if (x <= 0) {
         //throw ConversionError("char");
         // TODO: use bool on return and char on parameter (better than optional)
         exit(1);
      }

      return x;
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
   string getDiscarded() const
   {
      return discarded;
   }

   bool hasNextChar() const;
   char nextChar();

   bool nextCharIs(char c) const;
   bool nextCharIn(string s) const;

   void trimInput();

   Scanner(File* inputfile);
   Scanner(istream* input);
   Scanner(string input);

   Scanner(const Scanner& scanner);

   virtual ~Scanner();

   virtual Scanner& operator=(const Scanner& scanner);

   // useDefaultSeparators: chama o useSeparators para os caracteres:
   // espaco, quebra de linha (\n), tabulacao (\t) e retorno de carro (\r)

   void useDefaultSeparators();

   // useSeparators: equivalente ao useDelimiter de Java
   // a diferenca e que Java trata a string como uma
   // expressao regular, e neste caso o useSeparators
   // apenas considera cada caractere da string separadamente
   // como um separador.

   void useSeparators(string s);
   bool inSeparators(char c) const;

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

} // end namespace scannerpp

#endif /*SCANNERPP_SCANNER_HPP_*/
