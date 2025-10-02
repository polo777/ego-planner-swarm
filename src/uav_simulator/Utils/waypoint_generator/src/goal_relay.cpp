#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

class GoalRelay : public rclcpp::Node
{
public:
  GoalRelay()
  : Node("goal_relay")
  {
    // Change "/goal_pose" to the topic RViz2 uses, if different
    goal_sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
      "/goal_pose", 10,
      std::bind(&GoalRelay::goal_callback, this, std::placeholders::_1));

    // Change "/move_base_simple/goal" to your target topic if needed
    goal_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>(
      "/move_base_simple/goal", 10);

    RCLCPP_INFO(this->get_logger(), "GoalRelay node started.");
  }

private:
  void goal_callback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
  {
    RCLCPP_INFO(this->get_logger(), "Relaying goal to /move_base_simple/goal: (%.2f, %.2f, %.2f)",
      msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
    goal_pub_->publish(*msg);
  }

  rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr goal_sub_;
  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr goal_pub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GoalRelay>());
  rclcpp::shutdown();
  return 0;
}