#include <openE57/api.h>

using namespace e57;

[[nodiscard]] double DateTimeEncoder::getCurrentDateTimeAsDouble() const noexcept
{
  return 0.0;
}

[[nodiscard]] definitions::DateTime DateTimeEncoder::getCurrentDateTime() const noexcept
{
  return definitions::DateTime{};
}

[[nodiscard]] definitions::DateTime DateTimeEncoder::convertFromDouble(const double dateTimeValue)
{
  return definitions::DateTime{};
}

[[nodiscard]] double DateTimeEncoder::convertFromDateTime(const definitions::DateTime& dateTimeValue)
{
  return 0.0;
}
