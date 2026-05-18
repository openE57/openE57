#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "test_utils.h"

#include <openE57/LAS/openE57las.h>

#include <cstdint>
#include <fstream>

using namespace e57;
using e57::test::TempFile;

namespace
{

void writeLASHeader(std::ofstream& ofs, uint8_t versionMinor, uint16_t globalEncoding = 0)
{
  char signature[4] = {'L', 'A', 'S', 'F'};
  ofs.write(signature, 4);

  uint16_t fileSourceId = 123;
  ofs.write(reinterpret_cast<char*>(&fileSourceId), 2);

  ofs.write(reinterpret_cast<char*>(&globalEncoding), 2);

  uint32_t projectIdGuidData1 = 0x12345678;
  ofs.write(reinterpret_cast<char*>(&projectIdGuidData1), 4);
  uint16_t projectIdGuidData2 = 0x1234;
  ofs.write(reinterpret_cast<char*>(&projectIdGuidData2), 2);
  uint16_t projectIdGuidData3 = 0x5678;
  ofs.write(reinterpret_cast<char*>(&projectIdGuidData3), 2);
  uint8_t projectIdGuidData4[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
  ofs.write(reinterpret_cast<char*>(projectIdGuidData4), 8);

  uint8_t versionMajor = 1;
  ofs.write(reinterpret_cast<char*>(&versionMajor), 1);
  ofs.write(reinterpret_cast<char*>(&versionMinor), 1);

  char systemIdentifier[32] = "TestSystem";
  ofs.write(systemIdentifier, 32);

  char generatingSoftware[32] = "TestSoftware";
  ofs.write(generatingSoftware, 32);

  uint16_t fileCreationDayOfYear = 100;
  uint16_t fileCreationYear      = 2024;
  ofs.write(reinterpret_cast<char*>(&fileCreationDayOfYear), 2);
  ofs.write(reinterpret_cast<char*>(&fileCreationYear), 2);

  uint16_t headerSize = (versionMinor >= 3) ? 235 : 227;
  ofs.write(reinterpret_cast<char*>(&headerSize), 2);

  uint32_t offsetToPointData = headerSize;
  ofs.write(reinterpret_cast<char*>(&offsetToPointData), 4);

  uint32_t numberOfVariableLengthRecords = 0;
  ofs.write(reinterpret_cast<char*>(&numberOfVariableLengthRecords), 4);

  uint8_t pointDataFormatId = 0;
  ofs.write(reinterpret_cast<char*>(&pointDataFormatId), 1);

  uint16_t pointDataRecordLength = 20;
  ofs.write(reinterpret_cast<char*>(&pointDataRecordLength), 2);

  uint32_t numberOfPointRecords = 0;
  ofs.write(reinterpret_cast<char*>(&numberOfPointRecords), 4);

  if (versionMinor <= 2)
  {
    uint32_t numberOfPointsByReturn[5] = {0};
    ofs.write(reinterpret_cast<char*>(numberOfPointsByReturn), 20);
  }
  else
  {
    uint32_t numberOfPointsByReturn[7] = {0};
    ofs.write(reinterpret_cast<char*>(numberOfPointsByReturn), 28);
  }

  double xScaleFactor = 0.001;
  double yScaleFactor = 0.001;
  double zScaleFactor = 0.001;
  ofs.write(reinterpret_cast<char*>(&xScaleFactor), 8);
  ofs.write(reinterpret_cast<char*>(&yScaleFactor), 8);
  ofs.write(reinterpret_cast<char*>(&zScaleFactor), 8);

  double xOffset = 100.0;
  double yOffset = 200.0;
  double zOffset = 50.0;
  ofs.write(reinterpret_cast<char*>(&xOffset), 8);
  ofs.write(reinterpret_cast<char*>(&yOffset), 8);
  ofs.write(reinterpret_cast<char*>(&zOffset), 8);

  double maxX = 1000.0;
  double minX = 0.0;
  double maxY = 1000.0;
  double minY = 0.0;
  double maxZ = 100.0;
  double minZ = 0.0;
  ofs.write(reinterpret_cast<char*>(&maxX), 8);
  ofs.write(reinterpret_cast<char*>(&minX), 8);
  ofs.write(reinterpret_cast<char*>(&maxY), 8);
  ofs.write(reinterpret_cast<char*>(&minY), 8);
  ofs.write(reinterpret_cast<char*>(&maxZ), 8);
  ofs.write(reinterpret_cast<char*>(&minZ), 8);

  if (versionMinor >= 3)
  {
    uint64_t startOfWaveformDataPacketRecord = 0;
    ofs.write(reinterpret_cast<char*>(&startOfWaveformDataPacketRecord), 8);
  }
}

void writeLASFileWithPoints(const std::string& filename, uint8_t pointFormatId, uint32_t numPoints)
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
  uint8_t versionMinor = 2;
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

  uint16_t headerSize         = 227;
  uint32_t offsetToPointData  = 227;
  uint32_t numVLRs            = 0;
  uint16_t pointDataRecordLen = 20;

  switch (pointFormatId)
  {
  case 0:
    pointDataRecordLen = 20;
    break;
  case 1:
    pointDataRecordLen = 28;
    break;
  case 2:
    pointDataRecordLen = 26;
    break;
  case 3:
    pointDataRecordLen = 34;
    break;
  case 4:
    pointDataRecordLen = 57;
    break;
  case 5:
    pointDataRecordLen = 63;
    break;
  }

  ofs.write(reinterpret_cast<char*>(&headerSize), 2);
  ofs.write(reinterpret_cast<char*>(&offsetToPointData), 4);
  ofs.write(reinterpret_cast<char*>(&numVLRs), 4);
  ofs.write(reinterpret_cast<char*>(&pointFormatId), 1);
  ofs.write(reinterpret_cast<char*>(&pointDataRecordLen), 2);
  ofs.write(reinterpret_cast<char*>(&numPoints), 4);

  uint32_t numberOfPointsByReturn[5] = {0};
  ofs.write(reinterpret_cast<char*>(numberOfPointsByReturn), 20);

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

  for (uint32_t i = 0; i < numPoints; i++)
  {
    int32_t  x              = static_cast<int32_t>(i * 100);
    int32_t  y              = static_cast<int32_t>(i * 200);
    int32_t  z              = static_cast<int32_t>(i * 10);
    uint16_t intensity      = static_cast<uint16_t>(i * 10);
    uint8_t  flags          = 0;
    uint8_t  classification = static_cast<uint8_t>(i % 32);
    int8_t   scanAngleRank  = static_cast<int8_t>(i % 90 - 45);
    uint8_t  userData       = static_cast<uint8_t>(i);
    uint16_t pointSourceId  = 1;

    ofs.write(reinterpret_cast<char*>(&x), 4);
    ofs.write(reinterpret_cast<char*>(&y), 4);
    ofs.write(reinterpret_cast<char*>(&z), 4);
    ofs.write(reinterpret_cast<char*>(&intensity), 2);
    ofs.write(reinterpret_cast<char*>(&flags), 1);
    ofs.write(reinterpret_cast<char*>(&classification), 1);
    ofs.write(reinterpret_cast<char*>(&scanAngleRank), 1);
    ofs.write(reinterpret_cast<char*>(&userData), 1);
    ofs.write(reinterpret_cast<char*>(&pointSourceId), 2);

    if (pointFormatId == 1 || pointFormatId == 3 || pointFormatId == 4 || pointFormatId == 5)
    {
      double gpsTime = static_cast<double>(i) * 0.001;
      ofs.write(reinterpret_cast<char*>(&gpsTime), 8);
    }

    if (pointFormatId == 2 || pointFormatId == 3 || pointFormatId == 5)
    {
      uint16_t red   = static_cast<uint16_t>(i * 10);
      uint16_t green = static_cast<uint16_t>(i * 20);
      uint16_t blue  = static_cast<uint16_t>(i * 30);
      ofs.write(reinterpret_cast<char*>(&red), 2);
      ofs.write(reinterpret_cast<char*>(&green), 2);
      ofs.write(reinterpret_cast<char*>(&blue), 2);
    }

    if (pointFormatId == 4 || pointFormatId == 5)
    {
      uint8_t  wavePacketDescriptorIndex   = 1;
      uint64_t byteOffsetToWaveformData    = 0;
      uint32_t waveformPacketSizeInBytes   = 0;
      float    returnPointWaveformLocation = 0.0f;
      float    xT = 0.0f, yT = 0.0f, zT = 0.0f;

      ofs.write(reinterpret_cast<char*>(&wavePacketDescriptorIndex), 1);
      ofs.write(reinterpret_cast<char*>(&byteOffsetToWaveformData), 8);
      ofs.write(reinterpret_cast<char*>(&waveformPacketSizeInBytes), 4);
      ofs.write(reinterpret_cast<char*>(&returnPointWaveformLocation), 4);
      ofs.write(reinterpret_cast<char*>(&xT), 4);
      ofs.write(reinterpret_cast<char*>(&yT), 4);
      ofs.write(reinterpret_cast<char*>(&zT), 4);
    }
  }

  ofs.close();
}

void writeLASFileWithVLRs(const std::string& filename)
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
  uint8_t versionMinor = 2;
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

