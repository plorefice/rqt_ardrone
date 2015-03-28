#ifndef rqt_ardrone__commands__commands_H
#define rqt_ardrone__commands__commands_H

#include "ros/ros.h"
#include "std_msgs/Empty.h"
#include "std_srvs/Empty.h"
#include "geometry_msgs/Twist.h"
#include <rqt_gui_cpp/plugin.h>

#include "ui_commands.h"
#include <QWidget>
#include <QTimer>

namespace Ui {
	class CommandsWidget;
}

namespace rqt_ardrone {
	
class Commands : public rqt_gui_cpp::Plugin
{

	Q_OBJECT

public:

	Commands();

	virtual void initPlugin(qt_gui_cpp::PluginContext& context);

	virtual void shutdownPlugin();

	virtual void saveSettings(qt_gui_cpp::Settings& plugin_settings, 
	                          qt_gui_cpp::Settings& instance_settings) const;

	virtual void restoreSettings(const qt_gui_cpp::Settings& plugin_settings, 
	                             const qt_gui_cpp::Settings& instance_settings);

public slots:
	
	void toggleTransmission(bool t) { (t) ? cmdTimer_.start() : cmdTimer_.stop(); }

	void stop() {
		onRotVelSliderReleased();
		onXVelSliderReleased();
		onYVelSliderReleased();
		onZVelSliderReleased();
	}

	void takeoff()    { pub_takeoff_.publish(empty_msg_); }
	void land()       { pub_land_.publish(empty_msg_); }
	void reset()      { pub_reset_.publish(empty_msg_); }
	void trim()       { pub_trim_.publish(empty_msg_); }
	void sendCmdVel() { pub_cmd_vel_.publish(cmd_vel_); }

	void switchCamera() { srv_switchCamera_.call(empty_srv_); }

	void setXVel(double v)   { cmd_vel_.linear.x = v; }
	void setYVel(double v)   { cmd_vel_.linear.y = v; }
	void setZVel(double v)   { cmd_vel_.linear.z = v; }
	void setRotVel(double v) { cmd_vel_.angular.z = v; }

	void onGoForwardButton() {
		(ui_.goForwardButton->isDown()) ?
		ui_.xVelSlider->setValue(1.0) : 
		ui_.xVelSlider->setValue(0.0); 
	}

	void onGoBackwardsButton() {
		(ui_.goBackwardsButton->isDown()) ?
		ui_.xVelSlider->setValue(-1.0) : 
		ui_.xVelSlider->setValue(0.0); 
	}

	void onGoLeftButton() {
		(ui_.goLeftButton->isDown()) ?
		ui_.yVelSlider->setValue(1.0) : 
		ui_.yVelSlider->setValue(0.0); 
	}

	void onGoRightButton() {
		(ui_.goRightButton->isDown()) ?
		ui_.yVelSlider->setValue(-1.0) : 
		ui_.yVelSlider->setValue(0.0); 
	}

	void onRotVelSliderReleased() { ui_.rotationalSpeedSlider->setValue(0.0); }
	void onXVelSliderReleased()   { ui_.xVelSlider->setValue(0.0); }
	void onYVelSliderReleased()   { ui_.yVelSlider->setValue(0.0); }
	void onZVelSliderReleased()   { ui_.zVelSlider->setValue(0.0); }

private:

	Ui::CommandsWidget ui_;

	QWidget* widget_;

	/* Publishers */
	ros::Publisher  pub_takeoff_;
	ros::Publisher  pub_land_;
	ros::Publisher  pub_reset_;
	ros::Publisher  pub_trim_;
	ros::Publisher  pub_cmd_vel_;
	std_msgs::Empty empty_msg_;

	/* Velocity command */
	geometry_msgs::Twist cmd_vel_;

	/* Service clients */
	ros::ServiceClient srv_switchCamera_;
	std_srvs::Empty    empty_srv_;

	/* Timer to send periodic cmd_vel messages */
	QTimer cmdTimer_;

	void setupROSComponents();
	void shutdownROSComponents();

	void connectROSComponents();
	void disconnectROSComponents();

};

} // namespace

#endif // rqt_ardrone__commands__commands_H
