#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>
#include <turtlesim/Pose.h>
#include <std_srvs/Empty.h>
#include <laboratorio_ros/typedef.h>

turtlesim::Pose pose;
geometry_msgs::Twist msg;

void poseCallback(const turtlesim::Pose::ConstPtr& msg){
    pose.x = msg->x;
    pose.y = msg->y;
    pose.theta = msg->theta;
}

double norm(turtlesim::Pose pos, turtlesim::Pose goal){
  return sqrt(pow(pos.x - goal.x,2) + pow(pos.y - goal.y,2));
}

bool move_to_goal(turtlesim::Pose goal, double dis_tol){
  double K_v = 1.5, K_t = 3;
  msg.linear.x = K_v*norm(pose,goal);   
  msg.angular.z = K_t*(atan2(goal.y-pose.y,goal.x-pose.x) - pose.theta);
  if(norm(pose,goal)<dis_tol){
    msg.linear.x = 0;
    msg.angular.z = 0;
    ROS_INFO("La tortuga se detuvo...");
    return 0;
  }
  return 1;
}

int main(int argc, char** argv){
  double x, y;
  
  ros::init(argc,argv,"move_to_goal");
  ros::NodeHandle nh;
  ros::Publisher pub_vel = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
  ros::Subscriber sub_pose = nh.subscribe<turtlesim::Pose>("turtle1/pose", 1000, &poseCallback);
  const std::string PARAM_X = "~x";
  const std::string PARAM_Y = "~y";
  
  ros::param::get(PARAM_X,x);
  ros::param::get(PARAM_Y,y);
  
  ros::Rate r(10);
  
  turtlesim::Pose goal;
  goal.x = x;
  goal.y = y;
  
  bool n;
  while(n){
    n = move_to_goal(goal,0.01);
    pub_vel.publish(msg);
    ROS_INFO("x: %.21f y: %.21f", pose.x, pose.y);
    ros::spinOnce();
    r.sleep();
  }  
  return 0;
}
