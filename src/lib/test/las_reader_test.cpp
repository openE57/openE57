#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "test_utils.h"

#include <openE57/LAS/openE57las.h>

#include <cstdint>
#include <fstream>

using namespace e57;
using e57::test::TempFile;

namespace
{

void writeMinimalLAS10File(const std::string& filename)
{
  std::ofstream ofs(filename, std::ios::binary);

  char signature[4] = {'L', 'A', 'S', 'F'};
  ofs.write(signature, 4);

  uint16_t fileSourceId = 0;
  ofs.write(reinterpret_cast<char*>(&fileSourceId), 2);

  uint16_t globalEncoding = 0;
  ofs.write(reinterpret_cast<char*>(&globalEncoding), 2);

  uint32_t projectIdGuidData1 = 0;
  ofs.write(reinterpret_cast<char*>(&projectIdGuidData1), 4);
  uint16_t projectIdGuidData2 = 0;
  ofs.write(reinterpret_cast<char*>(&projectIdGuidData2), 2);
  uint16_t projectIdGuidData3 = 0;
  ofs.write(reinterpret_cast<char*>(&projectIdGuidData3), 2);
  uint8_t projectIdGuidData4[8] = {0};
  ofs.write(reinterpret_cast<char*>(projectIdGuidData4), 8);

  uint8_t versionMajor = 1;
  uint8_t versionMinor = 0;
  ofs.write(reinterpret_cast<char*>(&versionMajor), 1);
  ofs.write(reinterpret_cast<char*>(&versionMinor), 1);

  char systemIdentifier[32] = {0};
  ofs.write(systemIdentifier, 32);

  char generatingSoftware[32] = {0};
  ofs.write(generatingSoftware, 32);

  uint16_t fileCreationDayOfYear = 1;
  uint16_t fileCreationYear      = 2024;
  ofs.write(reinterpret_cast<char*>(&fileCreationDayOfYear), 2);
  ofs.write(reinterpret_cast<char*>(&fileCreationYear), 2);

  uint16_t headerSize = 227;
  ofs.write(reinterpret_cast<char*>(&headerSize), 2);

  uint32_t offsetToPointData = 227;
  ofs.write(reinterpret_cast<char*>(&offsetToPointData), 4);

  uint32_t numberOfVariableLengthRecords = 0;
  ofs.write(reinterpret_cast<char*>(&numberOfVariableLengthRecords), 4);

  uint8_t pointDataFormatId = 0;
  ofs.write(reinterpret_cast<char*>(&pointDataFormatId), 1);

  uint16_t pointDataRecordLength = 20;
  ofs.write(reinterpret_cast<char*>(&pointDataRecordLength), 2);

  uint32_t numberOfPointRecords = 0;
  ofs.write(reinterpret_cast<char*>(&numberOfPointRecords), 4);

  uint32_t numberOfPointsByReturn[7] = {0};
  ofs.write(reinterpret_cast<char*>(numberOfPointsByReturn), 28);

  double xScaleFactor = 0.001;
  double yScaleFactor = 0.001;
  double zScaleFactor = 0.001;
  ofs.write(reinterpret_cast<char*>(&xScaleFactor), 8);
  ofs.write(reinterpret_cast<char*>(&yScaleFactor), 8);
  ofs.write(reinterpret_cast<char*>(&zScaleFactor), 8);

  double xOffset = 0.0;
  double yOffset = 0.0;
  double zOffset = 0.0;
  ofs.write(reinterpret_cast<char*>(&xOffset), 8);
  ofs.write(reinterpret_cast<char*>(&yOffset), 8);
  ofs.write(reinterpret_cast<char*>(&zOffset), 8);

  double maxX = 100.0;
  double minX = 0.0;
  double maxY = 100.0;
  double minY = 0.0;
  double maxZ = 100.0;
  double minZ = 0.0;
  ofs.write(reinterpret_cast<char*>(&maxX), 8);
  ofs.write(reinterpret_cast<char*>(&minX), 8);
  ofs.write(reinterpret_cast<char*>(&maxY), 8);
  ofs.write(reinterpret_cast<char*>(&minY), 8);
  ofs.write(reinterpret_cast<char*>(&maxZ), 8);
  ofs.write(reinterpret_cast<char*>(&minZ), 8);

  ofs.close();
}

void writeInvalidLASFile(const std::string& filename)
{
  std::ofstream ofs(filename, std::ios::binary);
  char          signature[4] = {'B', 'A', 'D', 'F'};
  ofs.write(signature, 4);
  ofs.close();
}
} // namespace

TEST_SUITE("LASReader Header Tests")
{
  TEST_CASE("LASReader reads valid LAS 1.0 header")
  {
    TempFile tempFile{".las"};
    writeMinimalLAS10File(tempFile.string());

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);

    REQUIRE_EQ(header.fileSignature[0], 'L');
    REQUIRE_EQ(header.fileSignature[1], 'A');
    REQUIRE_EQ(header.fileSignature[2], 'S');
    REQUIRE_EQ(header.fileSignature[3], 'F');
    REQUIRE_EQ(header.versionMajor, 1);
    REQUIRE_EQ(header.versionMinor, 0);
  }

  TEST_CASE("LASReader throws exception for non-LAS file")
  {
    TempFile tempFile{".las"};
    writeInvalidLASFile(tempFile.string());

    REQUIRE_THROWS_AS(LASReader(tempFile.c_str()), std::exception);
  }

  TEST_CASE("LASReader throws exception for nonexistent file")
  {
    REQUIRE_THROWS_AS(LASReader("/nonexistent/path/to/file.las"), std::exception);
  }
}

TEST_SUITE("LASPublicHeaderBlock Tests")
{
  TEST_CASE("LASPublicHeaderBlock default initialization")
  {
    LASPublicHeaderBlock header;
    REQUIRE_EQ(header.versionMajor, 0);
    REQUIRE_EQ(header.versionMinor, 0);
  }

  TEST_CASE("LASPublicHeaderBlock dump produces output")
  {
    LASPublicHeaderBlock header;
    std::stringstream    ss;
    header.dump(0, ss);
    REQUIRE(ss.str().length() > 0);
  }
}

TEST_SUITE("LASPointDataRecord Tests")
{
  TEST_CASE("LASPointDataRecord default initialization")
  {
    LASPointDataRecord point;
    REQUIRE_EQ(point.x, 0);
    REQUIRE_EQ(point.y, 0);
    REQUIRE_EQ(point.z, 0);
    REQUIRE_EQ(point.intensity, 0);
    REQUIRE_EQ(point.returnNumber, 0);
    REQUIRE_EQ(point.numberOfReturns, 0);
    REQUIRE_EQ(point.scanDirectionFlag, false);
    REQUIRE_EQ(point.edgeOfFlightLine, false);
  }

  TEST_CASE("LASPointDataRecord dump produces output")
  {
    LASPointDataRecord point;
    std::stringstream  ss;
    point.dump(0, ss);
    REQUIRE(ss.str().length() > 0);
  }
}

TEST_SUITE("LASVariableLengthRecord Tests")
{
  TEST_CASE("LASVariableLengthRecord dump produces output")
  {
    LASVariableLengthRecord vlr;
    std::stringstream       ss;
    vlr.dump(0, ss);
    REQUIRE(ss.str().length() > 0);
  }
}
