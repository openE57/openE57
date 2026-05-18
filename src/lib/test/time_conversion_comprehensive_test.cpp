/*
 * time_conversion_comprehensive_test.cpp
 * Copyright (c) 2024 openE57 Contributors
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <openE57/impl/time_conversion.h>

using namespace e57;

TEST_SUITE("Time Conversion Comprehensive")
{
  TEST_CASE("gps_time_from_utc_time comprehensive valid inputs")
  {
    unsigned short gps_week = 0;
    double         gps_tow  = 0.0;

    SUBCASE("Year 1980 week 1")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(1980, 1, 7, 0, 0, 0.0f, gps_week, gps_tow));
    }

    SUBCASE("Year 1990")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(1990, 6, 15, 12, 30, 45.0f, gps_week, gps_tow));
      CHECK_GT(gps_week, 500);
    }

    SUBCASE("Year 2000")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(2000, 1, 1, 0, 0, 0.0f, gps_week, gps_tow));
      CHECK_GT(gps_week, 1000);
    }

    SUBCASE("Year 2010")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(2010, 12, 31, 23, 59, 59.0f, gps_week, gps_tow));
      CHECK_GT(gps_week, 1500);
    }

    SUBCASE("Year 2020")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(2020, 7, 1, 12, 0, 0.0f, gps_week, gps_tow));
      CHECK_GT(gps_week, 2000);
    }

    SUBCASE("Year 2024")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(2024, 1, 1, 0, 0, 0.0f, gps_week, gps_tow));
      CHECK_GT(gps_week, 2200);
    }

    SUBCASE("Leap year 2024 Feb 29")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(2024, 2, 29, 12, 0, 0.0f, gps_week, gps_tow));
    }

    SUBCASE("Midnight")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(2024, 6, 15, 0, 0, 0.0f, gps_week, gps_tow));
    }

    SUBCASE("Noon")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(2024, 6, 15, 12, 0, 0.0f, gps_week, gps_tow));
    }

    SUBCASE("End of day")
    {
      REQUIRE(e57::utils::gps_time_from_utc_time(2024, 6, 15, 23, 59, 59.999f, gps_week, gps_tow));
    }
  }

  TEST_CASE("gps_time_from_utc_time invalid inputs")
  {
    unsigned short gps_week = 0;
    double         gps_tow  = 0.0;

    SUBCASE("Invalid month 0")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 0, 1, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Invalid month 13")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 13, 1, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Invalid day 0")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 1, 0, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Invalid day 32")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 1, 32, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Invalid hour 24")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 1, 1, 24, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Invalid minute 60")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 1, 1, 0, 60, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Invalid seconds 61")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 1, 1, 0, 0, 61.0f, gps_week, gps_tow));
    }
    SUBCASE("Feb 30 non-leap")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2023, 2, 30, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Feb 29 non-leap")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2023, 2, 29, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Apr 31")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 4, 31, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Jun 31")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 6, 31, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Sep 31")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 9, 31, 0, 0, 0.0f, gps_week, gps_tow));
    }
    SUBCASE("Nov 31")
    {
      REQUIRE(!e57::utils::gps_time_from_utc_time(2024, 11, 31, 0, 0, 0.0f, gps_week, gps_tow));
    }
  }

  TEST_CASE("utc_time_from_gps_time valid inputs")
  {
    unsigned short utc_year    = 0;
    unsigned char  utc_month   = 0;
    unsigned char  utc_day     = 0;
    unsigned char  utc_hour    = 0;
    unsigned char  utc_minute  = 0;
    float          utc_seconds = 0.0f;

    SUBCASE("Week 0")
    {
      REQUIRE(e57::utils::utc_time_from_gps_time(0, 0.0, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds));
      CHECK_EQ(utc_year, 1980);
    }

    SUBCASE("Week 500")
    {
      REQUIRE(e57::utils::utc_time_from_gps_time(500, 100000.0, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds));
      CHECK_GT(utc_year, 1985);
    }

    SUBCASE("Week 1000")
    {
      REQUIRE(e57::utils::utc_time_from_gps_time(1000, 200000.0, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds));
      CHECK_GT(utc_year, 1995);
    }

    SUBCASE("Week 1500")
    {
      REQUIRE(e57::utils::utc_time_from_gps_time(1500, 300000.0, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds));
      CHECK_GT(utc_year, 2005);
    }

    SUBCASE("Week 2000")
    {
      REQUIRE(e57::utils::utc_time_from_gps_time(2000, 400000.0, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds));
      CHECK_GT(utc_year, 2015);
    }

    SUBCASE("Week 2200")
    {
      REQUIRE(e57::utils::utc_time_from_gps_time(2200, 100000.0, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds));
      CHECK_GE(utc_year, 2020);
    }
  }

  TEST_CASE("utc_time_from_gps_time invalid inputs")
  {
    unsigned short utc_year    = 0;
    unsigned char  utc_month   = 0;
    unsigned char  utc_day     = 0;
    unsigned char  utc_hour    = 0;
    unsigned char  utc_minute  = 0;
    float          utc_seconds = 0.0f;

    SUBCASE("Negative TOW")
    {
      REQUIRE(!e57::utils::utc_time_from_gps_time(1000, -1.0, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds));
    }
    SUBCASE("TOW > 604800")
    {
      REQUIRE(!e57::utils::utc_time_from_gps_time(1000, 604801.0, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds));
    }
  }

  TEST_CASE("gps_time_to_value and from_value")
  {
    SUBCASE("Zero")
    {
      double gps_time = 0.0;
      REQUIRE(e57::utils::gps_time_to_value(0, 0.0, gps_time));
      CHECK_EQ(gps_time, 0.0);
    }

    SUBCASE("Week 1 TOW 0")
    {
      double gps_time = 0.0;
      REQUIRE(e57::utils::gps_time_to_value(1, 0.0, gps_time));
      CHECK_EQ(gps_time, 604800.0);
    }

    SUBCASE("Week 100 TOW 100000")
    {
      double gps_time = 0.0;
      REQUIRE(e57::utils::gps_time_to_value(100, 100000.0, gps_time));
      CHECK_GT(gps_time, 6000000.0);
    }

    SUBCASE("Round trip week 500")
    {
      double         orig_time = 500.0 * 604800.0 + 123456.0;
      unsigned short gps_week  = 0;
      double         gps_tow   = 0.0;
      REQUIRE(e57::utils::gps_time_from_value(orig_time, gps_week, gps_tow));
      CHECK_EQ(gps_week, 500);
      CHECK_EQ(gps_tow, doctest::Approx(123456.0));
    }

    SUBCASE("Invalid negative time")
    {
      unsigned short gps_week = 0;
      double         gps_tow  = 0.0;
      REQUIRE(!e57::utils::gps_time_from_value(-1.0, gps_week, gps_tow));
    }

    SUBCASE("Invalid TOW > 604800")
    {
      double gps_time = 0.0;
      REQUIRE(!e57::utils::gps_time_to_value(100, 604801.0, gps_time));
    }
  }

  TEST_CASE("julian_date functions")
  {
    double julian_date = 0.0;

    SUBCASE("From UTC 1980-01-07")
    {
      REQUIRE(e57::utils::julian_date_from_utc_time(1980, 1, 7, 0, 0, 0.0f, julian_date));
      CHECK_GT(julian_date, 2444244.0);
    }

    SUBCASE("From UTC 2024-01-01")
    {
      REQUIRE(e57::utils::julian_date_from_utc_time(2024, 1, 1, 0, 0, 0.0f, julian_date));
      CHECK_GT(julian_date, 2460300.0);
    }

    SUBCASE("From UTC with time")
    {
      REQUIRE(e57::utils::julian_date_from_utc_time(2024, 6, 15, 12, 30, 45.0f, julian_date));
      CHECK_GT(julian_date, 2460400.0);
    }

    SUBCASE("Invalid date")
    {
      REQUIRE(!e57::utils::julian_date_from_utc_time(2024, 2, 30, 0, 0, 0.0f, julian_date));
    }

    unsigned short gps_week   = 0;
    double         gps_tow    = 0.0;
    unsigned char  utc_offset = 0;

    SUBCASE("From GPS time")
    {
      REQUIRE(e57::utils::julian_date_from_gps_time(1000, 100000.0, 15, julian_date));
      CHECK_GT(julian_date, 2450000.0);
    }

    SUBCASE("GPS time from julian")
    {
      REQUIRE(e57::utils::gps_time_from_julian_date(2444244.5, 0, gps_week, gps_tow));
      CHECK_EQ(gps_week, 0);
    }
  }

  TEST_CASE("determine_utc_offset")
  {
    unsigned char utc_offset = 0;

    SUBCASE("1980")
    {
      REQUIRE(e57::utils::determine_utc_offset(2444244.5, utc_offset));
      CHECK_EQ(utc_offset, 0);
    }
    SUBCASE("1985")
    {
      REQUIRE(e57::utils::determine_utc_offset(2446247.5, utc_offset));
      CHECK_GE(utc_offset, 0);
    }
    SUBCASE("1990")
    {
      REQUIRE(e57::utils::determine_utc_offset(2447892.5, utc_offset));
      CHECK_GE(utc_offset, 5);
    }
    SUBCASE("2000")
    {
      REQUIRE(e57::utils::determine_utc_offset(2451544.5, utc_offset));
      CHECK_GE(utc_offset, 10);
    }
    SUBCASE("2010")
    {
      REQUIRE(e57::utils::determine_utc_offset(2455197.5, utc_offset));
      CHECK_GE(utc_offset, 14);
    }
    SUBCASE("2020")
    {
      REQUIRE(e57::utils::determine_utc_offset(2458849.5, utc_offset));
      CHECK_GE(utc_offset, 15);
    }
    SUBCASE("2024")
    {
      REQUIRE(e57::utils::determine_utc_offset(2460300.5, utc_offset));
      CHECK_GE(utc_offset, 15);
    }
  }

  TEST_CASE("current_system_time")
  {
    unsigned short utc_year    = 0;
    unsigned char  utc_month   = 0;
    unsigned char  utc_day     = 0;
    unsigned char  utc_hour    = 0;
    unsigned char  utc_minute  = 0;
    float          utc_seconds = 0.0f;
    unsigned char  utc_offset  = 0;
    double         julian_date = 0.0;
    unsigned short gps_week    = 0;
    double         gps_tow     = 0.0;

    REQUIRE(e57::utils::current_system_time(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds, utc_offset, julian_date, gps_week, gps_tow));

    CHECK_GE(utc_year, 2024);
    CHECK_GE(utc_month, 1);
    CHECK_LE(utc_month, 12);
    CHECK_GE(utc_day, 1);
    CHECK_LE(utc_day, 31);
    CHECK_LE(utc_hour, 23);
    CHECK_LE(utc_minute, 59);
    CHECK_LT(utc_seconds, 61.0f);
    CHECK_GT(julian_date, 2460000.0);
  }

  TEST_CASE("current_julian_date")
  {
    double julian_date = 0.0;
    REQUIRE(e57::utils::current_julian_date(julian_date));
    CHECK_GT(julian_date, 2460000.0);
  }

  TEST_CASE("is_utc_time_valid")
  {
    SUBCASE("Valid date")
    {
      CHECK(e57::utils::is_utc_time_valid(2024, 6, 15, 12, 30, 45.0f));
    }
    SUBCASE("Invalid month 0")
    {
      CHECK(!e57::utils::is_utc_time_valid(2024, 0, 1, 0, 0, 0.0f));
    }
    SUBCASE("Invalid month 13")
    {
      CHECK(!e57::utils::is_utc_time_valid(2024, 13, 1, 0, 0, 0.0f));
    }
    SUBCASE("Invalid day 0")
    {
      CHECK(!e57::utils::is_utc_time_valid(2024, 1, 0, 0, 0, 0.0f));
    }
    SUBCASE("Invalid day 32")
    {
      CHECK(!e57::utils::is_utc_time_valid(2024, 1, 32, 0, 0, 0.0f));
    }
    SUBCASE("Invalid hour 24")
    {
      CHECK(!e57::utils::is_utc_time_valid(2024, 1, 1, 24, 0, 0.0f));
    }
    SUBCASE("Invalid minute 60")
    {
      CHECK(!e57::utils::is_utc_time_valid(2024, 1, 1, 0, 60, 0.0f));
    }
    SUBCASE("Invalid seconds 61")
    {
      CHECK(!e57::utils::is_utc_time_valid(2024, 1, 1, 0, 0, 61.0f));
    }
    SUBCASE("Feb 29 leap year")
    {
      CHECK(e57::utils::is_utc_time_valid(2024, 2, 29, 0, 0, 0.0f));
    }
    SUBCASE("Feb 29 non-leap")
    {
      CHECK(!e57::utils::is_utc_time_valid(2023, 2, 29, 0, 0, 0.0f));
    }
  }

  TEST_CASE("number_days_in_month")
  {
    unsigned char days = 0;

    SUBCASE("Jan")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 1, days));
      CHECK_EQ(days, 31);
    }
    SUBCASE("Feb non-leap")
    {
      REQUIRE(e57::utils::number_days_in_month(2023, 2, days));
      CHECK_EQ(days, 28);
    }
    SUBCASE("Feb leap")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 2, days));
      CHECK_EQ(days, 29);
    }
    SUBCASE("Mar")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 3, days));
      CHECK_EQ(days, 31);
    }
    SUBCASE("Apr")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 4, days));
      CHECK_EQ(days, 30);
    }
    SUBCASE("May")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 5, days));
      CHECK_EQ(days, 31);
    }
    SUBCASE("Jun")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 6, days));
      CHECK_EQ(days, 30);
    }
    SUBCASE("Jul")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 7, days));
      CHECK_EQ(days, 31);
    }
    SUBCASE("Aug")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 8, days));
      CHECK_EQ(days, 31);
    }
    SUBCASE("Sep")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 9, days));
      CHECK_EQ(days, 30);
    }
    SUBCASE("Oct")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 10, days));
      CHECK_EQ(days, 31);
    }
    SUBCASE("Nov")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 11, days));
      CHECK_EQ(days, 30);
    }
    SUBCASE("Dec")
    {
      REQUIRE(e57::utils::number_days_in_month(2024, 12, days));
      CHECK_EQ(days, 31);
    }
    SUBCASE("Invalid month 0")
    {
      REQUIRE(!e57::utils::number_days_in_month(2024, 0, days));
    }
    SUBCASE("Invalid month 13")
    {
      REQUIRE(!e57::utils::number_days_in_month(2024, 13, days));
    }
  }

  TEST_CASE("gps_time_from_rinex_time")
  {
    unsigned short gps_week = 0;
    double         gps_tow  = 0.0;

    SUBCASE("Valid RINEX time")
    {
      REQUIRE(e57::utils::gps_time_from_rinex_time(2024, 6, 15, 12, 30, 45.0f, gps_week, gps_tow));
      CHECK_GT(gps_week, 2000);
    }

    SUBCASE("Invalid RINEX time")
    {
      REQUIRE(!e57::utils::gps_time_from_rinex_time(2024, 2, 30, 0, 0, 0.0f, gps_week, gps_tow));
    }
  }
}
