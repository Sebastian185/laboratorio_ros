#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>
#include <std_srvs/Empty.h>
#include <laboratorio_ros/typedef.h>

int main(int argc, char** argv){
  double vel;  
  
  ros::init(argc,argv,"square");
  ros::NodeHandle nh;  
  ros::service::waitForService("spawn");
  
  ros::ServiceClient killClient = nh.serviceClient<turtlesim::Kill>("kill");
  turtlesim::Kill::Request reqk;
  turtlesim::Kill::Response respk;
  reqk.name = "turtle1";
  
  bool successk = killClient.call(reqk,respk);
  if(successk)
    ROS_INFO_STREAM("Se mato a la tortuga: " << reqk.name);
  else
    ROS_INFO_STREAM("fallo al matar tortuga");
  
  ros::ServiceClient spawnClient = nh.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn::Request req;
  turtlesim::Spawn::Response resp;
  
  req.x = 0.0;
  req.y = 0.0;
  req.theta = 0.0;
  req.name = "turtle1";
  bool success = spawnClient.call(req,resp);
  if(success)
    ROS_INFO_STREAM("Se Creo una nueva tortuga: " << resp.name);
  else
    ROS_INFO_STREAM("fallo al crear tortuga");
  
  ros::Publisher pub_vel = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
  
  const std::string PARAM_VELOCITY = "~vel";
    
  ros::param::get(PARAM_VELOCITY,vel);
  
  ros::Rate r(10);
  
  geometry_msgs::Twist msg;
  double t0 = ros::Time::now().toSec();
  double current_dis = 0, dis = 3;
  for(int i=0;i<8;i++){
    if(i%2==0){
      msg.linear.x = vel;
      msg.angular.z = 0;
      dis = 3;
    }else if(i%2==1){
      msg.linear.x = 0;
      msg.angular.z = vel;
      dis = M_PI/2;
    }
    while(current_dis < dis){
      pub_vel.publish(msg);
      double t1 = ros::Time::now().toSec();
      current_dis = vel * (t1 - t0);
      ROS_INFO("current_distance: %.21f total: %.21f", current_dis, dis);
    }
    t0 = ros::Time::now().toSec();
    current_dis = 0;
  }
  msg.linear.x = 0;
  msg.angular.z = 0;
  pub_vel.publish(msg);
  ROS_INFO("La tortuga se detuvo...");
  
  successk = killClient.call(reqk,respk);
  if(successk)
    ROS_INFO_STREAM("Se mato a la tortuga: " << reqk.name);
  else
    ROS_INFO_STREAM("fallo al matar tortuga");
  while(ros::ok()){
    ros::spinOnce();
    r.sleep();
  }
  return 0;
}