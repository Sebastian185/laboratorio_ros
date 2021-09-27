#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <laboratorio_ros/typedef.h>

int main(int argc, char** argv){
  double vel, dis;
  
  ros::init(argc,argv,"mover");
  ros::NodeHandle nh;
  
  ros::Publisher pub_vel = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
  
  const std::string PARAM_VELOCITY = "~vel";
  const std::string PARAM_DISTANCE = "~dis";
    
  ros::param::get(PARAM_VELOCITY, vel);
  ros::param::get(PARAM_DISTANCE, dis);
  
  ros::Rate r(10);
  
  geometry_msgs::Twist msg;
  msg.linear.x = vel;
  msg.angular.z = 0;
  double t0 = ros::Time::now().toSec();
  double current_dis = 0;
  while(current_dis < dis){
    pub_vel.publish(msg);
    
    double t1 = ros::Time::now().toSec();
    current_dis = vel * (t1 - t0);
    ROS_INFO("current_distance: %.21f total: %.21f", current_dis, dis);
  }
  ROS_INFO("current_distance: %.21f total: %.21f", current_dis, dis);
  msg.linear.x = 0;
  pub_vel.publish(msg);
  ROS_INFO("La tortuga se detuvo...");
  
  while(ros::ok()){
    ros::spin();
    r.sleep();
  }
  return 0;
}
