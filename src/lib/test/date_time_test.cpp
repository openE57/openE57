#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <openE57/api.h>

using namespace e57;

TEST_SUITE("DateTime Tests")
{
  TEST_CASE("Testings that current date time isn't zero")
  {
    CHECK(0.0 != utils::current_date_time_number());
  }

  TEST_CASE("Tests that current date time isn't 1/1/1970")
  {
    const auto dateTime = utils::current_date_time();
    CHECK(1970 != dateTime.year);
  }

  TEST_CASE("Testing Number to DateTime conversion")
  {
    const auto dateTime = utils::date_time_from_number(0.0);
    CHECK(1970 != dateTime.year);
  }

  TEST_CASE("Testing DateTime to Number conversion")
  {
    constexpr core::DateTime dateTime{2022, 3, 6};
    CHECK(0.0 != utils::date_time_number_from_value(dateTime));
  }
}