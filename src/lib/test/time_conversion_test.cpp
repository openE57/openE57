#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <openE57/impl/time_conversion.h>

using namespace e57;

TEST_SUITE("DateTime Tests")
{
  TEST_CASE("Tests that current system time function works as expected")
  {
    unsigned short utc_year{};
    unsigned char  utc_month{};
    unsigned char  utc_day{};
    unsigned char  utc_hour{};
    unsigned char  utc_minute{};
    float          utc_seconds{};
    unsigned char  utc_offset{};
    double         julian_date{};
    unsigned short gps_week{};
    double         gps_tow{};

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

  TEST_CASE("Tests the conversion from UTC Time to Julian Date")
  {
    double julian_date{};

    // This is the same as doing 2022-01-02 00:00:00
    const bool result = utils::julian_date_from_utc_time(2022, 1, 1, 23, 59, 60, julian_date);

    REQUIRE_EQ(true, result);
    REQUIRE_EQ(2459581.5, julian_date);
  }

  TEST_CASE("Tests the conversion from UTC Time to GPS Time")
  {
    unsigned short gps_week{};
    double         gps_tow{};

    // This is the same as doing 2022-01-02 00:00:00
    const bool result = utils::gps_time_from_utc_time(2022, 1, 1, 23, 59, 60, gps_week, gps_tow);

    REQUIRE_EQ(true, result);
    REQUIRE_EQ(2191, gps_week);
    REQUIRE_EQ(15.0, gps_tow);
  }

  TEST_CASE("Tests the conversion from Julian Date to UTC Time")
  {
    const double   julian_date{2459581.5};
    unsigned short utc_year{};
    unsigned char  utc_month{};
    unsigned char  utc_day{};
    unsigned char  utc_hour{};
    unsigned char  utc_minute{};
    float          utc_seconds{};

    const bool result = utils::utc_time_from_julian_date(julian_date, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds);

    REQUIRE_EQ(true, result);
    REQUIRE_EQ(2022, utc_year);
    REQUIRE_EQ(1, utc_month);
    REQUIRE_EQ(2, utc_day);
    REQUIRE_EQ(0, utc_hour);
    REQUIRE_EQ(0, utc_minute);
    REQUIRE_EQ(0, utc_seconds);
  }

  TEST_CASE("Tests the conversion from Julian Date to GPS Time")
  {
    const double        julian_date{2459581.5};
    const unsigned char utc_offset{15};

    unsigned short gps_week{};
    double         gps_tow{};

    const bool result = utils::gps_time_from_julian_date(julian_date, utc_offset, gps_week, gps_tow);

    REQUIRE_EQ(true, result);
    REQUIRE_EQ(2191, gps_week);
    REQUIRE_EQ(15.0, gps_tow);
  }

  TEST_CASE("Tests the conversion from GPS Time to Julian Date")
  {
    const unsigned short gps_week{2191};
    const double         gps_tow{15.0};
    const unsigned char  utc_offset{15};
    double               julian_date{};

    const bool result = utils::julian_date_from_gps_time(gps_week, gps_tow, utc_offset, julian_date);

    REQUIRE_EQ(true, result);
    REQUIRE_EQ(2459581.5, julian_date);
  }

  TEST_CASE("Tests the conversion from GPS Time to UTC Time")
  {
    const unsigned short gps_week{2191};
    const double         gps_tow{15.0};
    unsigned short       utc_year{};
    unsigned char        utc_month{};
    unsigned char        utc_day{};
    unsigned char        utc_hour{};
    unsigned char        utc_minute{};
    float                utc_seconds{};

    const bool result = utils::utc_time_from_gps_time(gps_week, gps_tow, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds);

    REQUIRE_EQ(true, result);
    REQUIRE_EQ(2022, utc_year);
    REQUIRE_EQ(1, utc_month);
    REQUIRE_EQ(2, utc_day);
    REQUIRE_EQ(0, utc_hour);
    REQUIRE_EQ(0, utc_minute);
    REQUIRE_EQ(0, utc_seconds);
  }

  TEST_CASE("Computes the UTC Offset from Julian Date")
  {
    const double  julian_date{2459581.5};
    unsigned char utc_offset{};

    const bool result = utils::determine_utc_offset(julian_date, utc_offset);

    REQUIRE_EQ(true, result);
    REQUIRE_EQ(15, utc_offset);
  }

  TEST_CASE("Tests for Leap Year")
  {
    REQUIRE_EQ(false, utils::is_leap_year(2022));
    REQUIRE_EQ(false, utils::is_leap_year(1399));
    REQUIRE_EQ(true, utils::is_leap_year(2000));
    REQUIRE_EQ(true, utils::is_leap_year(2004));
  }

  TEST_CASE("Tests for days in a month")
  {
    unsigned char days_in_month{};

    REQUIRE_EQ(true, utils::number_days_in_month(2022, 2, days_in_month));
    REQUIRE_EQ(28, days_in_month);

    REQUIRE_EQ(true, utils::number_days_in_month(2000, 2, days_in_month));
    REQUIRE_EQ(29, days_in_month);
  }

  TEST_CASE("Tests Number of Days in a Year")
  {
    unsigned short day_of_year{};

    REQUIRE_EQ(true, utils::day_of_year(2022, 1, 1, day_of_year));
    REQUIRE_EQ(1, day_of_year);

    REQUIRE_EQ(true, utils::day_of_year(2022, 12, 31, day_of_year));
    REQUIRE_EQ(365, day_of_year);
  }

  TEST_CASE("Tests the retrieval of GPS Time from Year and of Day of Year")
  {
    unsigned short gps_week{};
    double         gps_tow{};

    REQUIRE_EQ(true, utils::gps_time_from_year_and_day_of_year(2022, 2, gps_week, gps_tow));

    REQUIRE_EQ(2191, gps_week);
    REQUIRE_EQ(15.0, gps_tow);
  }
}