  uint16_t headerSize         = 227;
  uint32_t offsetToPointData  = 227 + 54 + 10;
  uint32_t numVLRs            = 1;
  uint8_t  pointDataFormatId  = 0;
  uint16_t pointDataRecordLen = 20;
  uint32_t numPoints          = 0;

  ofs.write(reinterpret_cast<char*>(&headerSize), 2);
  ofs.write(reinterpret_cast<char*>(&offsetToPointData), 4);
  ofs.write(reinterpret_cast<char*>(&numVLRs), 4);
  ofs.write(reinterpret_cast<char*>(&pointDataFormatId), 1);
  ofs.write(reinterpret_cast<char*>(&pointDataRecordLen), 2);
  ofs.write(reinterpret_cast<char*>(&numPoints), 4);

  uint32_t numberOfPointsByReturn[5] = {0};
  ofs.write(reinterpret_cast<char*>(numberOfPointsByReturn), 20);

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

  uint16_t reserved                = 0;
  char     userId[16]              = "TestVLR";
  uint16_t recordId                = 1;
  uint16_t recordLengthAfterHeader = 10;
  char     description[32]         = "Test VLR Description";
  char     vlrData[10]             = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  ofs.write(reinterpret_cast<char*>(&reserved), 2);
  ofs.write(userId, 16);
  ofs.write(reinterpret_cast<char*>(&recordId), 2);
  ofs.write(reinterpret_cast<char*>(&recordLengthAfterHeader), 2);
  ofs.write(description, 32);
  ofs.write(vlrData, 10);

