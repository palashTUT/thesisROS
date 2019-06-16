#include <cmath>
#include <iostream>

#include <franka/duration.h>
#include <franka/exception.h>
#include <franka/model.h>
#include <franka/robot.h>
#include <franka/gripper.h>
#include <franka/rate_limiting.h>
#include <cmath>  // for isnan()
#include <limits> // for quiet_NaN

// for subscribers
#include <thread>
#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include "geometry_msgs/PoseStamped.h"
#include "examples_common.h"

#include "MotionPlanner.h"
#include "thesis_franka/GripperData.h"

using namespace std;


// declaration of global variables
double target_x = std::numeric_limits<double>::quiet_NaN();
double target_y = std::numeric_limits<double>::quiet_NaN();
double target_z = std::numeric_limits<double>::quiet_NaN();
double start_x = std::numeric_limits<double>::quiet_NaN();
double start_y = std::numeric_limits<double>::quiet_NaN();
double start_z = std::numeric_limits<double>::quiet_NaN();
double joint_6_val = std::numeric_limits<double>::quiet_NaN();
double goal_x = std::numeric_limits<double>::quiet_NaN();
double goal_y = std::numeric_limits<double>::quiet_NaN();
double goal_z = std::numeric_limits<double>::quiet_NaN();
double speed = 0.0;
std::array<double, 16> current_pose_;
//double target_gripper_width = 1.0;
//double target_gripper_speed = 0.1;
//double target_gripper_force = 60;
bool gripper_grasp_stat = false;
bool grasp = false;
bool plan_available = false;

franka::Gripper* gripper = nullptr;  // just for the sake of creating a global object, will be changed later

/**
 * @example generate_cartesian_velocity_motion.cpp
 * An example showing how to generate a Cartesian velocity motion.
 *
 * @warning Before executing this example, make sure there is enough space in front of the robot.
 */

namespace {
    template <class T, size_t N>
    std::ostream& operator<<(std::ostream& ostream, const std::array<T, N>& array) {
        ostream << "[";
        std::copy(array.cbegin(), array.cend() - 1, std::ostream_iterator<T>(ostream, ","));
        std::copy(array.cend() - 1, array.cend(), std::ostream_iterator<T>(ostream));
        ostream << "]";
        return ostream;
    }
}
// parameters to set
double speed_limit = 0.10;  // was tested to work properly


bool speed_below_limit(double target_speed) {
    return target_speed <= speed_limit;
}


bool graspCallback(vector<float> msg)
{
    float target_gripper_width = msg[0];
    float target_gripper_speed = msg[1];
    float target_gripper_force = msg[2];

    bool grasped = gripper->grasp(target_gripper_width, target_gripper_speed, target_gripper_force, 0.1, 0.1);
    return grasped;
}

// subscriber for absolute coordinates and speed
void subscribe_target_motion() {

    ros::NodeHandle node_handle;
    std_msgs::Float64MultiArray states;
    states.data.resize(16);
    ros::Publisher publisher = node_handle.advertise<std_msgs::Float64MultiArray>("franka_current_position", 1);
    ros::Rate loop_rate(2000);  // 2 kHz

    while (ros::ok()) {
        for(int i = 0; i<16; i++){
            states.data[i] = current_pose_[i];
        }
        publisher.publish(states);
        ros::spinOnce();
        loop_rate.sleep();
    }
}


bool generateMotionPlan(thesis_franka::GripperData::Request  &req,
                        thesis_franka::GripperData::Response &res){
    MotionPlanner pathPlanner = MotionPlanner(0.005);
    ROS_INFO_STREAM("star_pos: " << start_x << " " << start_y << " " << start_z);
    ROS_INFO_STREAM("goal pos: " << req.goal.pose.position.x << " " << req.goal.pose.position.y << " " << req.goal.pose.position.z);

    goal_x = req.goal.pose.position.x;
    goal_y = req.goal.pose.position.y;
    goal_z = req.goal.pose.position.z;
    joint_6_val = req.goal.pose.orientation.z;
    vector<vector<double>> path = {{start_x, start_y, start_z}, {goal_x, goal_y,goal_z-0.3}, {goal_x,goal_y,goal_z}};

    auto motion_plan = pathPlanner.applyTrapezoidalVelocity(path);

    ROS_INFO_STREAM("motion plan received ");
    plan_available = true;
    sleep(2);
    for(auto traj: motion_plan){
        target_x = traj[0];
        target_y = traj[1];
        target_z = traj[2];
        speed = traj[3];
        usleep(5000);
    }
    sleep(1);

    ROS_INFO_STREAM("motion completed");
    return true;
}


