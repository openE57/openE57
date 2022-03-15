#include <openE57/api.h>
#include <openE57/impl/time_conversion.h>

using namespace e57;

[[nodiscard]] core::GpsTime utils::current_gps_time() noexcept
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

  if (!utils::current_system_time(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds, utc_offset, julian_date, gps_week, gps_tow))
  {
    return core::GpsTime{};
  }
  return core::GpsTime{gps_week, gps_tow};
}

[[nodiscard]] core::UtcTime utils::current_utc_time() noexcept
{
  return core::UtcTime{};
}

[[nodiscard]] core::UtcTime utils::date_time_from_number(const double coreValue)
{
  return core::UtcTime{};
}

[[nodiscard]] double utils::date_time_number_from_value(const core::UtcTime& coreValue)
{
  return 0.0;
}
