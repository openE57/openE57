#ifndef OPEN57_API_H
#define OPEN57_API_H

#ifndef OPENE57_H_INCLUDED
#  include <openE57/openE57.h>
#endif

#ifndef PI
constexpr double PI{3.1415926535897932384626433832795}; // from C++20, use <numbers>
#endif

namespace e57
{
namespace core
{
  /**
   * @brief The Translation structure defines a rigid body translation in Cartesian coordinates.
   */
  struct Translation
  {
    double x{0.0}; //!< The X coordinate of the translation (in meters)
    double y{0.0}; //!< The Y coordinate of the translation (in meters)
    double z{0.0}; //!< The Z coordinate of the translation (in meters)
  };

  /**
   * @brief The Quaternion structure is a quaternion which represents a rigid body rotation.
   *
   */
  struct Quaternion
  {
    double w{0.0}; //!< The real part of the quaternion. Shall be nonnegative
    double x{0.0}; //!< The i coefficient of the quaternion
    double y{0.0}; //!< The j coefficient of the quaternion
    double z{0.0}; //!< The k coefficient of the quaternion
  };

  /**
   * @brief The RigidBodyTransform is a structure that defines a rigid body transform in cartesian coordinates.
   */
  struct RigidBodyTransform
  {
    Quaternion  rotation{};    //!< A unit quaternion representing the rotation, R, of the transform
    Translation translation{}; //!< The translation point vector, t, of the transform
  };

  /**
   * @brief The CartesianBounds structure specifies an axis-aligned box in local cartesian coordinates.
   */
  struct CartesianBounds
  {
    double xMinimum{0.0}; //!< The minimum extent of the bounding box in the X direction
    double xMaximum{0.0}; //!< The maximum extent of the bounding box in the X direction
    double yMinimum{0.0}; //!< The minimum extent of the bounding box in the Y direction
    double yMaximum{0.0}; //!< The maximum extent of the bounding box in the Y direction
    double zMinimum{0.0}; //!< The minimum extent of the bounding box in the Z direction
    double zMaximum{0.0}; //!< The maximum extent of the bounding box in the Z direction
  };

  /**
   * @brief The SphericalBounds structure stores the bounds of some data in spherical coordinates.
   */
  struct SphericalBounds
  {
    double rangeMinimum{0.0};     //!< The minimum extent of the bounding region in the r direction
    double rangeMaximum{0.0};     //!< The maximum extent of the bounding region in the r direction
    double elevationMinimum{0.0}; //!< The minimum extent of the bounding region from the horizontal plane
    double elevationMaximum{0.0}; //!< The maximum extent of the bounding region from the horizontal plane
    double azimuthStart{0.0};     //!< The starting azimuth angle defining the extent of the bounding region around the z axis
    double azimuthEnd{0.0};       //!< The ending azimuth angle defining the extent of the bounding region around the z axix
  };

  /**
   * @brief The IndexBounds structure stores the minimum and maximum of rowIndex, columnIndex, and returnIndex fields for a set of points.
   */
  struct IndexBounds
  {
    int64_t rowMinimum{0};    //!< The minimum rowIndex value of any point represented by this IndexBounds object.
    int64_t rowMaximum{0};    //!< The maximum rowIndex value of any point represented by this IndexBounds object.
    int64_t columnMinimum{0}; //!< The minimum columnIndex value of any point represented by this IndexBounds object.
    int64_t columnMaximum{0}; //!< The maximum columnIndex value of any point represented by this IndexBounds object.
    int64_t returnMinimum{0}; //!< The minimum returnIndex value of any point represented by this IndexBounds object.
    int64_t returnMaximum{0}; //!< The maximum returnIndex value of any point represented by this IndexBounds object.
  };

  /**
   * @brief The IntensityLimits Structure specifies the limits for the value of signal intensity that a sensor is capable of producing
   */
  struct IntensityLimits
  {
    double intensityMinimum{0.0}; //!< The minimum producible intensity value. Unit is unspecified.
    double intensityMaximum{0.0}; //!< The maximum producible intensity value. Unit is unspecified.
  };

  /**
   * @brief The ColorLimits structure specifies the limits for the value of red, green, and blue color that a sensor is capable of producing.
   */
  struct ColorLimits
  {
    double colorRedMinimum{0.0};   //!< The minimum producible red color value. Unit is unspecified.
    double colorRedMaximum{0.0};   //!< The maximum producible red color value. Unit is unspecified.
    double colorGreenMinimum{0.0}; //!< The minimum producible green color value. Unit is unspecified.
    double colorGreenMaximum{0.0}; //!< The maximum producible green color value. Unit is unspecified.
    double colorBlueMinimum{0.0};  //!< The minimum producible blue color value. Unit is unspecified.
    double colorBlueMaximum{0.0};  //!< The maximum producible blue color value. Unit is unspecified.
  };

  /**
   * @brief The UtcTime structure defines a valid UTC Date Time representation.
   */
  struct UtcTime
  {
    uint32_t year{1970};    //!< The year 1900-9999
    uint32_t month{1};      //!< The month 0-11
    uint32_t day{1};        //!< The day 1-31
    uint32_t hour{0};       //!< The hour 0-23
    uint32_t minute{0};     //!< The minute 0-59
    float    seconds{0.0f}; //!< The seconds 0.0 - 59.999
  };

  /**
   * @brief The GpsTime structure defines a valid GPS Time representation.
   */
  struct GpsTime
  {
    uint32_t gps_week{0};  //!< GPS week (0-1024+)
    double   gps_tow{0.0}; //!< GPS time of week (0-604800.0)
  };
} // namespace core

namespace utils
{
  /**
   * @brief This function returns the current GPS time as core::GpsTime.
   *
   * @return the current GPS Time as core::GpsTime
   */
  [[nodiscard]] core::GpsTime current_gps_time() noexcept;

  /**
   * @brief This function returns the current UTC time as core::UtcTime.
   * @details The date and time is encoded using a single
   * floating point number, stored as an E57 Float element which is based on the Global Positioning
   * System (GPS) time scale.
   *
   * @return the current Date/Time as core::UtcTime
   */
  [[nodiscard]] core::UtcTime current_utc_time() noexcept;

  /**
   * @brief This function converts the date and time from the double dateTimeValue.
   * @details The date and time is encoded using a single
   * floating point number, stored as an E57 Float element which is based on the Global Positioning
   * System (GPS) time scale.
   *
   * @param dateTimeValue the value holding the GPS Date/Time value in double format
   * @return the Date/Time as core::UtcTime
   */
  [[nodiscard]] core::UtcTime date_time_from_number(const double dateTimeValue);

  /**
   * @brief This function converts the date and time from the structure core::UtcTime dateTimeValue.
   * @details The date and time is encoded using a single
   * floating point number, stored as an E57 Float element which is based on the Global Positioning
   * System (GPS) time scale.
   *
   * @param dateTimeValue the value holding the GPS Date/Time value as core::UtcTime structure
   * @return the Date/Time as double
   */
  [[nodiscard]] double date_time_number_from_value(const core::UtcTime& dateTimeValue);

} // namespace utils

} // namespace e57

#endif // OPEN57_API_H