  ofs.close();
}
} // namespace

TEST_SUITE("LASReader Version Tests")
{
  TEST_CASE("LASReader reads LAS 1.1 header")
  {
    TempFile tempFile{".las"};
    {
      std::ofstream ofs(tempFile.string(), std::ios::binary);
      writeLASHeader(ofs, 1);
    }

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);

    REQUIRE_EQ(header.versionMajor, 1);
    REQUIRE_EQ(header.versionMinor, 1);
    REQUIRE_EQ(header.fileSourceId, 123);
  }

  TEST_CASE("LASReader reads LAS 1.2 header")
  {
    TempFile tempFile{".las"};
    {
      std::ofstream ofs(tempFile.string(), std::ios::binary);
      writeLASHeader(ofs, 2);
    }

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);

    REQUIRE_EQ(header.versionMajor, 1);
    REQUIRE_EQ(header.versionMinor, 2);
  }

  TEST_CASE("LASReader reads LAS 1.3 header with waveform field")
  {
    TempFile tempFile{".las"};
    {
      std::ofstream ofs(tempFile.string(), std::ios::binary);
      writeLASHeader(ofs, 3);
    }

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);

    REQUIRE_EQ(header.versionMajor, 1);
    REQUIRE_EQ(header.versionMinor, 3);
    REQUIRE_EQ(header.headerSize, 235);
  }
}

TEST_SUITE("LASReader Global Encoding Tests")
{
  TEST_CASE("LASReader reads GPS time type from global encoding")
  {
    TempFile tempFile{".las"};
    {
      std::ofstream ofs(tempFile.string(), std::ios::binary);
      writeLASHeader(ofs, 2, 0x0001);
    }

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);

    REQUIRE_EQ(header.gpsTimeType, 1);
  }

  TEST_CASE("LASReader reads waveform flags from global encoding")
  {
    TempFile tempFile{".las"};
    {
      std::ofstream ofs(tempFile.string(), std::ios::binary);
      writeLASHeader(ofs, 3, 0x000E);
    }

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);

    REQUIRE_EQ(header.waveformDataPacketsInternal, true);
    REQUIRE_EQ(header.waveformDataPacketsExternal, true);
    REQUIRE_EQ(header.returnNumbersSyntheticallyGenerated, true);
  }
}

