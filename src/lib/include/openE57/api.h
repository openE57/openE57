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
namespace definitions
{
  /**
   * @brief The Translation structure defines a rigid body translation in Cartesian coordinates.
   */
  struct Translation
  {
    double x; //!< The X coordinate of the translation (in meters)
    double y; //!< The Y coordinate of the translation (in meters)
    double z; //!< The Z coordinate of the translation (in meters)
  };

  /**
   * @brief The Quaternion structure is a quaternion which represents a rigid body rotation.
   *
   */
  struct Quaternion
  {
    double w; //!< The real part of the quaternion. Shall be nonnegative
    double x; //!< The i coefficient of the quaternion
    double y; //!< The j coefficient of the quaternion
    double z; //!< The k coefficient of the quaternion
  };

  /**
   * @brief The RigidBodyTransform is a structure that defines a rigid body transform in cartesian coordinates.
   */
  struct RigidBodyTransform
  {
    Quaternion  rotation;    //!< A unit quaternion representing the rotation, R, of the transform
    Translation translation; //!< The translation point vector, t, of the transform
  };

  /**
   * @brief The CartesianBounds structure specifies an axis-aligned box in local cartesian coordinates.
   */
  struct CartesianBounds
  {
    double xMinimum; //!< The minimum extent of the bounding box in the X direction
    double xMaximum; //!< The maximum extent of the bounding box in the X direction
    double yMinimum; //!< The minimum extent of the bounding box in the Y direction
    double yMaximum; //!< The maximum extent of the bounding box in the Y direction
    double zMinimum; //!< The minimum extent of the bounding box in the Z direction
    double zMaximum; //!< The maximum extent of the bounding box in the Z direction
  };

  /**
   * @brief The SphericalBounds structure stores the bounds of some data in spherical coordinates.
   */
  struct SphericalBounds
  {
    double rangeMinimum;     //!< The minimum extent of the bounding region in the r direction
    double rangeMaximum;     //!< The maximum extent of the bounding region in the r direction
    double elevationMinimum; //!< The minimum extent of the bounding region from the horizontal plane
    double elevationMaximum; //!< The maximum extent of the bounding region from the horizontal plane
    double azimuthStart;     //!< The starting azimuth angle defining the extent of the bounding region around the z axis
    double azimuthEnd;       //!< The ending azimuth angle defining the extent of the bounding region around the z axix
  };

  /**
   * @brief The IndexBounds structure stores the minimum and maximum of rowIndex, columnIndex, and returnIndex fields for a set of points.
   */
  struct IndexBounds
  {
    int64_t rowMinimum;    //!< The minimum rowIndex value of any point represented by this IndexBounds object.
    int64_t rowMaximum;    //!< The maximum rowIndex value of any point represented by this IndexBounds object.
    int64_t columnMinimum; //!< The minimum columnIndex value of any point represented by this IndexBounds object.
    int64_t columnMaximum; //!< The maximum columnIndex value of any point represented by this IndexBounds object.
    int64_t returnMinimum; //!< The minimum returnIndex value of any point represented by this IndexBounds object.
    int64_t returnMaximum; //!< The maximum returnIndex value of any point represented by this IndexBounds object.
  };

  /**
   * @brief The IntensityLimits Structure specifies the limits for the value of signal intensity that a sensor is capable of producing
   */
  struct IntensityLimits
  {
    double intensityMinimum; //!< The minimum producible intensity value. Unit is unspecified.
    double intensityMaximum; //!< The maximum producible intensity value. Unit is unspecified.
  };

  /**
   * @brief The ColorLimits structure specifies the limits for the value of red, green, and blue color that a sensor is capable of producing.
   */
  struct ColorLimits
  {
    double colorRedMinimum;   //!< The minimum producible red color value. Unit is unspecified.
    double colorRedMaximum;   //!< The maximum producible red color value. Unit is unspecified.
    double colorGreenMinimum; //!< The minimum producible green color value. Unit is unspecified.
    double colorGreenMaximum; //!< The maximum producible green color value. Unit is unspecified.
    double colorBlueMinimum;  //!< The minimum producible blue color value. Unit is unspecified.
    double colorBlueMaximum;  //!< The maximum producible blue color value. Unit is unspecified.
  };

  /**
   * @brief The DateTime structure defines a valid GPS Date Time representation.
   */
  struct DateTime
  {
    uint32_t year{1970};    //!< The year 1900-9999
    uint32_t month{1};      //!< The month 0-11
    uint32_t day{1};        //!< The day 1-31
    uint32_t hour{0};       //!< The hour 0-23
    uint32_t minute{0};     //!< The minute 0-59
    float    seconds{0.0f}; //!< The seconds 0.0 - 59.999
  };
} // namespace definitions

/**
 * @brief The DateTimeEncoder is a structure for encoding date and time.
 * @details The date and time is encoded using a single
 * 562 floating point number, stored as an E57 Float element which is based on the Global Positioning
 * 563 System (GPS) time scale.
 */
class DateTimeEncoder
{
public:
  /**
   * @brief This function returns the current time as double.
   *
   * @return the current Date/Time as double
   */
  [[nodiscard]] double getCurrentDateTimeAsDouble() const noexcept;

  /**
   * @brief This function returns the current time as definitions::DateTime.
   *
   * @return the current Date/Time as definitions::DateTime
   */
  [[nodiscard]] definitions::DateTime getCurrentDateTime() const noexcept;

  /**
   * @brief This function converts the date and time from the double dateTimeValue.
   *
   * @param dateTimeValue the value holding the GPS Date/Time value in double format
   * @return the Date/Time as definitions::DateTime
   */
  [[nodiscard]] definitions::DateTime convertFromDouble(const double dateTimeValue);

  /**
   * @brief This function converts the date and time from the structure definitions::DateTime dateTimeValue.
   *
   * @param dateTimeValue the value holding the GPS Date/Time value as definitions::DateTime structure
   * @return the Date/Time as double
   */
  [[nodiscard]] double convertFromDateTime(const definitions::DateTime& dateTimeValue);
};

} // namespace e57

#endif // OPEN57_API_H