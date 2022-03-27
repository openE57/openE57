#include <iostream>
#include <math.h>
#include <openE57/impl/time_conversion.h>
#include <sys/timeb.h>
#include <time.h>

#define ERROR_MESSAGE(msg)                                                                                                                                     \
  {                                                                                                                                                            \
    const char* themsg = msg;                                                                                                                                  \
    if (themsg != NULL)                                                                                                                                        \
    {                                                                                                                                                          \
      printf("\n%s,\n%s, %d,\n%s\n", __FILE__, __func__, __LINE__, themsg);                                                                                    \
    }                                                                                                                                                          \
    else                                                                                                                                                       \
    {                                                                                                                                                          \
      printf("\n%s,\n%s, %d,\nUnknown Error\n", __FILE__, __func__, __LINE__);                                                                                 \
    }                                                                                                                                                          \
  }

// Julian Date Offsets
constexpr const double JULIAN_DATE_OFFSET_1980 = 2444244.5000; // Jan 06 1980 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1981 = 2444786.5000; // Jul 01 1981 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1982 = 2445151.5000; // Jul 01 1982 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1983 = 2445516.5000; // Jul 01 1983 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1985 = 2446247.5000; // Jul 01 1985 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1988 = 2447161.5000; // Jan 01 1988 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1990 = 2447892.5000; // Jan 01 1990 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1991 = 2448257.5000; // Jan 01 1991 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1992 = 2448804.5000; // Jul 01 1992 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1993 = 2449169.5000; // Jul 01 1993 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1994 = 2449534.5000; // Jul 01 1994 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1996 = 2450083.5000; // Jan 01 1996 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1997 = 2450630.5000; // Jul 01 1997 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_1999 = 2451179.5000; // Jan 01 1999 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_2006 = 2453736.5000; // Jan 01 2006 00:00:00.0
constexpr const double JULIAN_DATE_OFFSET_2009 = 2454832.5000; // Jan 01 2009 00:00:00.0

// Constants definition
constexpr const double SECONDS_IN_A_DAY              = 86400.0;
constexpr const double SECONDS_IN_A_WEEK             = SECONDS_IN_A_DAY * 7;    // 604800.0
constexpr const double JULIAN_DATE_START_OF_GPS_TIME = JULIAN_DATE_OFFSET_1980; // [days]
constexpr const double JULIAN_DATE_START_OF_PC_TIME  = 2440587.5;               // [days]
constexpr const int    DAYS_IN_JAN                   = 31;
constexpr const int    DAYS_IN_MAR                   = 31;
constexpr const int    DAYS_IN_APR                   = 30;
constexpr const int    DAYS_IN_MAY                   = 31;
constexpr const int    DAYS_IN_JUN                   = 30;
constexpr const int    DAYS_IN_JUL                   = 31;
constexpr const int    DAYS_IN_AUG                   = 31;
constexpr const int    DAYS_IN_SEP                   = 30;
constexpr const int    DAYS_IN_OCT                   = 31;
constexpr const int    DAYS_IN_NOV                   = 30;
constexpr const int    DAYS_IN_DEC                   = 31;

