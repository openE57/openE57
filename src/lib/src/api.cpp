#include <openE57/api.h>
#include <openE57/impl/time_conversion.h>

using namespace e57;

time::GpsTime::GpsTime(const double gps_time) : time{gps_time}
{
  unsigned short gps_week{};
  double         gps_tow{};

  if (!time::gps_time_from_value(gps_time, gps_week, gps_tow))
  {
    this->week = 0;
    this->tow  = 0.0;
    return;
  }

  this->week = gps_week;
  this->tow  = gps_tow;
}

time::GpsTime::GpsTime(const uint16_t gps_week, const double gps_tow) : week{gps_week}, tow{gps_tow}
{
  double gps_time{};
  if (!time::gps_time_to_value(week, tow, gps_time))
  {
    this->time = 0.0;
    return;
  }

  this->time = gps_time;
}

[[nodiscard]] time::GpsTime time::current_gps_time() noexcept
{
  uint16_t gps_week{};
  double   gps_tow{};

  if (!time::current_gps_time(gps_week, gps_tow))
  {
    return time::GpsTime{};
  }

  return time::GpsTime(gps_week, gps_tow);
}

[[nodiscard]] time::UtcTime time::current_utc_time() noexcept
{
  unsigned short utc_year{};
  unsigned char  utc_month{};
  unsigned char  utc_day{};
  unsigned char  utc_hour{};
  unsigned char  utc_minute{};
  float          utc_seconds{};

  if (!time::current_utc_time(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    return time::UtcTime{};
  }

  return time::UtcTime{utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds};
}

[[nodiscard]] time::UtcTime time::utc_time_from_gps_time(const time::GpsTime& gps_time) noexcept
{
  unsigned short utc_year{};
  unsigned char  utc_month{};
  unsigned char  utc_day{};
  unsigned char  utc_hour{};
  unsigned char  utc_minute{};
  float          utc_seconds{};

  if (!time::utc_time_from_gps_time(gps_time.week, gps_time.tow, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    return time::UtcTime{};
  }

  return time::UtcTime{utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds};
}

[[nodiscard]] time::GpsTime time::gps_time_from_utc_time(const time::UtcTime& utc_time) noexcept
{
  unsigned short gps_week{};
  double         gps_tow{};

  if (!time::gps_time_from_utc_time(utc_time.year, utc_time.month, utc_time.day, utc_time.hour, utc_time.minute, utc_time.seconds, gps_week, gps_tow))
  {
    return time::GpsTime{};
  }

  return time::GpsTime(gps_week, gps_tow);
}

[[nodiscard]] time::GpsTime time::gps_time_from_year_and_day(const unsigned short year, const unsigned short day_of_year) noexcept
{
  unsigned short gps_week{};
  double         gps_tow{};
  if (!time::gps_time_from_year_and_day_of_year(year, day_of_year, gps_week, gps_tow))
  {
    return time::GpsTime{};
  }

  return time::GpsTime(gps_week, gps_tow);
}
