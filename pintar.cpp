#include <ros/ros.h>
#include <std_srvs/Empty.h>
#include <laboratorio_ros/typedef.h>


int main(int argc, char** argv){
  
  ros::init(argc,argv,"pintar");
  ros::NodeHandle nh;
  
  ros::service::waitForService("clear");
  
  ros::param::set("background_r", 0);
  ros::param::set("background_g", 122);
  ros::param::set("background_b", 0);
  ros::ServiceClient clearClient = nh.serviceClient<std_srvs::Empty>("/clear");
  std_srvs::Empty srv;
  clearClient.call(srv);
}