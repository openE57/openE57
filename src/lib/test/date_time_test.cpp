#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <openE57/impl/time_conversion.h>

using namespace e57;

TEST_SUITE("DateTime Tests")
{
  TEST_CASE("Testings that current date time isn't zero")
  {
    unsigned short utc_year;
    unsigned char  utc_month;
    unsigned char  utc_day;
    unsigned char  utc_hour;
    unsigned char  utc_minute;
    float          utc_seconds;
    unsigned char  utc_offset;
    double         julian_date;
    unsigned short gps_week;
    double         gps_tow;

    const bool result = utils::current_system_time(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds, utc_offset, julian_date, gps_week, gps_tow);

    REQUIRE_EQ(true, result);
    REQUIRE(utc_year >= 2022);
    REQUIRE(utc_month >= 1);
    REQUIRE(utc_day >= 1);
    REQUIRE(utc_hour >= 0);
    REQUIRE(utc_minute >= 0);
    REQUIRE(utc_seconds >= 0);
    REQUIRE(julian_date >= 0);
    REQUIRE(gps_week >= 0);
    REQUIRE(gps_tow >= 0);
  }
}