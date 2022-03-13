/**
\b REFERENCES \n
- Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
  Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42 \n
- http://aa.usno.navy.mil/data/docs/JulianDate.html - Julian Date Converter \n
- http://aa.usno.navy.mil/faq/docs/UT.html \n
- http://wwwmacho.mcmaster.ca/JAVA/JD.html \n
- Raquet, J. F. (2002), GPS Receiver Design Lecture Notes. Geomatics Engineering,
  University of Calgary Graduate Course. \n
*/

#ifndef TIME_CONVERSION_H
#define TIME_CONVERSION_H

namespace e57
{
namespace utils
{
  /**
   * @brief Obtains the UTC time, GPS time, and Julian date from PC system time.
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool current_system_time(
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
    ) noexcept;

  /**
   * @brief Computes the day of the week from the Julian date.
   * @details http://en.wikipedia.org/wiki/Julian_day
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool day_of_week_from_julian_date(
    const double   julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
    unsigned char& day_of_week  //!< 0-Sunday, 1-Monday, 2-Tuesday, 3-Wednesday, 4-Thursday, 5-Friday, 6-Saturday [].
    ) noexcept;

  /**
   * @brief Computes the Julian date from GPS time
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool julian_date_from_gps_time(const unsigned short gps_week,   //!< GPS week (0-1024+)             [week]
                                               const double         gps_tow,    //!< GPS time of week (0-604800.0)  [s]
                                               const unsigned char  utc_offset, //!< Integer seconds that GPS is ahead of UTC time, always positive [s]
                                               double& julian_date //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
                                               ) noexcept;

  /**
   * @brief Computes the Julian date from UTC time
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42
   * @remarks Verified calculation using http://aa.usno.navy.mil/data/docs/JulianDate.html,
   * a Julian Date Converter and http://wwwmacho.mcmaster.ca/JAVA/JD.html,
   * another online converter tool. \n
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool julian_date_from_utc_time(const unsigned short utc_year,    //!< Universal Time Coordinated  [year]
                                               const unsigned char  utc_month,   //!< Universal Time Coordinated  [1-12 months]
                                               const unsigned char  utc_day,     //!< Universal Time Coordinated  [1-31 days]
                                               const unsigned char  utc_hour,    //!< Universal Time Coordinated  [hours]
                                               const unsigned char  utc_minute,  //!< Universal Time Coordinated  [minutes]
                                               const float          utc_seconds, //!< Universal Time Coordinated  [s]
                                               double& julian_date //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
                                               ) noexcept;

  /**
   * @brief Computes GPS time from the Julian date
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool gps_time_from_julian_date(const double julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
                                               const unsigned char utc_offset, //!< Integer seconds that GPS is ahead of UTC time, always positive [s]
                                               unsigned short&     gps_week,   //!< GPS week (0-1024+)            [week]
                                               double&             gps_tow     //!< GPS time of week [s]
                                               ) noexcept;

  /**
   * @brief Computes UTC time from the Julian date
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool utc_time_from_julian_date(const double julian_date,  //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
                                               unsigned short& utc_year,  //!< Universal Time Coordinated    [year]
                                               unsigned char&  utc_month, //!< Universal Time Coordinated    [1-12 months]
                                               unsigned char&  utc_day,   //!< Universal Time Coordinated    [1-31 days]
                                               unsigned char&  utc_hour,  //!< Universal Time Coordinated    [hours]
                                               unsigned char&  utc_minute, //!< Universal Time Coordinated    [minutes]
                                               float&          utc_seconds //!< Universal Time Coordinated    [s]
                                               ) noexcept;

  /**
   * @brief Computes GPS time from UTC time
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42
   * @remarks The utc offset is determined automatically from a look up table
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool gps_time_from_utc_time(const unsigned short utc_year,    //!< Universal Time Coordinated    [year]
                                            const unsigned char  utc_month,   //!< Universal Time Coordinated    [1-12 months]
                                            const unsigned char  utc_day,     //!< Universal Time Coordinated    [1-31 days]
                                            const unsigned char  utc_hour,    //!< Universal Time Coordinated    [hours]
                                            const unsigned char  utc_minute,  //!< Universal Time Coordinated    [minutes]
                                            const float          utc_seconds, //!< Universal Time Coordinated    [s]
                                            unsigned short&      gps_week,    //!< GPS week (0-1024+)            [week]
                                            double&              gps_tow      //!< GPS time of week (0-604800.0) [s]
                                            ) noexcept;

  /**
   * @brief Computes GPS time from RINEX time. RINEX time looks like UTC but it is GPS time in year, month, day, hours, minutes, seconds.
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42.
   * RINEX version 2.11, (http://www.aiub-download.unibe.ch/rinex/rinex211.txt)
   * @remarks There is no UTC offset to apply. The RINEX time system must be the GPS Time system to use this function.
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool gps_time_from_rinex_time(const unsigned short utc_year,    //!< Universal Time Coordinated    [year]
                                              const unsigned char  utc_month,   //!< Universal Time Coordinated    [1-12 months]
                                              const unsigned char  utc_day,     //!< Universal Time Coordinated    [1-31 days]
                                              const unsigned char  utc_hour,    //!< Universal Time Coordinated    [hours]
                                              const unsigned char  utc_minute,  //!< Universal Time Coordinated    [minutes]
                                              const float          utc_seconds, //!< Universal Time Coordinated    [s]
                                              unsigned short&      gps_week,    //!< GPS week (0-1024+)            [week]
                                              double&              gps_tow      //!< GPS time of week (0-604800.0) [s]
                                              ) noexcept;

  /**
   * @brief Computes UTC time from GPS time.
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42.
   * @remarks The utc offset is determined automatically from a look up table
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool utc_time_from_gps_time(const unsigned short gps_week,   //!< GPS week (0-1024+)            [week]
                                            const double         gps_tow,    //!< GPS time of week (0-604800.0) [s]
                                            unsigned short&      utc_year,   //!< Universal Time Coordinated    [year]
                                            unsigned char&       utc_month,  //!< Universal Time Coordinated    [1-12 months]
                                            unsigned char&       utc_day,    //!< Universal Time Coordinated    [1-31 days]
                                            unsigned char&       utc_hour,   //!< Universal Time Coordinated    [hours]
                                            unsigned char&       utc_minute, //!< Universal Time Coordinated    [minutes]
                                            float&               utc_seconds //!< Universal Time Coordinated    [s]
                                            ) noexcept;

  /**
   * @brief This function is a look up table to determine the UTC offset from the Julian Date.
   * @details Raquet, J. F. (2002), GPS Receiver Design Lecture Notes. Geomatics Engineering,
   * University of Calgary Graduate Course.
   * @remarks This function must be updated when the next UTC utc_offset step occurs. Current max is (13).
   * \b "Offset Table" \n
   * UTCOffset, UTC Date, Julian Date [days] \n
   * 0,    Jan 06 1980 00:00:00.0,    2444244.5000 \n
   * 1,    Jul 01 1981 00:00:00.0,    2444786.5000 \n
   * 2,    Jul 01 1982 00:00:00.0,    2445151.5000 \n
   * 3,    Jul 01 1983 00:00:00.0,    2445516.5000 \n
   * 4,    Jul 01 1985 00:00:00.0,    2446247.5000 \n
   * 5,    Jan 01 1988 00:00:00.0,    2447161.5000 \n
   * 6,    Jan 01 1990 00:00:00.0,    2447892.5000 \n
   * 7,    Jan 01 1991 00:00:00.0,    2448257.5000 \n
   * 8,    Jul 01 1992 00:00:00.0,    2448804.5000 \n
   * 9,    Jul 01 1993 00:00:00.0,    2449169.5000 \n
   * 10,   Jul 01 1994 00:00:00.0,    2449534.5000 \n
   * 11,   Jan 01 1996 00:00:00.0,    2450083.5000 \n
   * 12,   Jul 01 1997 00:00:00.0,    2450630.5000 \n
   * 13,   Jan 01 1999 00:00:00.0,    2451179.5000 \n
   * 14,   Jan 01 2006 00:00:00.0,    2453736.5000 \n
   * 15,   Jan 01 2009 00:00:00.0,    2454832.5000 \n
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool determine_utc_offset(
    const double   julian_date, //!< Number of days since noon Universal Time Jan 1, 4713 BCE (Julian calendar) [days]
    unsigned char& utc_offset   //!< Integer seconds that GPS is ahead of UTC time, always positive             [s], obtained from a look up table
    ) noexcept;

  /**
   * @brief Determines the number of days in a month, given the month and year.
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42.
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool number_days_in_month(const unsigned short year,         //!< Universal Time Coordinated    [year]
                                          const unsigned char  month,        //!< Universal Time Coordinated    [1-12 months]
                                          unsigned char&       days_in_month //!< Days in the specified month   [1-28|29|30|31 days]
                                          ) noexcept;

  /**
   * @brief Determines if the given year is a leap year
   * @details Hofmann-Wellenhof, B., H. Lichtenegger, and J. Collins (1994). GPS Theory and
   * Practice, Third, revised edition. Springer-Verlag, Wien New York. pp. 38-42.
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool is_leap_year(const unsigned short year) noexcept;

  /**
   * @brief Determines the number of day in year given the year, month, and day
   * @remarks Performed independant comparison with http://www.mbari.org/staff/coletti/doytable.html
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool day_of_year(const unsigned short utc_year,   // Universal Time Coordinated           [year]
                                 const unsigned char  utc_month,  // Universal Time Coordinated           [1-12 months]
                                 const unsigned char  utc_day,    // Universal Time Coordinated           [1-31 days]
                                 unsigned short&      day_of_year // number of days into the year (1-366) [days]
                                 ) noexcept;

  /**
   * @brief Determines the GPS time of the start of a day from the day of year and the year.
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool gps_time_from_year_and_day_of_year(const unsigned short year,        // The year [year]
                                                        const unsigned short day_of_year, // The number of days into the year (1-366) [days]
                                                        unsigned short&      gps_week,    //!< GPS week (0-1024+)            [week]
                                                        double&              gps_tow      //!< GPS time of week (0-604800.0) [s]
                                                        ) noexcept;

  /**
   * @brief Determines if the UTC input values are valid.
   *
   * @return true if successful, false otherwise
   */
  [[nodiscard]] bool is_utc_time_valid(const unsigned short utc_year,   //!< Universal Time Coordinated  [year]
                                       const unsigned char  utc_month,  //!< Universal Time Coordinated  [1-12 months]
                                       const unsigned char  utc_day,    //!< Universal Time Coordinated  [1-31 days]
                                       const unsigned char  utc_hour,   //!< Universal Time Coordinated  [hours]
                                       const unsigned char  utc_minute, //!< Universal Time Coordinated  [minutes]
                                       const float          utc_seconds //!< Universal Time Coordinated  [s]
                                       ) noexcept;
} // namespace utils

} // namespace e57

#endif // TIME_CONVERSION_H
