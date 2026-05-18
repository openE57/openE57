/*
 * simple_impl_comprehensive_test.cpp
 * Copyright (c) 2024 openE57 Contributors
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "test_utils.h"

using namespace e57;
using e57::test::TempFile;

namespace
{

// Minimal JPEG for testing (1x1 pixel)
std::vector<uint8_t> createTestJPEG()
{
  return {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0xFF, 0xDB, 0x00, 0x43, 0x00,
          0x08, 0x06, 0x06, 0x07, 0x06, 0x05, 0x08, 0x07, 0x07, 0x07, 0x09, 0x09, 0x08, 0x0A, 0x0C, 0x14, 0x0D, 0x0C, 0x0B, 0x0B, 0x0C, 0x19, 0x12, 0x13, 0x0F,
          0x14, 0x1D, 0x1A, 0x1F, 0x1E, 0x1D, 0x1A, 0x1C, 0x1C, 0x20, 0x24, 0x2E, 0x27, 0x20, 0x22, 0x2C, 0x23, 0x1C, 0x1C, 0x28, 0x37, 0x29, 0x2C, 0x30, 0x31,
          0x34, 0x34, 0x34, 0x1F, 0x27, 0x39, 0x3D, 0x38, 0x32, 0x3C, 0x2E, 0x33, 0x34, 0x32, 0xFF, 0xC0, 0x00, 0x0B, 0x08, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01,
          0x11, 0x00, 0xFF, 0xC4, 0x00, 0x1F, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
          0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0xFF, 0xC4, 0x00, 0xB5, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05,
          0x04, 0x04, 0x00, 0x00, 0x01, 0x7D, 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14,
          0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A,
          0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x53, 0x54, 0x55, 0x56,
          0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87,
          0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5,
          0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
          0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFF, 0xDA, 0x00, 0x08, 0x01, 0x01, 0x00,
          0x00, 0x3F, 0x00, 0xFB, 0xD5, 0xDB, 0x20, 0xA8, 0xBA, 0xAE, 0xEB, 0xFF, 0xD9};
}
} // namespace

TEST_SUITE("Writer Comprehensive Implementation Tests")
{
  TEST_CASE("Writer NewData3D with all optional fields")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "All fields test");

      Data3D data3D;
      data3D.name        = "CompleteScan";
      data3D.guid        = "{test-guid-001}";
      data3D.description = "Test with all fields";
      data3D.pointsSize  = 0;

      data3D.sensorVendor          = "VendorABC";
      data3D.sensorModel           = "Model123";
      data3D.sensorSerialNumber    = "SN001";
      data3D.sensorHardwareVersion = "1.0";
      data3D.sensorSoftwareVersion = "2.0";
      data3D.sensorFirmwareVersion = "3.0";

      data3D.temperature         = 25.5f;
      data3D.relativeHumidity    = 60.0f;
      data3D.atmosphericPressure = 101325.0f;

      data3D.pointFields.cartesianXField = true;
      data3D.pointFields.cartesianYField = true;
      data3D.pointFields.cartesianZField = true;
      data3D.pointFields.intensityField  = true;
      data3D.pointFields.colorRedField   = true;
      data3D.pointFields.colorGreenField = true;
      data3D.pointFields.colorBlueField  = true;

      int32_t idx = writer.NewData3D(data3D);
      REQUIRE_GE(idx, 0);
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }

  TEST_CASE("Writer NewData3D with spherical coordinates")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Spherical test");

      Data3D data3D;
      data3D.guid                                = "{spherical-guid-001}";
      data3D.pointsSize                          = 0;
      data3D.pointFields.sphericalRangeField     = true;
      data3D.pointFields.sphericalAzimuthField   = true;
      data3D.pointFields.sphericalElevationField = true;

      int32_t idx = writer.NewData3D(data3D);
      REQUIRE_GE(idx, 0);
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }

  TEST_CASE("Writer NewData3D with pose and bounds")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Pose and bounds test");

      Data3D data3D;
      data3D.guid                        = "{pose-guid-001}";
      data3D.pointsSize                  = 0;
      data3D.pose.rotation.w             = 1.0;
      data3D.pose.rotation.x             = 0.0;
      data3D.pose.rotation.y             = 0.0;
      data3D.pose.rotation.z             = 0.0;
      data3D.pose.translation.x          = 100.0;
      data3D.pose.translation.y          = 200.0;
      data3D.pose.translation.z          = 300.0;
      data3D.cartesianBounds.xMinimum    = -50.0;
      data3D.cartesianBounds.xMaximum    = 50.0;
      data3D.cartesianBounds.yMinimum    = -25.0;
      data3D.cartesianBounds.yMaximum    = 25.0;
      data3D.cartesianBounds.zMinimum    = 0.0;
      data3D.cartesianBounds.zMaximum    = 100.0;
      data3D.pointFields.cartesianXField = true;
      data3D.pointFields.cartesianYField = true;
      data3D.pointFields.cartesianZField = true;

      int32_t idx = writer.NewData3D(data3D);
      REQUIRE_GE(idx, 0);
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }

  TEST_CASE("Writer NewData3D with index bounds")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Index bounds test");

      Data3D data3D;
      data3D.guid                         = "{index-guid-001}";
      data3D.pointsSize                   = 0;
      data3D.indexBounds.rowMinimum       = 0;
      data3D.indexBounds.rowMaximum       = 999;
      data3D.indexBounds.columnMinimum    = 0;
      data3D.indexBounds.columnMaximum    = 999;
      data3D.indexBounds.returnMinimum    = 0;
      data3D.indexBounds.returnMaximum    = 3;
      data3D.pointFields.cartesianXField  = true;
      data3D.pointFields.cartesianYField  = true;
      data3D.pointFields.cartesianZField  = true;
      data3D.pointFields.rowIndexField    = true;
      data3D.pointFields.columnIndexField = true;
      data3D.pointFields.returnIndexField = true;

      int32_t idx = writer.NewData3D(data3D);
      REQUIRE_GE(idx, 0);
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }

  TEST_CASE("Writer NewData3D with intensity and color limits")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Limits test");

      Data3D data3D;
      data3D.guid                             = "{limits-guid-001}";
      data3D.pointsSize                       = 0;
      data3D.intensityLimits.intensityMinimum = 0.0f;
      data3D.intensityLimits.intensityMaximum = 1.0f;
      data3D.colorLimits.colorRedMinimum      = 0;
      data3D.colorLimits.colorRedMaximum      = 255;
      data3D.colorLimits.colorGreenMinimum    = 0;
      data3D.colorLimits.colorGreenMaximum    = 255;
      data3D.colorLimits.colorBlueMinimum     = 0;
      data3D.colorLimits.colorBlueMaximum     = 255;
      data3D.pointFields.cartesianXField      = true;
      data3D.pointFields.intensityField       = true;
      data3D.pointFields.colorRedField        = true;
      data3D.pointFields.colorGreenField      = true;
      data3D.pointFields.colorBlueField       = true;

      int32_t idx = writer.NewData3D(data3D);
      REQUIRE_GE(idx, 0);
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }

  TEST_CASE("Writer NewImage2D with visual reference")
  {
    TempFile tempFile;
    auto     jpegData = createTestJPEG();

    {
      Writer writer(tempFile.c_str(), "Image2D test");

      Image2D image2D;
      image2D.name                                        = "TestImage";
      image2D.guid                                        = "{image-guid-001}";
      image2D.description                                 = "Test image";
      image2D.visualReferenceRepresentation.imageWidth    = 1;
      image2D.visualReferenceRepresentation.imageHeight   = 1;
      image2D.visualReferenceRepresentation.jpegImageSize = jpegData.size();

      int32_t idx = writer.NewImage2D(image2D);
      REQUIRE_GE(idx, 0);

      int64_t bytesWritten = writer.WriteImage2DData(idx, E57_JPEG_IMAGE, E57_VISUAL, jpegData.data(), 0, jpegData.size());
      REQUIRE_EQ(bytesWritten, static_cast<int64_t>(jpegData.size()));
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }

  TEST_CASE("Writer NewImage2D with pinhole representation")
  {
    TempFile tempFile;
    auto     jpegData = createTestJPEG();

    {
      Writer writer(tempFile.c_str(), "Pinhole image test");

      Image2D image2D;
      image2D.name                                  = "PinholeImage";
      image2D.guid                                  = "{pinhole-guid-001}";
      image2D.pinholeRepresentation.focalLength     = 35.0;
      image2D.pinholeRepresentation.pixelWidth      = 0.001;
      image2D.pinholeRepresentation.pixelHeight     = 0.001;
      image2D.pinholeRepresentation.principalPointX = 0.5;
      image2D.pinholeRepresentation.principalPointY = 0.5;
      image2D.pinholeRepresentation.imageWidth      = 1;
      image2D.pinholeRepresentation.imageHeight     = 1;
      image2D.pinholeRepresentation.jpegImageSize   = jpegData.size();

      int32_t idx = writer.NewImage2D(image2D);
      REQUIRE_GE(idx, 0);

      int64_t bytesWritten = writer.WriteImage2DData(idx, E57_JPEG_IMAGE, E57_PINHOLE, jpegData.data(), 0, jpegData.size());
      REQUIRE_EQ(bytesWritten, static_cast<int64_t>(jpegData.size()));
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }

  TEST_CASE("Writer NewImage2D with spherical representation")
  {
    TempFile tempFile;
    auto     jpegData = createTestJPEG();

    {
      Writer writer(tempFile.c_str(), "Spherical image test");

      Image2D image2D;
      image2D.name                                  = "SphericalImage";
      image2D.guid                                  = "{spherical-img-guid-001}";
      image2D.sphericalRepresentation.pixelWidth    = 0.01;
      image2D.sphericalRepresentation.pixelHeight   = 0.01;
      image2D.sphericalRepresentation.imageWidth    = 1;
      image2D.sphericalRepresentation.imageHeight   = 1;
      image2D.sphericalRepresentation.jpegImageSize = jpegData.size();

      int32_t idx = writer.NewImage2D(image2D);
      REQUIRE_GE(idx, 0);

      int64_t bytesWritten = writer.WriteImage2DData(idx, E57_JPEG_IMAGE, E57_SPHERICAL, jpegData.data(), 0, jpegData.size());
      REQUIRE_EQ(bytesWritten, static_cast<int64_t>(jpegData.size()));
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }

  TEST_CASE("Writer NewImage2D with cylindrical representation")
  {
    TempFile tempFile;
    auto     jpegData = createTestJPEG();

    {
      Writer writer(tempFile.c_str(), "Cylindrical image test");

      Image2D image2D;
      image2D.name                                      = "CylindricalImage";
      image2D.guid                                      = "{cylindrical-img-guid-001}";
      image2D.cylindricalRepresentation.pixelWidth      = 0.01;
      image2D.cylindricalRepresentation.pixelHeight     = 0.001;
      image2D.cylindricalRepresentation.radius          = 0.5;
      image2D.cylindricalRepresentation.principalPointY = 0.5;
      image2D.cylindricalRepresentation.imageWidth      = 1;
      image2D.cylindricalRepresentation.imageHeight     = 1;
      image2D.cylindricalRepresentation.jpegImageSize   = jpegData.size();

      int32_t idx = writer.NewImage2D(image2D);
      REQUIRE_GE(idx, 0);

      int64_t bytesWritten = writer.WriteImage2DData(idx, E57_JPEG_IMAGE, E57_CYLINDRICAL, jpegData.data(), 0, jpegData.size());
      REQUIRE_EQ(bytesWritten, static_cast<int64_t>(jpegData.size()));
    }

    REQUIRE(std::filesystem::exists(tempFile.string()));
  }
}

TEST_SUITE("Reader Comprehensive Implementation Tests")
{
  TEST_CASE("Reader GetData3DCount with multiple scans")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Multi scan test");

      Data3D scan1;
      scan1.guid                        = "{multi-scan-guid-001}";
      scan1.pointsSize                  = 0;
      scan1.name                        = "Scan1";
      scan1.pointFields.cartesianXField = true;
      writer.NewData3D(scan1);

      Data3D scan2;
      scan2.guid                        = "{multi-scan-guid-002}";
      scan2.pointsSize                  = 0;
      scan2.name                        = "Scan2";
      scan2.pointFields.cartesianXField = true;
      writer.NewData3D(scan2);
    }

    Reader reader(tempFile.c_str());
    REQUIRE_EQ(reader.GetData3DCount(), 2);
  }

  TEST_CASE("Reader GetImage2DCount with multiple images")
  {
    TempFile tempFile;
    auto     jpegData = createTestJPEG();

    {
      Writer writer(tempFile.c_str(), "Multi image test");

      Image2D img1;
      img1.guid                                        = "{multi-img-guid-001}";
      img1.name                                        = "Image1";
      img1.visualReferenceRepresentation.imageWidth    = 1;
      img1.visualReferenceRepresentation.imageHeight   = 1;
      img1.visualReferenceRepresentation.jpegImageSize = jpegData.size();
      int32_t idx1                                     = writer.NewImage2D(img1);
      writer.WriteImage2DData(idx1, E57_JPEG_IMAGE, E57_VISUAL, jpegData.data(), 0, jpegData.size());

      Image2D img2;
      img2.guid                                        = "{multi-img-guid-002}";
      img2.name                                        = "Image2";
      img2.visualReferenceRepresentation.imageWidth    = 1;
      img2.visualReferenceRepresentation.imageHeight   = 1;
      img2.visualReferenceRepresentation.jpegImageSize = jpegData.size();
      int32_t idx2                                     = writer.NewImage2D(img2);
      writer.WriteImage2DData(idx2, E57_JPEG_IMAGE, E57_VISUAL, jpegData.data(), 0, jpegData.size());
    }

    Reader reader(tempFile.c_str());
    REQUIRE_EQ(reader.GetImage2DCount(), 2);
  }

  TEST_CASE("Reader ReadData3D with all optional fields")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Full metadata test");

      Data3D data3D;
      data3D.name                        = "FullScan";
      data3D.guid                        = "{full-metadata-guid-001}";
      data3D.description                 = "Complete scan";
      data3D.sensorVendor                = "Vendor";
      data3D.sensorModel                 = "Model";
      data3D.sensorSerialNumber          = "Serial";
      data3D.temperature                 = 25.0f;
      data3D.relativeHumidity            = 50.0f;
      data3D.atmosphericPressure         = 100000.0f;
      data3D.pointsSize                  = 0;
      data3D.pointFields.cartesianXField = true;
      data3D.pointFields.cartesianYField = true;
      data3D.pointFields.cartesianZField = true;

      writer.NewData3D(data3D);
    }

    Reader reader(tempFile.c_str());
    Data3D readData3D;
    REQUIRE(reader.ReadData3D(0, readData3D));
    REQUIRE_EQ(readData3D.name, "FullScan");
    REQUIRE_EQ(readData3D.guid, "{full-metadata-guid-001}");
    REQUIRE_EQ(readData3D.description, "Complete scan");
    REQUIRE_EQ(readData3D.sensorVendor, "Vendor");
    REQUIRE_EQ(readData3D.sensorModel, "Model");
    REQUIRE_EQ(readData3D.sensorSerialNumber, "Serial");
  }

  TEST_CASE("Reader ReadImage2D with different representations")
  {
    TempFile tempFile;
    auto     jpegData = createTestJPEG();

    {
      Writer writer(tempFile.c_str(), "Image repr test");

      Image2D img1;
      img1.guid                                        = "{repr-img-guid-001}";
      img1.name                                        = "VisualRef";
      img1.visualReferenceRepresentation.imageWidth    = 1;
      img1.visualReferenceRepresentation.imageHeight   = 1;
      img1.visualReferenceRepresentation.jpegImageSize = jpegData.size();
      int32_t idx1                                     = writer.NewImage2D(img1);
      writer.WriteImage2DData(idx1, E57_JPEG_IMAGE, E57_VISUAL, jpegData.data(), 0, jpegData.size());

      Image2D img2;
      img2.guid                                = "{repr-img-guid-002}";
      img2.name                                = "Pinhole";
      img2.pinholeRepresentation.focalLength   = 35.0;
      img2.pinholeRepresentation.imageWidth    = 1;
      img2.pinholeRepresentation.imageHeight   = 1;
      img2.pinholeRepresentation.jpegImageSize = jpegData.size();
      int32_t idx2                             = writer.NewImage2D(img2);
      writer.WriteImage2DData(idx2, E57_JPEG_IMAGE, E57_PINHOLE, jpegData.data(), 0, jpegData.size());

      Image2D img3;
      img3.guid                                  = "{repr-img-guid-003}";
      img3.name                                  = "Spherical";
      img3.sphericalRepresentation.imageWidth    = 1;
      img3.sphericalRepresentation.imageHeight   = 1;
      img3.sphericalRepresentation.jpegImageSize = jpegData.size();
      int32_t idx3                               = writer.NewImage2D(img3);
      writer.WriteImage2DData(idx3, E57_JPEG_IMAGE, E57_SPHERICAL, jpegData.data(), 0, jpegData.size());
    }

    Reader reader(tempFile.c_str());
    REQUIRE_EQ(reader.GetImage2DCount(), 3);

    Image2D readImg1, readImg2, readImg3;
    REQUIRE(reader.ReadImage2D(0, readImg1));
    REQUIRE_EQ(readImg1.name, "VisualRef");

    REQUIRE(reader.ReadImage2D(1, readImg2));
    REQUIRE_EQ(readImg2.name, "Pinhole");

    REQUIRE(reader.ReadImage2D(2, readImg3));
    REQUIRE_EQ(readImg3.name, "Spherical");
  }

  TEST_CASE("Reader GetRawE57Root returns valid node")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Raw root test");
    }

    Reader        reader(tempFile.c_str());
    StructureNode root = reader.GetRawE57Root();
    REQUIRE_GE(root.childCount(), 0);
  }

  TEST_CASE("Reader GetRawData3D returns valid vector")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Raw Data3D test");

      Data3D data3D;
      data3D.guid                        = "{raw-data3d-guid-001}";
      data3D.pointsSize                  = 0;
      data3D.pointFields.cartesianXField = true;
      writer.NewData3D(data3D);
    }

    Reader     reader(tempFile.c_str());
    VectorNode data3D = reader.GetRawData3D();
    REQUIRE_GE(data3D.childCount(), 0);
  }

  TEST_CASE("Reader GetRawImages2D returns valid vector")
  {
    TempFile tempFile;
    auto     jpegData = createTestJPEG();

    {
      Writer writer(tempFile.c_str(), "Raw Images2D test");

      Image2D img;
      img.guid                                        = "{raw-images-guid-001}";
      img.name                                        = "TestImage";
      img.visualReferenceRepresentation.imageWidth    = 1;
      img.visualReferenceRepresentation.imageHeight   = 1;
      img.visualReferenceRepresentation.jpegImageSize = jpegData.size();
      int32_t idx                                     = writer.NewImage2D(img);
      writer.WriteImage2DData(idx, E57_JPEG_IMAGE, E57_VISUAL, jpegData.data(), 0, jpegData.size());
    }

    Reader     reader(tempFile.c_str());
    VectorNode images2D = reader.GetRawImages2D();
    REQUIRE_GE(images2D.childCount(), 0);
  }

  TEST_CASE("Reader GetRawIMF returns valid ImageFile")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Raw IMF test");
    }

    Reader    reader(tempFile.c_str());
    ImageFile imf = reader.GetRawIMF();
    REQUIRE(imf.isOpen());
  }
}

TEST_SUITE("Writer Raw API Access Tests")
{
  TEST_CASE("Writer GetRawE57Root returns valid node")
  {
    TempFile tempFile;
    Writer   writer(tempFile.c_str(), "Writer raw root test");

    StructureNode root = writer.GetRawE57Root();
    REQUIRE_GE(root.childCount(), 0);
  }

  TEST_CASE("Writer GetRawData3D returns valid vector")
  {
    TempFile tempFile;
    Writer   writer(tempFile.c_str(), "Writer raw Data3D test");

    VectorNode data3D = writer.GetRawData3D();
    REQUIRE_GE(data3D.childCount(), 0);
  }

  TEST_CASE("Writer GetRawImages2D returns valid vector")
  {
    TempFile tempFile;
    Writer   writer(tempFile.c_str(), "Writer raw Images2D test");

    VectorNode images2D = writer.GetRawImages2D();
    REQUIRE_GE(images2D.childCount(), 0);
  }

  TEST_CASE("Writer GetRawIMF returns valid ImageFile")
  {
    TempFile tempFile;
    Writer   writer(tempFile.c_str(), "Writer raw IMF test");

    ImageFile imf = writer.GetRawIMF();
    REQUIRE(imf.isOpen());
  }
}

TEST_SUITE("Edge Cases and Error Handling")
{
  TEST_CASE("Multiple Data3D scans with different fields")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Different fields test");

      Data3D scan1;
      scan1.guid                        = "{diff-fields-guid-001}";
      scan1.pointsSize                  = 0;
      scan1.name                        = "Cartesian";
      scan1.pointFields.cartesianXField = true;
      scan1.pointFields.cartesianYField = true;
      scan1.pointFields.cartesianZField = true;
      writer.NewData3D(scan1);

      Data3D scan2;
      scan2.guid                                = "{diff-fields-guid-002}";
      scan2.pointsSize                          = 0;
      scan2.name                                = "Spherical";
      scan2.pointFields.sphericalRangeField     = true;
      scan2.pointFields.sphericalAzimuthField   = true;
      scan2.pointFields.sphericalElevationField = true;
      writer.NewData3D(scan2);
    }

    Reader reader(tempFile.c_str());
    REQUIRE_EQ(reader.GetData3DCount(), 2);

    Data3D readScan1, readScan2;
    REQUIRE(reader.ReadData3D(0, readScan1));
    REQUIRE_EQ(readScan1.name, "Cartesian");

    REQUIRE(reader.ReadData3D(1, readScan2));
    REQUIRE_EQ(readScan2.name, "Spherical");
  }

  TEST_CASE("Empty file with no scans or images")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Empty file test");
    }

    Reader reader(tempFile.c_str());
    REQUIRE_EQ(reader.GetData3DCount(), 0);
    REQUIRE_EQ(reader.GetImage2DCount(), 0);
  }

  TEST_CASE("File with only metadata")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Metadata only test");
    }

    Reader  reader(tempFile.c_str());
    E57Root root;
    REQUIRE(reader.GetE57Root(root));
  }

  TEST_CASE("Open and close multiple times")
  {
    TempFile tempFile;

    {
      Writer writer(tempFile.c_str(), "Multi open test");
    }

    {
      Reader reader(tempFile.c_str());
      REQUIRE(reader.IsOpen());
      reader.Close();
      REQUIRE(!reader.IsOpen());
    }

    {
      Reader reader(tempFile.c_str());
      REQUIRE(reader.IsOpen());
    }
  }
}
