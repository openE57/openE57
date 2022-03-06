#include <iostream>
#include <math.h>
#include <openE57/impl/time_conversion.h>
#include <sys/timeb.h>
#include <time.h>

#define GNSS_ERROR_MSG(msg)                                                                                                                                    \
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

// Constants definition
constexpr const double SECONDS_IN_A_DAY              = 86400.0;
constexpr const double SECONDS_IN_A_WEEK             = SECONDS_IN_A_DAY * 7;
constexpr const double JULIAN_DATE_START_OF_GPS_TIME = 2444244.5; // [days]
constexpr const double JULIAN_DATE_START_OF_PC_TIME  = 2440587.5; // [days]
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

bool e57::utils::current_system_time(
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
)
{
#if defined(WIN32) && !defined(__GNUC__)
  struct _timeb timebuffer; // found in <sys/timeb.h>
#else
  struct timeb timebuffer;
#endif
  double timebuffer_time_in_days;
  double timebuffer_time_in_seconds;

#ifdef _CRT_SECURE_NO_DEPRECATE
  if (_ftime_s(&timebuffer) != 0)
  {
    GNSS_ERROR_MSG("if( _ftime_s( &timebuffer ) != 0 )");
    return FALSE;
  }
#else

#  if defined(WIN32) && !defined(__GNUC__)
  _ftime64_s(&timebuffer);
#  else
  ftime(&timebuffer);
#  endif

#endif
  timebuffer_time_in_seconds = timebuffer.time + timebuffer.millitm / 1000.0; // [s] with ms resolution

  // timebuffer_time_in_seconds is the time in seconds since midnight (00:00:00), January 1, 1970,
  // coordinated universal time (UTC). Julian date for (00:00:00), January 1, 1970 is: 2440587.5 [days]

  // convert timebuffer.time from seconds to days
  timebuffer_time_in_days = timebuffer_time_in_seconds / SECONDS_IN_A_DAY; // days since julian date 2440587.5000000 [days]

  // convert to julian date
  julian_date = JULIAN_DATE_START_OF_PC_TIME + timebuffer_time_in_days;

  bool result = e57::utils::determine_utc_offset(julian_date, utc_offset);
  if (!result)
  {
    GNSS_ERROR_MSG("determine_utc_offset returned false.");
    return result;
  }

  result = e57::utils::gps_time_from_julian_date(julian_date, utc_offset, gps_week, gps_tow);
  if (!result)
  {
    GNSS_ERROR_MSG("gps_time_from_julian_date returned false.");
    return result;
  }

  result = e57::utils::utc_time_from_julian_date(julian_date, utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds);
  if (!result)
  {
    GNSS_ERROR_MSG("utc_time_from_julian_date returned false");
    return result;
  }

  return result;
}

bool e57::utils::day_of_week_from_julian_date(const double   julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
                                              unsigned char& day_of_week  //!< 0-Sunday, 1-Monday, 2-Tuesday, 3-Wednesday, 4-Thursday, 5-Friday, 6-Saturday [].
)
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

bool e57::utils::julian_date_from_gps_time(const unsigned short gps_week,   //!< GPS week (0-1024+)             [week]
                                           const double         gps_tow,    //!< GPS time of week (0-604800.0)  [s]
                                           const unsigned char  utc_offset, //!< Integer seconds that GPS is ahead of UTC time, always positive [s]
                                           double& julian_date //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
)
{
  if (gps_tow < 0.0 || gps_tow > SECONDS_IN_A_WEEK)
  {
    GNSS_ERROR_MSG("if( gps_tow < 0.0  || gps_tow > 604800.0 )");
    return false;
  }

  // GPS time is ahead of UTC time and Julian time by the UTC offset
  julian_date = (static_cast<double>(gps_week) + (gps_tow - static_cast<double>(utc_offset)) / SECONDS_IN_A_WEEK) * 7.0 + JULIAN_DATE_START_OF_GPS_TIME;
  return true;
}

bool e57::utils::julian_date_from_utc_time(const unsigned short utc_year,    //!< Universal Time Coordinated  [year]
                                           const unsigned char  utc_month,   //!< Universal Time Coordinated  [1-12 months]
                                           const unsigned char  utc_day,     //!< Universal Time Coordinated  [1-31 days]
                                           const unsigned char  utc_hour,    //!< Universal Time Coordinated  [hours]
                                           const unsigned char  utc_minute,  //!< Universal Time Coordinated  [minutes]
                                           const float          utc_seconds, //!< Universal Time Coordinated  [s]
                                           double& julian_date //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
)
{
  double y{0.0}; // temp for year
  double m{0.0}; // temp for month

  // Check the input.
  bool result = e57::utils::is_utc_time_valid(utc_year, utc_month, utc_day, utc_hour, utc_minute, utc_seconds);
  if (!result)
  {
    GNSS_ERROR_MSG("is_utc_time_valid returned false.");
    return result;
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

  julian_date = (int)(365.25 * y) + (int)(30.6001 * (m + 1.0)) + utc_day + utc_hour / 24.0 + utc_minute / 1440.0 + utc_seconds / SECONDS_IN_A_DAY + 1720981.5;
  return result;
}

bool e57::utils::gps_time_from_julian_date(const double julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
                                           const unsigned char utc_offset, //!< Integer seconds that GPS is ahead of UTC time, always positive [s]
                                           unsigned short&     gps_week,   //!< GPS week (0-1024+)            [week]
                                           double&             gps_tow     //!< GPS time of week [s]
)
{
  // Check the input.
  if (julian_date < 0.0)
  {
    GNSS_ERROR_MSG("julian_date is negative");
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

bool e57::utils::utc_time_from_julian_date(const double    julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
                                           unsigned short& utc_year,    //!< Universal Time Coordinated    [year]
                                           unsigned char&  utc_month,   //!< Universal Time Coordinated    [1-12 months]
                                           unsigned char&  utc_day,     //!< Universal Time Coordinated    [1-31 days]
                                           unsigned char&  utc_hour,    //!< Universal Time Coordinated    [hours]
                                           unsigned char&  utc_minute,  //!< Universal Time Coordinated    [minutes]
                                           float&          utc_seconds  //!< Universal Time Coordinated    [s]
)
{
  // Check the input.
  if (julian_date < 0.0)
  {
    GNSS_ERROR_MSG("julian_date is negative");
    return false;
  }

  unsigned char  days_in_month = 0;

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

  unsigned char month   = static_cast<unsigned char>(e - 1 - 12 * static_cast<int>(e / 14));
  unsigned short year    = static_cast<unsigned short>(c - 4715 - static_cast<int>((7.0 + static_cast<double>(month)) / 10.0));

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

        bool result = number_days_in_month(year, month, days_in_month);
        if (!result)
        {
          GNSS_ERROR_MSG("number_days_in_month returned false.");
          return result;
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
  utc_seconds = (float)seconds;

  return true;
}