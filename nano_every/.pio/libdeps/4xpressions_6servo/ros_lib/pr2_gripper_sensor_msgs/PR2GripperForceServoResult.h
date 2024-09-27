#ifndef _ROS_pr2_gripper_sensor_msgs_PR2GripperForceServoResult_h
#define _ROS_pr2_gripper_sensor_msgs_PR2GripperForceServoResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "pr2_gripper_sensor_msgs/PR2GripperForceServoData.h"

namespace pr2_gripper_sensor_msgs
{

  class PR2GripperForceServoResult : public ros::Msg
  {
    public:
      typedef pr2_gripper_sensor_msgs::PR2GripperForceServoData _data_type;
      _data_type data;

    PR2GripperForceServoResult():
      data()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->data.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->data.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "pr2_gripper_sensor_msgs/PR2GripperForceServoResult"; };
    virtual const char * getMD5() override { return "a85c0d43537b45945527f5de565ab7c2"; };

  };

}
#endif