int main(int argc, char** argv) {
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    if (argc != 2) {
        ROS_ERROR_STREAM("Usage: ./franka_gripper_control <robot-hostname>");
        return -1;
    }
    try {
        franka::Robot robot(argv[1]);
        gripper = new franka::Gripper(argv[1]);
        sleep(2);
        gripper->homing();

        ros::ServiceServer service = n.advertiseService("robot_server", generateMotionPlan);
        std::array<double, 7> q_goal = {{-1.5712350861180224, 0.31163586411978067, 0.0001420356207276586,
                                                -0.6899356769762541,-0.0006083739476036627, 0.9990197826387897, 0.7855223296198005}};
        MotionGenerator motion_generator(0.2, q_goal);
        ROS_WARN_STREAM("WARNING: This example will move the robot! "
                  << "Please make sure to have the user stop button at hand!");
        ROS_INFO_STREAM("Finished moving to initial joint configuration.");
        ROS_INFO_STREAM("Press Enter to continue...");
        std::cin.ignore();
        robot.control(motion_generator);

        // Set the collision behavior.
        std::array<double, 7> lower_torque_thresholds_nominal{
                {25.0, 25.0, 22.0, 20.0, 19.0, 17.0, 14.}};
        std::array<double, 7> upper_torque_thresholds_nominal{
                {35.0, 35.0, 32.0, 30.0, 29.0, 27.0, 24.0}};
        std::array<double, 7> lower_torque_thresholds_acceleration{
                {25.0, 25.0, 22.0, 20.0, 19.0, 17.0, 14.0}};
        std::array<double, 7> upper_torque_thresholds_acceleration{
                {35.0, 35.0, 32.0, 30.0, 29.0, 27.0, 24.0}};
        std::array<double, 6> lower_force_thresholds_nominal{{30.0, 30.0, 30.0, 25.0, 25.0, 25.0}};
        std::array<double, 6> upper_force_thresholds_nominal{{40.0, 40.0, 40.0, 35.0, 35.0, 35.0}};
        std::array<double, 6> lower_force_thresholds_acceleration{{30.0, 30.0, 30.0, 25.0, 25.0, 25.0}};
        std::array<double, 6> upper_force_thresholds_acceleration{{40.0, 40.0, 40.0, 35.0, 35.0, 35.0}};
        robot.setCollisionBehavior(
                lower_torque_thresholds_acceleration, upper_torque_thresholds_acceleration,
                lower_torque_thresholds_nominal, upper_torque_thresholds_nominal,
                lower_force_thresholds_acceleration, upper_force_thresholds_acceleration,
                lower_force_thresholds_nominal, upper_force_thresholds_nominal);
        franka::Model model = robot.loadModel();
        double time = 0.0;

        auto initial_pose = robot.readOnce().O_T_EE_d;
        start_x = initial_pose[12];
        start_y = initial_pose[13];
        start_z = initial_pose[14];
        current_pose_ = initial_pose;

        // call the subscription thread
        std::thread t1(subscribe_target_motion);

        ROS_INFO("Waiting for GOAL");

        while(isnan(joint_6_val) || isnan(goal_x)){
            sleep(1);
        }

        ROS_INFO_STREAM("Aligning gripper with object");
        q_goal = {{-1.5712350861180224, 0.31163586411978067, 0.0001420356207276586,
                          -0.6899356769762541,-0.0006083739476036627, 0.9990197826387897, 0.7855223296198005-joint_6_val}};
        MotionGenerator motion_generator1(0.2, q_goal);
        robot.control(motion_generator1);
        sleep(2);

        ROS_INFO_STREAM("Starting Robot motion");
        auto cartesian_velocity_callback = [=, &time](const franka::RobotState& robot_state,
                                                      franka::Duration time_step) -> franka::CartesianVelocities {
            double vel_x = 0.0;
            double vel_y = 0.0;
            double vel_z = 0.0;

            static double old_vel_x = 0.0;
            static double old_vel_y = 0.0;
            static double old_vel_z = 0.0;

            time += time_step.toSec();

            auto state_pose = robot_state.O_T_EE_d;
            current_pose_ = state_pose;

            double cur_x = current_pose_[12];
            double cur_y = current_pose_[13];
            double cur_z = current_pose_[14];

            // initially, the robot moves to its current position (-> no motion)
            if (isnan(target_x)) {
                target_x = cur_x;
                target_y = cur_y;
                target_z = cur_z;
            }
            double vec_x = target_x - cur_x;
            double vec_y = target_y - cur_y;
            double vec_z = target_z - cur_z;

            double l2_norm = sqrt(vec_x*vec_x + vec_y*vec_y + vec_z*vec_z);

            if (l2_norm < 0.02) {
                vel_x = 0.9*old_vel_x;
                vel_y = 0.9*old_vel_y;
                vel_z = 0.9*old_vel_z;
            }
            else {
                vel_x = speed*(vec_x / l2_norm);
                vel_y = speed*(vec_y / l2_norm);
                vel_z = speed*(vec_z / l2_norm);
            }

            vel_x = 0.99*old_vel_x + 0.01*vel_x;
            vel_y = 0.99*old_vel_y + 0.01*vel_y;
            vel_z = 0.99*old_vel_z + 0.01*vel_z;

            old_vel_x = vel_x;
            old_vel_y = vel_y;
            old_vel_z = vel_z;

            franka::CartesianVelocities output = {{vel_x, vel_y, vel_z, 0.0, 0.0, 0.0}};

            double vel_norm = sqrt(vel_x*vel_x + vel_y*vel_y + vel_z*vel_z);
            return output;
        };
        // Set gains for the joint impedance control.
        // Stiffness
        const std::array<double, 7> k_gains = {{60.0, 60.0, 60.0, 60.0, 60.0, 30.0, 20.0}};
        // Damping
        const std::array<double, 7> d_gains = {{50.0, 50.0, 50.0, 50.0, 30.0, 25.0, 15.0}};

        std::function<franka::Torques(const franka::RobotState&, franka::Duration)>
        impedance_control_callback =
                [&model, k_gains, d_gains](
                        const franka::RobotState& state, franka::Duration /*period*/) -> franka::Torques {
                    // Read current coriolis terms from model.
                    std::array<double, 7> coriolis = model.coriolis(state);

                    // Compute torque command from joint impedance control law.
                    // Note: The answer to our Cartesian pose inverse kinematics is always in state.q_d with one
                    // time step delay.
                    std::array<double, 7> tau_d_calculated;
                    for (size_t i = 0; i < 7; i++) {
                        tau_d_calculated[i] =
                                k_gains[i] * (state.q_d[i] - state.q[i]) - d_gains[i] * state.dq[i] + coriolis[i];
                    }

                    // The following line is only necessary for printing the rate limited torque. As we activated
                    // rate limiting for the control loop (activated by default), the torque would anyway be
                    // adjusted!
                    std::array<double, 7> tau_d_rate_limited =
                            franka::limitRate(franka::kMaxTorqueRate, tau_d_calculated, state.tau_J_d);

                    // Send torque command.
                    return tau_d_rate_limited;
                };
        robot.control(impedance_control_callback, cartesian_velocity_callback);
        if(t1.joinable()){
            t1.join();
        }
    } catch (const franka::Exception& e) {
        ROS_ERROR_STREAM(e.what());
        return -1;
    }
    return 0;
}
