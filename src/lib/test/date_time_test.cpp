#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <openE57/api.h>

using namespace e57;

TEST_CASE("Testing Kata Hello World")
{
  const DateTimeEncoder encoder{};
  CHECK(0.0 == encoder.getCurrentDateTimeAsDouble());
}