TEST_SUITE("LASReader Point Reading Tests")
{
  TEST_CASE("LASReader readPoints with format 0")
  {
    TempFile tempFile{".las"};
    writeLASFileWithPoints(tempFile.string(), 0, 5);

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);
    REQUIRE_EQ(header.numberOfPointRecords, 5);

    std::vector<LASPointDataRecord> points(5);
    int                             read = reader.readPoints(points.data(), 0, 5);
    REQUIRE_EQ(read, 5);

    REQUIRE_EQ(points[0].x, 0);
    REQUIRE_EQ(points[1].x, 100);
    REQUIRE_EQ(points[2].x, 200);
  }

  TEST_CASE("LASReader readPoints with format 1 (GPS time)")
  {
    TempFile tempFile{".las"};
    writeLASFileWithPoints(tempFile.string(), 1, 3);

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);
    REQUIRE_EQ(header.pointDataFormatId, 1);
    REQUIRE_EQ(header.pointDataRecordLength, 28);

    std::vector<LASPointDataRecord> points(3);
    int                             read = reader.readPoints(points.data(), 0, 3);
    REQUIRE_EQ(read, 3);
  }

  TEST_CASE("LASReader readPoints with format 2 (RGB)")
  {
    TempFile tempFile{".las"};
    writeLASFileWithPoints(tempFile.string(), 2, 3);

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);
    REQUIRE_EQ(header.pointDataFormatId, 2);
    REQUIRE_EQ(header.pointDataRecordLength, 26);

    std::vector<LASPointDataRecord> points(3);
    int                             read = reader.readPoints(points.data(), 0, 3);
    REQUIRE_EQ(read, 3);
  }

  TEST_CASE("LASReader readPoints with format 3 (GPS+RGB)")
  {
    TempFile tempFile{".las"};
    writeLASFileWithPoints(tempFile.string(), 3, 3);

    LASReader reader(tempFile.c_str());

    LASPublicHeaderBlock header;
    reader.getHeader(header);
    REQUIRE_EQ(header.pointDataFormatId, 3);
    REQUIRE_EQ(header.pointDataRecordLength, 34);

    std::vector<LASPointDataRecord> points(3);
    int                             read = reader.readPoints(points.data(), 0, 3);
    REQUIRE_EQ(read, 3);
  }

  TEST_CASE("LASReader readPoints with partial range")
  {
    TempFile tempFile{".las"};
    writeLASFileWithPoints(tempFile.string(), 0, 10);

    LASReader reader(tempFile.c_str());

    std::vector<LASPointDataRecord> points(5);
    int                             read = reader.readPoints(points.data(), 3, 5);
    REQUIRE_EQ(read, 5);
    REQUIRE_EQ(points[0].x, 300);
    REQUIRE_EQ(points[4].x, 700);
  }
}

TEST_SUITE("LASReader VLR Tests")
{
  TEST_CASE("LASReader readNextVLR returns false for no VLRs")
  {
    TempFile tempFile{".las"};
    writeLASFileWithPoints(tempFile.string(), 0, 0);

    LASReader               reader(tempFile.c_str());
    LASVariableLengthRecord vlr;
    REQUIRE_FALSE(reader.readNextVLR(vlr));
  }

  TEST_CASE("LASReader readNextVLR reads VLR")
  {
    TempFile tempFile{".las"};
    writeLASFileWithVLRs(tempFile.string());

    LASReader               reader(tempFile.c_str());
    LASVariableLengthRecord vlr;
    REQUIRE(reader.readNextVLR(vlr));
    REQUIRE_EQ(vlr.userId, "TestVLR");
    REQUIRE_EQ(vlr.recordId, 1);
    REQUIRE_EQ(vlr.recordLengthAfterHeader, 10);
  }

  TEST_CASE("LASReader rewindVLR resets VLR iterator")
  {
    TempFile tempFile{".las"};
    writeLASFileWithVLRs(tempFile.string());

    LASReader               reader(tempFile.c_str());
    LASVariableLengthRecord vlr1, vlr2;

    REQUIRE(reader.readNextVLR(vlr1));
    REQUIRE_FALSE(reader.readNextVLR(vlr2));

    reader.rewindVLR();

    REQUIRE(reader.readNextVLR(vlr2));
    REQUIRE_EQ(vlr1.userId, vlr2.userId);
  }
}

TEST_SUITE("LASReader getRawHeader Tests")
{
  TEST_CASE("LASReader getRawHeader returns correct size")
  {
    TempFile tempFile{".las"};
    writeLASFileWithPoints(tempFile.string(), 0, 0);

    LASReader reader(tempFile.c_str());

    std::vector<uint8_t> rawHeader = reader.getRawHeader();
    REQUIRE_EQ(rawHeader.size(), 227);
    REQUIRE_EQ(rawHeader[0], 'L');
    REQUIRE_EQ(rawHeader[1], 'A');
    REQUIRE_EQ(rawHeader[2], 'S');
    REQUIRE_EQ(rawHeader[3], 'F');
  }
}

TEST_SUITE("LASReader Dump Tests")
{
  TEST_CASE("LASReader dump produces output")
  {
    TempFile tempFile{".las"};
    writeLASFileWithPoints(tempFile.string(), 0, 2);

    LASReader reader(tempFile.c_str());

    std::stringstream ss;
    reader.dump(0, ss);
    REQUIRE(ss.str().length() > 0);
    REQUIRE(ss.str().find("fileSignature") != std::string::npos);
    REQUIRE(ss.str().find("numberOfPointRecords") != std::string::npos);
  }
}

TEST_SUITE("LASReader Error Tests")
{
  TEST_CASE("LASReader throws for unknown version")
  {
    TempFile tempFile{".las"};
    {
      std::ofstream ofs(tempFile.string(), std::ios::binary);

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

      uint8_t versionMajor = 2;
      uint8_t versionMinor = 0;
      ofs.write(reinterpret_cast<char*>(&versionMajor), 1);
      ofs.write(reinterpret_cast<char*>(&versionMinor), 1);
    }

    REQUIRE_THROWS_AS(LASReader(tempFile.c_str()), std::exception);
  }
}
