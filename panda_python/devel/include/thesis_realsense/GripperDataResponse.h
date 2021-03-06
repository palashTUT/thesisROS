// Generated by gencpp from file thesis_realsense/GripperDataResponse.msg
// DO NOT EDIT!


#ifndef THESIS_REALSENSE_MESSAGE_GRIPPERDATARESPONSE_H
#define THESIS_REALSENSE_MESSAGE_GRIPPERDATARESPONSE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace thesis_realsense
{
template <class ContainerAllocator>
struct GripperDataResponse_
{
  typedef GripperDataResponse_<ContainerAllocator> Type;

  GripperDataResponse_()
    : grasp_result(false)  {
    }
  GripperDataResponse_(const ContainerAllocator& _alloc)
    : grasp_result(false)  {
  (void)_alloc;
    }



   typedef uint8_t _grasp_result_type;
  _grasp_result_type grasp_result;





  typedef boost::shared_ptr< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> const> ConstPtr;

}; // struct GripperDataResponse_

typedef ::thesis_realsense::GripperDataResponse_<std::allocator<void> > GripperDataResponse;

typedef boost::shared_ptr< ::thesis_realsense::GripperDataResponse > GripperDataResponsePtr;
typedef boost::shared_ptr< ::thesis_realsense::GripperDataResponse const> GripperDataResponseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::thesis_realsense::GripperDataResponse_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace thesis_realsense

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "26f477d83c816f35d1c952351d253e10";
  }

  static const char* value(const ::thesis_realsense::GripperDataResponse_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x26f477d83c816f35ULL;
  static const uint64_t static_value2 = 0xd1c952351d253e10ULL;
};

template<class ContainerAllocator>
struct DataType< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "thesis_realsense/GripperDataResponse";
  }

  static const char* value(const ::thesis_realsense::GripperDataResponse_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "bool grasp_result\n\
";
  }

  static const char* value(const ::thesis_realsense::GripperDataResponse_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.grasp_result);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct GripperDataResponse_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::thesis_realsense::GripperDataResponse_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::thesis_realsense::GripperDataResponse_<ContainerAllocator>& v)
  {
    s << indent << "grasp_result: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.grasp_result);
  }
};

} // namespace message_operations
} // namespace ros

#endif // THESIS_REALSENSE_MESSAGE_GRIPPERDATARESPONSE_H