[[nodiscard]] bool e57::utils::current_system_time(
  unsigned short& utc_year,    //!< Universal Time Coordinated    [year]
  unsigned char&  utc_month,   //!< Universal Time Coordinated    [1-12 months]
  unsigned char&  utc_day,     //!< Universal Time Coordinated    [1-31 days]
  unsigned char&  utc_hour,    //!< Universal Time Coordinated    [hours]
  unsigned char&  utc_minute,  //!< Universal Time Coordinated    [minutes]
  float&          utc_seconds, //!< Universal Time Coordinated    [s]
  unsigned char&  utc_offset,  //!< Integer seconds that GPS is ahead of UTC time, always positive             [s], obtained from a look up table
  double&         julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
  unsigned short& gps_week,    //!< GPS week (0-1024+)            [week]
  double&         gps_tow      //!< GPS time of week (0-604800.0) [s]
  ) noexcept
{
  if (!e57::utils::current_julian_date(julian_date))
  {
    ERROR_MESSAGE("current_julian_date returned false.");
    return false;
  }

  if (!e57::utils::determine_utc_offset(julian_date, utc_offset))
  {
    ERROR_MESSAGE("determine_utc_offset returned false.");
    return false;
  }

  if (!e57::utils::gps_time_from_julian_date(julian_date, utc_offset, gps_week, gps_tow))
  {
    ERROR_MESSAGE("gps_time_from_julian_date returned false.");
    return false;
  }

  if (!e57::utils::utc_time_from_julian_date(julian_date, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    ERROR_MESSAGE("utc_time_from_julian_date returned false");
    return false;
  }

  return true;
}

[[nodiscard]] bool e57::utils::current_julian_date(double& julian_date //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
                                                   ) noexcept
{
#if defined(WIN32) && !defined(__GNUC__)
  struct _timeb timebuffer; // found in <sys/timeb.h>
#else
  struct timeb timebuffer;
#endif

#ifdef _CRT_SECURE_NO_DEPRECATE
  if (_ftime_s(&timebuffer) != 0)
  {
    ERROR_MESSAGE("if( _ftime_s( &timebuffer ) != 0 )");
    return false;
  }
#else

#  if defined(WIN32) && !defined(__GNUC__)
  if (_ftime64_s(&timebuffer); != 0)
  {
    ERROR_MESSAGE("ftime returned a non-zero result code");
    return false;
  };
#  else
  if (ftime(&timebuffer) != 0)
  {
    ERROR_MESSAGE("ftime returned a non-zero result code");
    return false;
  };
#  endif

#endif
  const double timebuffer_time_in_seconds = timebuffer.time + timebuffer.millitm / 1000.0; // [s] with ms resolution

  // timebuffer_time_in_seconds is the time in seconds since midnight (00:00:00), January 1, 1970,
  // coordinated universal time (UTC). Julian date for (00:00:00), January 1, 1970 is: 2440587.5 [days]

  // convert timebuffer.time from seconds to days
  const double timebuffer_time_in_days = timebuffer_time_in_seconds / SECONDS_IN_A_DAY; // days since julian date 2440587.5000000 [days]

  // convert to julian date
  julian_date = JULIAN_DATE_START_OF_PC_TIME + timebuffer_time_in_days;

  return true;
}

[[nodiscard]] bool e57::utils::current_gps_time(unsigned short& gps_week, //!< GPS week (0-1024+)            [week]
                                                double&         gps_tow   //!< GPS time of week (0-604800.0) [s]
                                                ) noexcept
{
  double        julian_date{};
  unsigned char utc_offset{};

  if (!e57::utils::current_julian_date(julian_date))
  {
    ERROR_MESSAGE("current_julian_date returned false.");
    return false;
  }

  if (!e57::utils::determine_utc_offset(julian_date, utc_offset))
  {
    ERROR_MESSAGE("determine_utc_offset returned false.");
    return false;
  }

  if (!e57::utils::gps_time_from_julian_date(julian_date, utc_offset, gps_week, gps_tow))
  {
    ERROR_MESSAGE("gps_time_from_julian_date returned false.");
    return false;
  }

  return true;
}

[[nodiscard]] bool e57::utils::current_utc_time(unsigned short& utc_year,   //!< Universal Time Coordinated    [year]
                                                unsigned char&  utc_month,  //!< Universal Time Coordinated    [1-12 months]
                                                unsigned char&  utc_day,    //!< Universal Time Coordinated    [1-31 days]
                                                unsigned char&  utc_hour,   //!< Universal Time Coordinated    [hours]
                                                unsigned char&  utc_minute, //!< Universal Time Coordinated    [minutes]
                                                float&          utc_seconds //!< Universal Time Coordinated    [s]
                                                ) noexcept
{
  double julian_date{};

  if (!e57::utils::current_julian_date(julian_date))
  {
    ERROR_MESSAGE("current_julian_date returned false.");
    return false;
  }

  if (!e57::utils::utc_time_from_julian_date(julian_date, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    ERROR_MESSAGE("utc_time_from_julian_date returned false");
    return false;
  }

  return true;
}

[[nodiscard]] bool e57::utils::day_of_week_from_julian_date(
  const double   julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
  unsigned char& day_of_week  //!< 0-Sunday, 1-Monday, 2-Tuesday, 3-Wednesday, 4-Thursday, 5-Friday, 6-Saturday [].
  ) noexcept
{
  // "If the Julian date of noon is applied to the entire midnight-to-midnight civil
  // day centered on that noon,[5] rounding Julian dates (fractional days) for the
  // twelve hours before noon up while rounding those after noon down, then the remainder
  // upon division by 7 represents the day of the week, with 0 representing Monday,
  // 1 representing Tuesday, and so forth. Now at 17:48, Wednesday December 3 2008 (UTC)
  // the nearest noon JDN is 2454804 yielding a remainder of 2." (http://en.wikipedia.org/wiki/Julian_day, 2008-12-03)
  int64_t dow = 0;
  int64_t jd  = 0;

  if (julian_date - floor(julian_date) > 0.5)
  {
    jd = static_cast<int64_t>(floor(julian_date + 0.5));
  }
  else
  {
    jd = static_cast<int64_t>(floor(julian_date));
  }
  dow = jd % 7; // 0 is monday, 1 is tuesday, etc

  switch (dow)
  {
  case 0:
    day_of_week = 1;
    break;
  case 1:
    day_of_week = 2;
    break;
  case 2:
    day_of_week = 3;
    break;
  case 3:
    day_of_week = 4;
    break;
  case 4:
    day_of_week = 5;
    break;
  case 5:
    day_of_week = 6;
    break;
  case 6:
    day_of_week = 0;
    break;
  default:
    return false;
    break;
  }

  return true;
}

[[nodiscard]] bool e57::utils::julian_date_from_gps_time(
  const unsigned short gps_week,   //!< GPS week (0-1024+)             [week]
  const double         gps_tow,    //!< GPS time of week (0-604800.0)  [s]
  const unsigned char  utc_offset, //!< Integer seconds that GPS is ahead of UTC time, always positive [s]
  double&              julian_date //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
  ) noexcept
{
  if (gps_tow < 0.0 || gps_tow > SECONDS_IN_A_WEEK)
  {
    ERROR_MESSAGE("if( gps_tow < 0.0  || gps_tow > 604800.0 )");
    return false;
  }

  // GPS time is ahead of UTC time and Julian time by the UTC offset
  julian_date = (static_cast<double>(gps_week) + (gps_tow - static_cast<double>(utc_offset)) / SECONDS_IN_A_WEEK) * 7.0 + JULIAN_DATE_START_OF_GPS_TIME;
  return true;
}

[[nodiscard]] bool e57::utils::julian_date_from_utc_time(
  const unsigned short utc_year,    //!< Universal Time Coordinated  [year]
  const unsigned char  utc_month,   //!< Universal Time Coordinated  [1-12 months]
  const unsigned char  utc_day,     //!< Universal Time Coordinated  [1-31 days]
  const unsigned char  utc_hour,    //!< Universal Time Coordinated  [hours]
  const unsigned char  utc_minute,  //!< Universal Time Coordinated  [minutes]
  const float          utc_seconds, //!< Universal Time Coordinated  [s]
  double&              julian_date  //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
  ) noexcept
{
  double y{0.0}; // temp for year
  double m{0.0}; // temp for month

  // Check the input.
  if (!e57::utils::is_utc_time_valid(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    ERROR_MESSAGE("is_utc_time_valid returned false.");
    return false;
  }

  if (utc_month <= 2)
  {
    y = utc_year - 1;
    m = utc_month + 12;
  }
  else
  {
    y = utc_year;
    m = utc_month;
  }

  julian_date = static_cast<int32_t>(365.25 * y) + static_cast<int32_t>(30.6001 * (m + 1.0)) + utc_day + utc_hour / 24.0 + utc_minute / 1440.0
                + utc_seconds / SECONDS_IN_A_DAY + 1720981.5;
  return true;
}

[[nodiscard]] bool e57::utils::gps_time_from_julian_date(
  const double        julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
  const unsigned char utc_offset,  //!< Integer seconds that GPS is ahead of UTC time, always positive [s]
  unsigned short&     gps_week,    //!< GPS week (0-1024+)            [week]
  double&             gps_tow      //!< GPS time of week [s]
  ) noexcept
{
  // Check the input.
  if (julian_date < 0.0)
  {
    ERROR_MESSAGE("julian_date is negative");
    return false;
  }

  unsigned short week = static_cast<unsigned short>((julian_date - JULIAN_DATE_START_OF_GPS_TIME) / 7.0);

  double tow = (julian_date - JULIAN_DATE_START_OF_GPS_TIME) * SECONDS_IN_A_DAY; // seconds since start of gps time [s]
  tow -= (week)*SECONDS_IN_A_WEEK;                                               // seconds into the current week [s]

  // however, GPS time is ahead of utc time by the UTC offset (and thus the Julian date as well)
  tow += utc_offset;
  if (tow > SECONDS_IN_A_WEEK)
  {
    tow -= SECONDS_IN_A_WEEK;
    week += 1;
  }

  gps_week = week;
  gps_tow  = tow;

  return true;
}

[[nodiscard]] bool e57::utils::utc_time_from_julian_date(
  const double    julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
  unsigned short& utc_year,    //!< Universal Time Coordinated    [year]
  unsigned char&  utc_month,   //!< Universal Time Coordinated    [1-12 months]
  unsigned char&  utc_day,     //!< Universal Time Coordinated    [1-31 days]
  unsigned char&  utc_hour,    //!< Universal Time Coordinated    [hours]
  unsigned char&  utc_minute,  //!< Universal Time Coordinated    [minutes]
  float&          utc_seconds  //!< Universal Time Coordinated    [s]
  ) noexcept
{
  // Check the input.
  if (julian_date < 0.0)
  {
    ERROR_MESSAGE("julian_date is negative");
    return false;
  }

  unsigned char days_in_month = 0;

  // temporary values
  const int32_t a = static_cast<int32_t>(julian_date + 0.5);
  const int32_t b = a + 1537;
  const int32_t c = static_cast<int32_t>((static_cast<double>(b) - 122.1) / 365.25);
  const int32_t d = static_cast<int32_t>(365.25 * c);
  const int32_t e = static_cast<int32_t>((static_cast<double>(b - d)) / 30.6001);

  double        td  = b - d - static_cast<int>(30.6001 * e) + fmod(julian_date + 0.5, 1.0); // [days]
  unsigned char day = static_cast<unsigned char>(td);

  // compute hours
  td -= day;
  td *= 24.0;
  unsigned char hour = static_cast<unsigned char>(td);
  // compute minutes
  td -= hour;
  td *= 60.0;
  unsigned char minute = static_cast<unsigned char>(td);
  // compute seconds
  td -= minute;
  td *= 60.0;
  float seconds = td;

  unsigned char  month = static_cast<unsigned char>(e - 1 - 12 * static_cast<int>(e / 14));
  unsigned short year  = static_cast<unsigned short>(c - 4715 - static_cast<int>((7.0 + static_cast<double>(month)) / 10.0));

  // check for rollover issues
  if (seconds >= 60.0)
  {
    seconds -= 60.0;
    minute++;
    if (minute >= 60)
    {
      minute -= 60;
      hour++;
      if (hour >= 24)
      {
        hour -= 24;
        day++;

        if (!e57::utils::number_days_in_month(year, month, days_in_month))
        {
          ERROR_MESSAGE("number_days_in_month returned false.");
          return false;
        }

        if (day > days_in_month)
        {
          day = 1;
          month++;
          if (month > 12)
          {
            month = 1;
            year++;
          }
        }
      }
    }
  }

  utc_year    = year;
  utc_month   = month;
  utc_day     = day;
  utc_hour    = hour;
  utc_minute  = minute;
  utc_seconds = static_cast<float>(seconds);

  return true;
}

[[nodiscard]] bool e57::utils::gps_time_from_utc_time(const unsigned short utc_year,    //!< Universal Time Coordinated    [year]
                                                      const unsigned char  utc_month,   //!< Universal Time Coordinated    [1-12 months]
                                                      const unsigned char  utc_day,     //!< Universal Time Coordinated    [1-31 days]
                                                      const unsigned char  utc_hour,    //!< Universal Time Coordinated    [hours]
                                                      const unsigned char  utc_minute,  //!< Universal Time Coordinated    [minutes]
                                                      const float          utc_seconds, //!< Universal Time Coordinated    [s]
                                                      unsigned short&      gps_week,    //!< GPS week (0-1024+)            [week]
                                                      double&              gps_tow      //!< GPS time of week (0-604800.0) [s]
                                                      ) noexcept
{
  double        julian_date{0.0};
  unsigned char utc_offset{0};

  // Check the input.
  if (!e57::utils::is_utc_time_valid(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    ERROR_MESSAGE("is_utc_time_valid returned false.");
    return false;
  }

  if (!e57::utils::julian_date_from_utc_time(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds, julian_date))
  {
    ERROR_MESSAGE("julian_date_from_utc_time returned false.");
    return false;
  }

  if (!e57::utils::determine_utc_offset(julian_date, utc_offset))
  {
    ERROR_MESSAGE("determine_utc_offset returned false.");
    return false;
  }

  if (!e57::utils::gps_time_from_julian_date(julian_date, utc_offset, gps_week, gps_tow))
  {
    ERROR_MESSAGE("gps_time_from_julian_date returned false.");
    return false;
  }

  return true;
}

[[nodiscard]] bool e57::utils::gps_time_from_rinex_time(const unsigned short utc_year,    //!< Universal Time Coordinated    [year]
                                                        const unsigned char  utc_month,   //!< Universal Time Coordinated    [1-12 months]
                                                        const unsigned char  utc_day,     //!< Universal Time Coordinated    [1-31 days]
                                                        const unsigned char  utc_hour,    //!< Universal Time Coordinated    [hours]
                                                        const unsigned char  utc_minute,  //!< Universal Time Coordinated    [minutes]
                                                        const float          utc_seconds, //!< Universal Time Coordinated    [s]
                                                        unsigned short&      gps_week,    //!< GPS week (0-1024+)            [week]
                                                        double&              gps_tow      //!< GPS time of week (0-604800.0) [s]
                                                        ) noexcept
{
  double        julian_date{0.0};
  unsigned char utc_offset{0};

  // Check the input.
  if (!e57::utils::is_utc_time_valid(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    ERROR_MESSAGE("is_utc_time_valid returned false.");
    return false;
  }

  if (!e57::utils::julian_date_from_utc_time(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds, julian_date))
  {
    ERROR_MESSAGE("julian_date_from_utc_time returned false.");
    return false;
  }

  if (!e57::utils::gps_time_from_julian_date(julian_date, utc_offset, gps_week, gps_tow))
  {
    ERROR_MESSAGE("GetGPSTimeFromJulianDate returned false.");
    return false;
  }

  return true;
}

[[nodiscard]] bool e57::utils::utc_time_from_gps_time(const unsigned short gps_week,   //!< GPS week (0-1024+)            [week]
                                                      const double         gps_tow,    //!< GPS time of week (0-604800.0) [s]
                                                      unsigned short&      utc_year,   //!< Universal Time Coordinated    [year]
                                                      unsigned char&       utc_month,  //!< Universal Time Coordinated    [1-12 months]
                                                      unsigned char&       utc_day,    //!< Universal Time Coordinated    [1-31 days]
                                                      unsigned char&       utc_hour,   //!< Universal Time Coordinated    [hours]
                                                      unsigned char&       utc_minute, //!< Universal Time Coordinated    [minutes]
                                                      float&               utc_seconds //!< Universal Time Coordinated    [s]
                                                      ) noexcept
{
  double        julian_date{0.0};
  unsigned char utc_offset{0};
  int           i{0};

  if (gps_tow < 0.0 || gps_tow > SECONDS_IN_A_WEEK)
  {
    ERROR_MESSAGE("if( gps_tow < 0.0 || gps_tow > 604800.0 )");
    return false;
  }

  // iterate to get the right utc offset
  for (i = 0; i < 4; i++)
  {
    if (!e57::utils::julian_date_from_gps_time(gps_week, gps_tow, utc_offset, julian_date))
    {
      ERROR_MESSAGE("julian_date_from_gps_time returned false.");
      return false;
    }

    if (!e57::utils::determine_utc_offset(julian_date, utc_offset))
    {
      ERROR_MESSAGE("determine_utc_offset returned false.");
      return false;
    }
  }

  if (!e57::utils::utc_time_from_julian_date(julian_date, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds))
  {
    ERROR_MESSAGE("utc_time_from_julian_date returned false.");
    return false;
  }

  return true;
}

[[nodiscard]] bool e57::utils::determine_utc_offset(
  const double   julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
  unsigned char& utc_offset   //!< Integer seconds that GPS is ahead of UTC time, always positive             [s], obtained from a look up table
  ) noexcept
{
  if (julian_date < 0.0)
  {
    ERROR_MESSAGE("julian_date is negative");
    return false;
  }

  if (julian_date < JULIAN_DATE_OFFSET_1981)
    utc_offset = 0;
  else if (julian_date < JULIAN_DATE_OFFSET_1982)
    utc_offset = 1;
  else if (julian_date < JULIAN_DATE_OFFSET_1983)
    utc_offset = 2;
  else if (julian_date < JULIAN_DATE_OFFSET_1985)
    utc_offset = 3;
  else if (julian_date < JULIAN_DATE_OFFSET_1988)
    utc_offset = 4;
  else if (julian_date < JULIAN_DATE_OFFSET_1990)
    utc_offset = 5;
  else if (julian_date < JULIAN_DATE_OFFSET_1991)
    utc_offset = 6;
  else if (julian_date < JULIAN_DATE_OFFSET_1992)
    utc_offset = 7;
  else if (julian_date < JULIAN_DATE_OFFSET_1993)
    utc_offset = 8;
  else if (julian_date < JULIAN_DATE_OFFSET_1994)
    utc_offset = 9;
  else if (julian_date < JULIAN_DATE_OFFSET_1996)
    utc_offset = 10;
  else if (julian_date < JULIAN_DATE_OFFSET_1997)
    utc_offset = 11;
  else if (julian_date < JULIAN_DATE_OFFSET_1999)
    utc_offset = 12;
  else if (julian_date < JULIAN_DATE_OFFSET_2006)
    utc_offset = 13;
  else if (julian_date < JULIAN_DATE_OFFSET_2009)
    utc_offset = 14;
  else
    utc_offset = 15;
  /*
   * 12/Sep/2009 (KA): The following program will print out the required julian date for the next leap second (after editting year/month).
   * #include <iostream>
   * #include <iomanip>
   * #include "time_conversion.h"
   * void main() {
   *     double julianDate;
   *     if (GetJulianDateFromUTCTime(2008, 12, 31, 23, 59, 60.0, &julianDate))
   *         std::cout << "utc:2008-12-31 23:59:60 --> julian:" << std::setprecision(12) << julianDate << std::endl;
   * }
   */

  return true;
}

[[nodiscard]] bool e57::utils::number_days_in_month(const unsigned short year,         //!< Universal Time Coordinated    [year]
                                                    const unsigned char  month,        //!< Universal Time Coordinated    [1-12 months]
                                                    unsigned char&       days_in_month //!< Days in the specified month   [1-28|29|30|31 days]
                                                    ) noexcept
{
  const bool    is_a_leapyear = e57::utils::is_leap_year(year);
  unsigned char utmp{0};

  switch (month)
  {
  case 1:
    utmp = DAYS_IN_JAN;
    break;
  case 2:
    if (is_a_leapyear)
    {
      utmp = 29;
    }
    else
    {
      utmp = 28;
    }
    break;
  case 3:
    utmp = DAYS_IN_MAR;
    break;
  case 4:
    utmp = DAYS_IN_APR;
    break;
  case 5:
    utmp = DAYS_IN_MAY;
    break;
  case 6:
    utmp = DAYS_IN_JUN;
    break;
  case 7:
    utmp = DAYS_IN_JUL;
    break;
  case 8:
    utmp = DAYS_IN_AUG;
    break;
  case 9:
    utmp = DAYS_IN_SEP;
    break;
  case 10:
    utmp = DAYS_IN_OCT;
    break;
  case 11:
    utmp = DAYS_IN_NOV;
    break;
  case 12:
    utmp = DAYS_IN_DEC;
    break;
  default: {
    ERROR_MESSAGE("unexpected default case.");
    return false;
    break;
  }
  }

  days_in_month = utmp;

  return true;
}

[[nodiscard]] bool e57::utils::is_leap_year(const unsigned short year) noexcept
{
  bool is_a_leap_year{false};

  if ((year % 4) == 0)
  {
    is_a_leap_year = true;
    if ((year % 100) == 0)
    {
      if ((year % 400) == 0)
      {
        is_a_leap_year = true;
      }
      else
      {
        is_a_leap_year = false;
      }
    }
  }

  if (is_a_leap_year)
  {
    return true;
  }

  return false;
}

[[nodiscard]] bool e57::utils::day_of_year(const unsigned short utc_year,   // Universal Time Coordinated           [year]
                                           const unsigned char  utc_month,  // Universal Time Coordinated           [1-12 months]
                                           const unsigned char  utc_day,    // Universal Time Coordinated           [1-31 days]
                                           unsigned short&      day_of_year // the day of the year (1-366) [days]
                                           ) noexcept
{
  unsigned char days_in_feb{0};
  if (!e57::utils::number_days_in_month(utc_year, 2, days_in_feb))
  {
    ERROR_MESSAGE("number_days_in_month returned false.");
    return false;
  }

  switch (utc_month)
  {
  case 1:
    day_of_year = utc_day;
    break;
  case 2:
    day_of_year = (unsigned short)(DAYS_IN_JAN + utc_day);
    break;
  case 3:
    day_of_year = (unsigned short)(DAYS_IN_JAN + days_in_feb + utc_day);
    break;
  case 4:
    day_of_year = (unsigned short)(62 + days_in_feb + utc_day);
    break;
  case 5:
    day_of_year = (unsigned short)(92 + days_in_feb + utc_day);
    break;
  case 6:
    day_of_year = (unsigned short)(123 + days_in_feb + utc_day);
    break;
  case 7:
    day_of_year = (unsigned short)(153 + days_in_feb + utc_day);
    break;
  case 8:
    day_of_year = (unsigned short)(184 + days_in_feb + utc_day);
    break;
  case 9:
    day_of_year = (unsigned short)(215 + days_in_feb + utc_day);
    break;
  case 10:
    day_of_year = (unsigned short)(245 + days_in_feb + utc_day);
    break;
  case 11:
    day_of_year = (unsigned short)(276 + days_in_feb + utc_day);
    break;
  case 12:
    day_of_year = (unsigned short)(306 + days_in_feb + utc_day);
    break;
  default: {
    ERROR_MESSAGE("unexpected default case.");
    return false;
    break;
  }
  }

  return true;
}

[[nodiscard]] bool e57::utils::gps_time_from_year_and_day_of_year(const unsigned short year,        // The year [year]
                                                                  const unsigned short day_of_year, // The number of days into the year (1-366) [days]
                                                                  unsigned short&      gps_week,    //!< GPS week (0-1024+)            [week]
                                                                  double&              gps_tow      //!< GPS time of week (0-604800.0) [s]
                                                                  ) noexcept
{
  double        julian_date{0.0};
  unsigned char utc_offset{};

  if (day_of_year < 1 || day_of_year > 366)
  {
    ERROR_MESSAGE("invalid date of year");
    return false;
  }

  if (!e57::utils::julian_date_from_utc_time(year, 1, 1, 0, 0, 0, julian_date))
  {
    ERROR_MESSAGE("julian_date_from_utc_time returned false.");
    return false;
  }

  if (!e57::utils::determine_utc_offset(julian_date, utc_offset))
  {
    ERROR_MESSAGE("determine_utc_offset returned false.");
    return false;
  }

  julian_date += day_of_year - 1; // at the start of the day so -1.

  if (!e57::utils::gps_time_from_julian_date(julian_date, utc_offset, gps_week, gps_tow))
  {
    ERROR_MESSAGE("gps_time_from_julian_date returned false.");
    return false;
  }

  return true;
}

[[nodiscard]] bool e57::utils::is_utc_time_valid(const unsigned short utc_year,   //!< Universal Time Coordinated  [year]
                                                 const unsigned char  utc_month,  //!< Universal Time Coordinated  [1-12 months]
                                                 const unsigned char  utc_day,    //!< Universal Time Coordinated  [1-31 days]
                                                 const unsigned char  utc_hour,   //!< Universal Time Coordinated  [hours]
                                                 const unsigned char  utc_minute, //!< Universal Time Coordinated  [minutes]
                                                 const float          utc_seconds //!< Universal Time Coordinated  [s]
                                                 ) noexcept
{
  unsigned char daysInMonth;
  if (utc_month == 0 || utc_month > 12)
  {
    ERROR_MESSAGE("invalid utc_month value");
    return false;
  }
  if (utc_hour > 23)
  {
    ERROR_MESSAGE("invalid utc_hour value");
    return false;
  }
  if (utc_minute > 59)
  {
    ERROR_MESSAGE("invalid utc_minute value");
    return false;
  }
  if (utc_seconds > 60)
  {
    ERROR_MESSAGE("invalid utc_seconds value");
    return false;
  }

  if (!e57::utils::number_days_in_month(utc_year, utc_month, daysInMonth))
  {
    ERROR_MESSAGE("number_days_in_month returned false.");
    return false;
  }

  if (utc_day == 0 || utc_day > daysInMonth)
  {
    ERROR_MESSAGE("if( utc_day == 0 || utc_day > daysInMonth )");
    return false;
  }

  return true;
}

[[nodiscard]] bool e57::utils::gps_time_to_value(const unsigned short gps_week, //!< GPS week (0-1024+)            [week]
                                                 const double         gps_tow,  //!< GPS time of week (0-604800.0) [s])
                                                 double&              gps_time  //!< GPS time expressed as a single double value)
                                                 ) noexcept
{
  if (gps_tow < 0.0 || gps_tow > SECONDS_IN_A_WEEK)
  {
    ERROR_MESSAGE("if( gps_tow < 0.0  || gps_tow > 604800.0 )");
    return false;
  }

  gps_time = (gps_week * SECONDS_IN_A_WEEK) + gps_tow;

  return true;
}

[[nodiscard]] bool e57::utils::gps_time_from_value(const double    gps_time, //!< GPS time expressed as a single double value)
                                                   unsigned short& gps_week, //!< GPS week (0-1024+)            [week]
                                                   double&         gps_tow   //!< GPS time of week (0-604800.0) [s])
                                                   ) noexcept
{
  if (gps_time <= 0.0)
  {
    ERROR_MESSAGE("if( gps_time <= 0.0");
    return false;
  }

  gps_week = static_cast<int32_t>(floor(gps_time)) / SECONDS_IN_A_WEEK;
  gps_tow  = gps_time - gps_week * SECONDS_IN_A_WEEK;
  return true;
}