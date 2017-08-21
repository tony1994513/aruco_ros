#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_listener.h>
#include <cmath>
using namespace std;

#include <Eigen/Core>
// Eigen 几何模块
#include <Eigen/Geometry>
#include <Eigen/Dense>

void poseCallback(const geometry_msgs::PoseStamped &msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  
  Eigen::Matrix3d rotation_matrix = Eigen::Matrix3d::Identity();
  Eigen::AngleAxisd rotation_vector1 ( -M_PI/2, Eigen::Vector3d ( 1,0,0 ) ); 
  //Eigen::AngleAxisd rotation_vector2 ( -M_PI, Eigen::Vector3d ( 1,0,0 ) ); 
  Eigen::Matrix3d rotation_vector;
  //rotation_vector = rotation_vector2 * rotation_vector1;
  Eigen::Vector4d tt;
  tt = Eigen::Quaterniond(rotation_vector1).coeffs();
  tt.normalize();
  
  //geometry_msgs::Pose temp;
  //transform.setOrigin( tf::Vector3(msg.pose.position.x, msg.pose.position.y, msg.pose.position.z) );
  transform.setOrigin( tf::Vector3(msg.pose.position.x, msg.pose.position.y, msg.pose.position.z) );

  
  //transform.setRotation( tf::Quaternion(msg.pose.orientation.x, msg.pose.orientation.y, msg.pose.orientation.z, msg.pose.orientation.w) ); 
  
  transform.setRotation( tf::Quaternion(tt[0], tt[1], tt[2], tt[3]) );
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "camera_rgb_optical_frame", "new_co"));
  
  

}
  
int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe( "aruco_tracker/pose", 10, &poseCallback);
  
/*  tf::TransformListener listener;
  ros::Rate rate(10.0); 
  while (node.ok()){
    tf::StampedTransform transform_listener;
    
    try {
    listener.waitForTransform("new_co", "camera_link", ros::Time(0), ros::Duration(10.0) );
    listener.lookupTransform("new_co", "camera_link", ros::Time(0), transform_listener);
} catch (tf::TransformException ex) {
    ROS_ERROR("%s",ex.what());
} */
    //cout<<"transform_listener.getOrigin() = "<< transform_listener.getOrigin() <<endl;
  ros::spin();
  return 0;
  
};

