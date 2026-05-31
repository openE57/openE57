// $Id: e57unpack.cpp 338 2013-09-03 12:36:09Z roland_schwarz $
#define PROGRAM_NAME "e57unpack"
#define PROGRAM_VERSION "1.0"
#ifndef E57_REFIMPL_REVISION_ID
#  define E57_REFIMPL_REVISION_ID unknown
#endif
#define STRING_HELPER2(a) #a
#define STRING_HELPER1(a) STRING_HELPER2(a)
#define BUILD_VERSION STRING_HELPER1(E57_REFIMPL_REVISION_ID)

#if defined(_WIN32) || defined(_WIN64)
#  define PLATFORM_NAME "Windows"
#elif defined(__APPLE__)
#  define PLATFORM_NAME "macOS"
#elif defined(__linux__)
#  define PLATFORM_NAME "Linux"
#else
#  define PLATFORM_NAME "Unknown"
#endif

#include <openE57/openE57.h>
using e57::BlobNode;
using e57::CompressedVectorNode;
using e57::CompressedVectorReader;
using e57::E57Exception;
using e57::E57Utilities;
using e57::FloatNode;
using e57::ImageFile;
using e57::int64_t;
using e57::IntegerNode;
using e57::Node;
using e57::ScaledIntegerNode;
using e57::SourceDestBuffer;
using e57::StringNode;
using e57::StructureNode;
using e57::uint64_t;
using e57::uint8_t;
using e57::ustring;
using e57::VectorNode;

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using std::ios_base;
using std::ostream;
using std::streamsize;

#include <exception>
using std::exception;

#include <stdexcept>
using std::runtime_error;

#include <memory>

#include <string>
using std::string;
using std::to_string;

#include <vector>
using std::vector;

#include <limits>
using std::numeric_limits;

#include <cstring>
using std::strcmp;

#include <filesystem>
namespace fs = std::filesystem;

#include <variant>
using std::variant;
using std::get;
using std::visit;

#include <fstream>
using std::ofstream;

#include <sstream>
using std::ostringstream;

#include <iomanip>
using std::setprecision;

struct CommandLineOptions
{
  string src;
  string dst;
  string format;
  bool   noImages;
  bool   noPoints;

  CommandLineOptions() : noImages(false), noPoints(false) {}
  void parse(int argc, char** argv);
};

void usage(const string& msg)
{
  cerr << "ERROR: " << msg << endl;
  cerr << "Usage: " PROGRAM_NAME " [options] e57-file [unpack-directory]" << endl;
  cerr << "  The purpose of this program is to break the e57 file into" << endl;
  cerr << "  parts." << endl;
  cerr << endl;
  cerr << "  -F, --format <string>  format string for point data" << endl;
  cerr << "  --no-images            suppress image output" << endl;
  cerr << "  --no-points            suppress pointcloud output" << endl;
  cerr << "  -v, --version          show version" << endl;
  cerr << "  -h, --help             show help" << endl;
  cerr << endl;
  cerr << "  For example:" << endl;
  cerr << "      e57unpack scan0001.e57" << endl;
  cerr << "      e57unpack scan0001.e57 output_dir" << endl;
  cerr << "      e57unpack --no-images scan0001.e57 output_dir" << endl;
  cerr << endl;
  exit(-1);
}

void CommandLineOptions::parse(int argc, char** argv)
{
  argc--;
  argv++;

  for (; argc > 0 && *argv[0] == '-'; argc--, argv++)
  {
    if (strcmp(argv[0], "-v") == 0 || strcmp(argv[0], "--version") == 0)
    {
      int     astmMajor;
      int     astmMinor;
      ustring libraryId;
      E57Utilities().getVersions(astmMajor, astmMinor, libraryId);
      cout << PROGRAM_NAME " (" PLATFORM_NAME ") " << PROGRAM_VERSION << "." << '\n';
      cout << "E57 API " << astmMajor << "." << astmMinor << endl;
      cout << libraryId << endl;
      exit(0);
    }
    else if (strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0)
    {
      usage("help requested");
    }
    else if (strcmp(argv[0], "-F") == 0 || strcmp(argv[0], "--format") == 0)
    {
      if (argc < 2)
        usage("missing argument for --format");
      argc--;
      argv++;
      format = argv[0];
    }
    else if (strcmp(argv[0], "--no-images") == 0)
    {
      noImages = true;
    }
    else if (strcmp(argv[0], "--no-points") == 0)
    {
      noPoints = true;
    }
    else
    {
      usage(string("unknown option: ") + argv[0]);
    }
  }

  if (argc < 1)
    usage("missing e57 file argument");

  src = argv[0];
  argc--;
  argv++;

  if (argc >= 1)
    dst = argv[0];
}

