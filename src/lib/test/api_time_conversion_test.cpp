#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <openE57/api.h>

using namespace e57;

TEST_SUITE("API Date/Time Tests")
{
  TEST_CASE("Tests the creation of GpsTime from week and tow works successfully")
  {
    const core::GpsTime gps_time(2191, 15.0);
    REQUIRE_EQ(1325116815.0, gps_time.time);
  }

  TEST_CASE("Tests the creation of GpsTime works from time works successfully")
  {
    const core::GpsTime gps_time(1325116815.0);
    REQUIRE_EQ(2191, gps_time.week);
    REQUIRE_EQ(15.0, gps_time.tow);
  }

  TEST_CASE("Tests the conversion to UTC Time from a given GPS Time")
  {
    const core::UtcTime utc_time = utils::utc_time_from_gps_time(core::GpsTime(2191, 15.0));
    REQUIRE_EQ(2022, utc_time.year);
    REQUIRE_EQ(1, utc_time.month);
    REQUIRE_EQ(2, utc_time.day);
    REQUIRE_EQ(0, utc_time.hour);
    REQUIRE_EQ(0, utc_time.minute);
    REQUIRE_EQ(0, utc_time.seconds);
  }
}