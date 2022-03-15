#include <openE57/api.h>
#include <openE57/impl/time_conversion.h>

using namespace e57;

[[nodiscard]] double utils::current_gps_time() noexcept
{
  return 0.0;
}

[[nodiscard]] core::DateTime utils::current_date_time() noexcept
{
  return core::DateTime{};
}

[[nodiscard]] core::DateTime utils::date_time_from_number(const double coreValue)
{
  return core::DateTime{};
}

[[nodiscard]] double utils::date_time_number_from_value(const core::DateTime& coreValue)
{
  return 0.0;
}