void print_version()
{
  cout << PROGRAM_NAME " (" PLATFORM_NAME ") " << PROGRAM_VERSION << "." << '\n';

  int     astmMajor;
  int     astmMinor;
  ustring libraryId;
  E57Utilities().getVersions(astmMajor, astmMinor, libraryId);

  cout << "E57 API " << astmMajor << "." << astmMinor << endl;
  cout << libraryId << endl;
}

void print_help()
{
  cout << "Usage:\n"
       << PROGRAM_NAME " [options] e57-file [unpack-directory]\n"
       << "  The purpose of this program is to break the e57 file into\n"
       << "  parts.\n"
       << endl
       << "  -F, --format <string>  format string for point data" << endl
       << "  --no-images            suppress image output" << endl
       << "  --no-points            suppress pointcloud output" << endl
       << "  -v, --version          show version" << endl
       << "  -h, --help             show help" << endl;
}

int main(int argc, char* argv[])
{
  try
  {
    CommandLineOptions options;
    options.parse(argc, argv);

    if (options.src.empty())
    {
      print_help();
      return -1;
    }

    string     fmt = options.format;
    ImageFile  imf(options.src, "r");
    StructureNode root = imf.root();

    fs::path dst;
    if (!options.dst.empty())
      dst = fs::path(options.dst);
    else
    {
      dst = fs::path(options.src);
      dst.replace_extension();
    }

    fs::create_directories(dst);

    ofstream root_inf(dst / "root.inf");
    root_inf << "formatName = " << StringNode(root.get("formatName")).value() << endl;
    root_inf << "guid = " << StringNode(root.get("guid")).value() << endl;
    root_inf << "versionMajor = " << IntegerNode(root.get("versionMajor")).value() << endl;
    root_inf << "versionMinor = " << IntegerNode(root.get("versionMinor")).value() << endl;
    if (root.isDefined("e57LibraryVersion"))
    {
      root_inf << "e57LibraryVersion = " << StringNode(root.get("e57LibraryVersion")).value() << endl;
    }
    if (root.isDefined("coordinateMetadata"))
    {
      root_inf << "coordinateMetadata = " << StringNode(root.get("coordinateMetadata")).value() << endl;
    }
    if (root.isDefined("creationDateTime"))
    {
      StructureNode t(root.get("creationDateTime"));
      {
        ostringstream oss;
        oss << setprecision(15) << FloatNode(t.get("dateTimeValue")).value();
        root_inf << "creationDateTime.dateTimeValue = " << oss.str() << '\n';
      }
      if (t.isDefined("isAtomicClockReferenced"))
      {
        root_inf << "creationDateTime.isAtomicClockReferenced = " << IntegerNode(t.get("isAtomicClockReferenced")).value() << endl;
      }
    }
    root_inf.close();

    if (root.isDefined("data3D") && !options.noPoints)
    {
      VectorNode data3D(root.get("data3D"));
      for (int64_t child = 0; child < data3D.childCount(); ++child)
      {
        StructureNode                                                     scan(data3D.get(child));
        CompressedVectorNode                                              points(scan.get("points"));
        StructureNode                                                     prototype(points.prototype());
        vector<SourceDestBuffer>                                          sdb;
        const size_t                                                      buf_size = 1024;
        vector<variant<vector<double>, vector<int64_t>, vector<ustring>>> buf;
        string                                                            pointrecord;

        string comma;
        if (options.format.empty())
        {
          fmt.clear();
          for (int64_t i = 0; i < prototype.childCount(); ++i)
          {
            switch (prototype.get(i).type())
            {
            case e57::E57_FLOAT:
            case e57::E57_SCALED_INTEGER:
              buf.push_back(vector<double>(buf_size));
              if (options.format.empty())
                fmt += comma + "%." + to_string(numeric_limits<double>::digits10) + "g";
              break;
            case e57::E57_INTEGER:
              buf.push_back(vector<int64_t>(buf_size));
              if (options.format.empty())
                fmt += comma + "%d";
              break;
            case e57::E57_STRING:
              buf.push_back(vector<ustring>(buf_size));
              if (options.format.empty())
                fmt += comma + "%s";
              break;
            case e57::E57_STRUCTURE:
            case e57::E57_VECTOR:
            case e57::E57_COMPRESSED_VECTOR:
            case e57::E57_BLOB:
            default:
              break;
            }
            if (comma.empty())
              comma = ",";
          }
        }
        comma.clear();
        for (int i = 0; i < prototype.childCount(); ++i)
        {
          Node n(prototype.get(i));
          pointrecord += comma + n.elementName();
          if (comma.empty())
            comma = ",";
          switch (n.type())
          {
          case e57::E57_FLOAT:
          case e57::E57_SCALED_INTEGER:
            sdb.push_back(SourceDestBuffer(imf, n.elementName(), &get<vector<double>>(buf[i])[0], buf_size, true, true));
            break;
          case e57::E57_INTEGER:
            sdb.push_back(SourceDestBuffer(imf, n.elementName(), &get<vector<int64_t>>(buf[i])[0], buf_size, true, true));
            break;
          case e57::E57_STRING:
            sdb.push_back(SourceDestBuffer(imf, n.elementName(), &get<vector<ustring>>(buf[i])));
            break;
          default:
            throw(runtime_error("prototype contains illegal type"));
          }
        }

        ofstream inf(dst / fs::path(string("image3d-") + to_string(child) + ".inf"));
        inf << "pointrecord = " << pointrecord << endl; // can be used as a header line for the csv file
        inf << "pointrecord.format = " << fmt << endl;
        if (scan.isDefined("name"))
        {
          inf << "name = " << StringNode(scan.get("name")).value() << endl;
        }
        inf << "guid = " << StringNode(scan.get("guid")).value() << endl;
        if (scan.isDefined("description"))
        {
          inf << "description = " << StringNode(scan.get("description")).value() << endl;
        }
        if (scan.isDefined("pose"))
        {
          StructureNode pose(scan.get("pose"));
          StructureNode translation(pose.get("translation"));
          StructureNode rotation(pose.get("rotation"));
          inf << "pose.translation.x = " << FloatNode(translation.get("x")).value() << endl;
          inf << "pose.translation.y = " << FloatNode(translation.get("y")).value() << endl;
          inf << "pose.translation.z = " << FloatNode(translation.get("z")).value() << endl;
          inf << "pose.rotation.w = " << FloatNode(rotation.get("w")).value() << endl;
          inf << "pose.rotation.x = " << FloatNode(rotation.get("x")).value() << endl;
          inf << "pose.rotation.y = " << FloatNode(rotation.get("y")).value() << endl;
          inf << "pose.rotation.z = " << FloatNode(rotation.get("z")).value() << endl;
        }
        if (scan.isDefined("acquisitionStart"))
        {
          StructureNode t(scan.get("acquisitionStart"));
          {
            ostringstream oss;
            oss << setprecision(15) << FloatNode(t.get("dateTimeValue")).value();
            inf << "acquisitionStart.dateTimeValue = " << oss.str() << '\n';
          }
          if (t.isDefined("isAtomicClockReferenced"))
          {
            inf << "acquisitionStart.isAtomicClockReferenced = " << IntegerNode(t.get("isAtomicClockReferenced")).value() << endl;
          }
        }
        if (scan.isDefined("acquisitionEnd"))
        {
          StructureNode t(scan.get("acquisitionEnd"));
          {
            ostringstream oss;
            oss << setprecision(15) << FloatNode(t.get("dateTimeValue")).value();
            inf << "acquisitionEnd.dateTimeValue = " << oss.str() << '\n';
          }
          if (t.isDefined("isAtomicClockReferenced"))
          {
            inf << "acquisitionEnd.isAtomicClockReferenced = " << IntegerNode(t.get("isAtomicClockReferenced")).value() << endl;
          }
        }
        if (scan.isDefined("sensorVendor"))
        {
          inf << "sensorVendor = " << StringNode(scan.get("sensorVendor")).value() << endl;
        }
        if (scan.isDefined("sensorModel"))
        {
          inf << "sensorModel = " << StringNode(scan.get("sensorModel")).value() << endl;
        }
        if (scan.isDefined("sensorSerialNumber"))
        {
          inf << "sensorSerialNumber = " << StringNode(scan.get("sensorSerialNumber")).value() << endl;
        }
        if (scan.isDefined("HardwareVersion"))
        {
          inf << "HardwareVersion = " << StringNode(scan.get("HardwareVersion")).value() << endl;
        }
        if (scan.isDefined("SoftwareVersion"))
        {
          inf << "SoftwareVersion = " << StringNode(scan.get("SoftwareVersion")).value() << endl;
        }
        if (scan.isDefined("FirmwareVersion"))
        {
          inf << "FirmwareVersion = " << StringNode(scan.get("FirmwareVersion")).value() << endl;
        }
        if (scan.isDefined("temperature"))
        {
          inf << "temperature = " << FloatNode(scan.get("temperature")).value() << endl;
        }
        if (scan.isDefined("relativeHumidity"))
        {
          inf << "relativeHumidity = " << FloatNode(scan.get("relativeHumidity")).value() << endl;
        }
        if (scan.isDefined("atmosphericPressure"))
        {
          inf << "atmosphericPressure = " << FloatNode(scan.get("atmosphericPressure")).value() << endl;
        }
        inf.close();

        CompressedVectorReader rd(points.reader(sdb));
        fs::path               csvname(string("image3d-") + to_string(child) + ".csv");
        ofstream               ocsv(dst / csvname);
        ostream&               out(ocsv); // needed to fix ambiguity for << operator on msvc
        cout << "unpacking: " << dst / csvname << " ... ";
        unsigned count;
        uint64_t total_count(0);

        vector<string> field_formats;
        {
          string        current;
          bool          in_spec = false;
          for (char c : fmt)
          {
            if (c == '%')
            {
              if (!current.empty())
                field_formats.push_back(current);
              current = "%";
              in_spec = true;
            }
            else if (in_spec)
            {
              current += c;
              if (c == 'd' || c == 'i' || c == 'g' || c == 'G' || c == 'e' || c == 'E' || c == 'f' || c == 's')
              {
                field_formats.push_back(current);
                current.clear();
                in_spec = false;
              }
            }
            else
            {
              field_formats.push_back(string(1, c));
            }
          }
          if (!current.empty())
            field_formats.push_back(current);
        }

        out << pointrecord << endl;
        while ((count = rd.read()))
        {
          total_count += count;
          for (size_t i = 0; i < count; ++i)
          {
            size_t fmt_idx = 0;
            for (size_t j = 0; j < buf.size(); ++j)
            {
              while (fmt_idx < field_formats.size() && field_formats[fmt_idx][0] != '%')
              {
                out << field_formats[fmt_idx];
                fmt_idx++;
              }
              if (fmt_idx < field_formats.size())
              {
                visit(
                  [&](auto& vec) {
                    using T = std::decay_t<decltype(vec[0])>;
                    if constexpr (std::is_same_v<T, double>)
                    {
                      ostringstream oss;
                      const string& spec = field_formats[fmt_idx];
                      if (spec.size() > 2 && spec[1] == '.')
                      {
                        size_t prec_end = spec.find_first_of("gGeEf");
                        string prec_str = spec.substr(2, prec_end - 2);
                        int    prec = 0;
                        for (char c : prec_str)
                          if (c >= '0' && c <= '9')
                            prec = prec * 10 + (c - '0');
                        oss << setprecision(prec);
                      }
                      oss << vec[i];
                      out << oss.str();
                    }
                    else if constexpr (std::is_same_v<T, int64_t>)
                    {
                      out << vec[i];
                    }
                    else
                    {
                      out << vec[i];
                    }
                  },
                  buf.at(j));
                fmt_idx++;
              }
            }
            while (fmt_idx < field_formats.size())
            {
              out << field_formats[fmt_idx];
              fmt_idx++;
            }
            out << endl;
          }
        }
        cout << " total points: " << total_count << endl;

        ocsv.close();
      }
    }

    if (root.isDefined("images2D") && !options.noImages)
    {
      VectorNode images2D(root.get("images2D"));
      for (int64_t child = 0; child < images2D.childCount(); ++child)
      {
        StructureNode image(images2D.get(child));
        fs::path      inf_path = dst / fs::path(string("image2d-") + to_string(child) + ".inf");
        ofstream      inf(inf_path);
        string        reptype;
        if (image.isDefined("visualReferenceRepresentation"))
          reptype = "visualReferenceRepresentation";
        else if (image.isDefined("pinholeRepresentation"))
          reptype = "pinholeRepresentation";
        else if (image.isDefined("sphericalRepresentation"))
          reptype = "sphericalRepresentation";
        else if (image.isDefined("cylindricalRepresentation"))
          reptype = "cylindricalRepresentation";
        StructureNode rep(image.get(reptype));
        string        imgtype;
        fs::path      img_path(dst / fs::path(string("image2d-") + to_string(child) + ".img"));
        if (rep.isDefined("jpegImage"))
        {
          img_path.replace_extension(".jpg");
          imgtype = "jpegImage";
        }
        else if (rep.isDefined("pngImage"))
        {
          img_path.replace_extension(".png");
          imgtype = "pngImage";
        }
        BlobNode         blob(rep.get(imgtype));
        ofstream         img(img_path, ios_base::out | ios_base::binary);
        const streamsize buffer_size = 1024 * 1024;
        vector<uint8_t>  buffer(buffer_size);
        int64_t          offset    = 0;
        int64_t          remaining = blob.byteCount();
        size_t           get;
        cout << "unpacking: " << img_path << " ... ";
        while (img && remaining > 0)
        {
          get = (remaining > buffer_size) ? buffer_size : remaining;
          blob.read(&buffer[0], offset, get);
          img.write(reinterpret_cast<char*>(&buffer[0]), get);
          offset += get;
          remaining -= get;
        }
        img.close();
        inf << "guid = " << StringNode(image.get("guid")).value() << endl;
        if (image.isDefined("name"))
        {
          inf << "name = " << StringNode(image.get("name")).value() << endl;
        }
        if (image.isDefined("description"))
        {
          inf << "description = " << StringNode(image.get("description")).value() << endl;
        }
        if (image.isDefined("sensorVendor"))
        {
          inf << "sensorVendor = " << StringNode(image.get("sensorVendor")).value() << endl;
        }
        if (image.isDefined("sensorModel"))
        {
          inf << "sensorModel = " << StringNode(image.get("sensorModel")).value() << endl;
        }
        if (image.isDefined("sensorSerialNumber"))
        {
          inf << "sensorSerialNumber = " << StringNode(image.get("sensorSerialNumber")).value() << endl;
        }
        if (image.isDefined("pose"))
        {
          StructureNode pose(image.get("pose"));
          StructureNode translation(pose.get("translation"));
          StructureNode rotation(pose.get("rotation"));
          inf << "pose.translation.x = " << FloatNode(translation.get("x")).value() << endl;
          inf << "pose.translation.y = " << FloatNode(translation.get("y")).value() << endl;
          inf << "pose.translation.z = " << FloatNode(translation.get("z")).value() << endl;
          inf << "pose.rotation.w = " << FloatNode(rotation.get("w")).value() << endl;
          inf << "pose.rotation.x = " << FloatNode(rotation.get("x")).value() << endl;
          inf << "pose.rotation.y = " << FloatNode(rotation.get("y")).value() << endl;
          inf << "pose.rotation.z = " << FloatNode(rotation.get("z")).value() << endl;
        }
        if (rep.isDefined("imageHeight"))
        {
          inf << reptype + ".imageHeight = " << IntegerNode(rep.get("imageHeight")).value() << endl;
          cout << IntegerNode(rep.get("imageHeight")).value() << " x ";
        }
        if (rep.isDefined("imageWidth"))
        {
          inf << reptype + ".imageWidth = " << IntegerNode(rep.get("imageWidth")).value() << endl;
          cout << IntegerNode(rep.get("imageWidth")).value() << " pixels" << endl;
        }
        if (rep.isDefined("focalLength"))
        {
          inf << reptype + ".focalLength = " << FloatNode(rep.get("focalLength")).value() << endl;
        }
        if (rep.isDefined("pixelWidth"))
        {
          inf << reptype + ".pixelWidth = " << FloatNode(rep.get("pixelWidth")).value() << endl;
        }
        if (rep.isDefined("pixelHeight"))
        {
          inf << reptype + ".pixelHeight = " << FloatNode(rep.get("pixelHeight")).value() << endl;
        }
        if (rep.isDefined("principalPointX"))
        {
          inf << reptype + ".principalPointX = " << FloatNode(rep.get("principalPointX")).value() << endl;
        }
        if (rep.isDefined("principalPointY"))
        {
          inf << reptype + ".principalPointY = " << FloatNode(rep.get("principalPointY")).value() << endl;
        }
        if (rep.isDefined("radius"))
        {
          inf << reptype + ".radius = " << FloatNode(rep.get("radius")).value() << endl;
        }

        inf.close();
      }
    }

    return 0;
  }
  catch (E57Exception& e)
  {
    e.report(__FILE__, __LINE__, __FUNCTION__);
    return -1;
  }
  catch (exception& e)
  {
    cerr << e.what() << endl;
    return -1;
  }
  catch (...)
  {
    cerr << "unknown exception" << endl;
    return -1;
  }
  return 0;
}
