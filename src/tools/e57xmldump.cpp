/*
 * e57xmldump.cpp - print XML section of E57 file to stdout.
 *
 * Copyright (c) 2009 - 2010 Kevin Ackley (kackley@gwi.net)
 * Copyright (c) 2020 - 2022 Michele Adduci (adduci@tutanota.com)
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include <iostream>
#include <openE57/impl/openE57Impl.h> //??? for exceptions, should be in separate file
#include <openE57/openE57.h>
using namespace e57;
using namespace std;
// using namespace std::tr1; //??? is this really needed, gives error on gcc <roland>

/// Uncomment the line below to enable debug printing
//#define VERBOSE 1

struct CommandLineOptions
{
  ustring inputFileName;

  CommandLineOptions(){};
  void parse(int argc, char** argv);
};

int main(int argc, char** argv)
{
  /// Catch any exceptions thrown.
  try
  {
    CommandLineOptions options;
    options.parse(argc, argv);

    const int      N = 1024;
    static uint8_t buf[N];

    uint64_t length = E57Utilities().rawXmlLength(options.inputFileName);

    uint64_t start = 0;
    while (start < length)
    {
      size_t count;
      if (length - start > N)
        count = N;
      else
        count = static_cast<size_t>(length - start);

      /// Read a block of xml text
      E57Utilities().rawXmlRead(options.inputFileName, buf, start, count);

      /// Send block to cout
      cout.write(reinterpret_cast<char*>(buf), count);

      start += count;
    }
  }
  catch (E57Exception& ex)
  {
    ex.report(__FILE__, __LINE__, __FUNCTION__);
    return -1;
  }
  catch (std::exception& ex)
  {
    cerr << "Got an std::exception, what=" << ex.what() << endl;
    return -1;
  }
  catch (...)
  {
    cerr << "Got an unknown exception" << endl;
    return -1;
  }
  return 0;
}

//================================================================

void usage(ustring msg)
{
  cerr << "ERROR: " << msg << endl;
  cerr << "Usage:" << endl;
  cerr << "    e57xmldump <e57_file>" << endl;
  cerr << "    For example:" << endl;
  cerr << "        e57fields scan0001.e57" << endl;
  cerr << endl;
}

void CommandLineOptions::parse(int argc, char** argv)
{
  /// Skip program name
  argc--;
  argv++;

  if (argc != 1)
  {
    usage("wrong number of command line arguments");
    throw EXCEPTION("wrong number of command line arguments"); //??? UsageException
  }

  inputFileName = argv[0];
}
