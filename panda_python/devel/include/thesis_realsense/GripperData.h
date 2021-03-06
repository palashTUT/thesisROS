// Generated by gencpp from file thesis_realsense/GripperData.msg
// DO NOT EDIT!


#ifndef THESIS_REALSENSE_MESSAGE_GRIPPERDATA_H
#define THESIS_REALSENSE_MESSAGE_GRIPPERDATA_H

#include <ros/service_traits.h>


#include <thesis_realsense/GripperDataRequest.h>
#include <thesis_realsense/GripperDataResponse.h>


namespace thesis_realsense
{

struct GripperData
{

typedef GripperDataRequest Request;
typedef GripperDataResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct GripperData
} // namespace thesis_realsense


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::thesis_realsense::GripperData > {
  static const char* value()
  {
    return "d34785af14fea5ec34d952bf17a99f4a";
  }

  static const char* value(const ::thesis_realsense::GripperData&) { return value(); }
};

template<>
struct DataType< ::thesis_realsense::GripperData > {
  static const char* value()
  {
    return "thesis_realsense/GripperData";
  }

  static const char* value(const ::thesis_realsense::GripperData&) { return value(); }
};


// service_traits::MD5Sum< ::thesis_realsense::GripperDataRequest> should match 
// service_traits::MD5Sum< ::thesis_realsense::GripperData > 
template<>
struct MD5Sum< ::thesis_realsense::GripperDataRequest>
{
  static const char* value()
  {
    return MD5Sum< ::thesis_realsense::GripperData >::value();
  }
  static const char* value(const ::thesis_realsense::GripperDataRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::thesis_realsense::GripperDataRequest> should match 
// service_traits::DataType< ::thesis_realsense::GripperData > 
template<>
struct DataType< ::thesis_realsense::GripperDataRequest>
{
  static const char* value()
  {
    return DataType< ::thesis_realsense::GripperData >::value();
  }
  static const char* value(const ::thesis_realsense::GripperDataRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::thesis_realsense::GripperDataResponse> should match 
// service_traits::MD5Sum< ::thesis_realsense::GripperData > 
template<>
struct MD5Sum< ::thesis_realsense::GripperDataResponse>
{
  static const char* value()
  {
    return MD5Sum< ::thesis_realsense::GripperData >::value();
  }
  static const char* value(const ::thesis_realsense::GripperDataResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::thesis_realsense::GripperDataResponse> should match 
// service_traits::DataType< ::thesis_realsense::GripperData > 
template<>
struct DataType< ::thesis_realsense::GripperDataResponse>
{
  static const char* value()
  {
    return DataType< ::thesis_realsense::GripperData >::value();
  }
  static const char* value(const ::thesis_realsense::GripperDataResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // THESIS_REALSENSE_MESSAGE_GRIPPERDATA_H
