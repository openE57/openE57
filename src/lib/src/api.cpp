#include <openE57/api.h>
#include <openE57/impl/time_conversion.h>

using namespace e57;

core::GpsTime::GpsTime(const double gps_time) : time{gps_time}
{
  unsigned short gps_week{};
  double         gps_tow{};

  if (!utils::gps_time_from_value(gps_time, gps_week, gps_tow))
  {
    this->week = 0;
    this->tow  = 0.0;
    return;
  }

  this->week = gps_week;
  this->tow  = gps_tow;
}

core::GpsTime::GpsTime(const uint16_t gps_week, const double gps_tow) : week{gps_week}, tow{gps_tow}
{
  double gps_time{};
  if (!utils::gps_time_to_value(week, tow, gps_time))
  {
    this->time = 0.0;
    return;
  }

  this->time = gps_time;
}

[[nodiscard]] core::GpsTime utils::current_gps_time() noexcept
{
  uint16_t gps_week{};
  double   gps_tow{};
  double   gps_time{};

  if (!utils::current_gps_time(gps_week, gps_tow))
  {
    return core::GpsTime{};
  }

  if (!utils::gps_time_to_value(gps_week, gps_tow, gps_time))
  {
    return core::GpsTime(0.0);
  }

  return core::GpsTime(gps_time);
}

[[nodiscard]] core::UtcTime utils::current_utc_time() noexcept
{
  unsigned short utc_year{};
  unsigned char  utc_month{};
  unsigned char  utc_day{};
  unsigned char  utc_hour{};
  unsigned char  utc_minute{};
  float          utc_seconds{};

  if (!utils::current_utc_time(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    return core::UtcTime{};
  }

  return core::UtcTime{utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds};
}

[[nodiscard]] core::UtcTime utils::utc_time_from_gps_time(const core::GpsTime& gps_time)
{
  unsigned short utc_year{};
  unsigned char  utc_month{};
  unsigned char  utc_day{};
  unsigned char  utc_hour{};
  unsigned char  utc_minute{};
  float          utc_seconds{};

  if (!utils::utc_time_from_gps_time(gps_time.week, gps_time.tow, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    return core::UtcTime{};
  }

  return core::UtcTime{utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds};
}

[[nodiscard]] core::GpsTime utils::gps_time_from_utc_time(const core::UtcTime& utc_time)
{
  unsigned short gps_week{};
  double         gps_tow{};

  if (!utils::gps_time_from_utc_time(utc_time.year, utc_time.month, utc_time.day, utc_time.hour, utc_time.minute, utc_time.seconds, gps_week, gps_tow))
  {
    return core::GpsTime{};
  }

  return core::GpsTime(gps_week, gps_tow);
}
