#include "rqt_ardrone/commands/commands.h"
#include <pluginlib/class_list_macros.h>
#include <QStringList>

namespace rqt_ardrone {

Commands::Commands()
: rqt_gui_cpp::Plugin()
, widget_(0)
{
	setObjectName("ArdroneCommands");
}

void Commands::initPlugin(qt_gui_cpp::PluginContext& context)
{
	QStringList argv = context.argv();

	widget_ = new QWidget();
	ui_.setupUi(widget_);
	context.addWidget(widget_);

	/* Setup sliders */
	ui_.xVelSlider->setRange(-1.0, 1.0, 0.1);
	ui_.yVelSlider->setRange(1.0, -1.0, 0.1);
	ui_.zVelSlider->setRange(-1.0, 1.0, 0.1);
	ui_.rotationalSpeedSlider->setRange(1.0, -1.0, 0.1);

	/* Setup ROS interaction */
	setupROSComponents();
	connectROSComponents();

	/* Start periodic updates */
	connect(&cmdTimer_, SIGNAL(timeout()), this, SLOT(sendCmdVel()));
	cmdTimer_.setInterval(35);
}

void Commands::shutdownPlugin()
{
	/* Stop periodic updates */
	cmdTimer_.stop();
	disconnect(&cmdTimer_, SIGNAL(timeout()), this, SLOT(sendCmdVel()));

	/* Disconnect ROS */
	disconnectROSComponents();
	shutdownROSComponents();
}

void Commands::saveSettings(qt_gui_cpp::Settings& plugin_settings,
                            qt_gui_cpp::Settings& instance_settings) const
{
	// TODO save intrinsic configuration, usually using:
	// instance_settings.setValue(k, v)
}

void Commands::restoreSettings(const qt_gui_cpp::Settings& plugin_settings, 
                               const qt_gui_cpp::Settings& instance_settings)
{
	// TODO restore intrinsic configuration, usually using:
	// v = instance_settings.value(k)
}

void Commands::setupROSComponents()
{
	/* Setup publishers */
	pub_takeoff_ = getNodeHandle().advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
	pub_land_    = getNodeHandle().advertise<std_msgs::Empty>("/ardrone/land", 1);
	pub_reset_   = getNodeHandle().advertise<std_msgs::Empty>("/ardrone/reset", 1);
	pub_trim_    = getNodeHandle().advertise<std_msgs::Empty>("/ardrone/trim", 1);
	pub_cmd_vel_ = getNodeHandle().advertise<geometry_msgs::Twist>("/cmd_vel", 1);

	/* Setup services */
	srv_switchCamera_ = getNodeHandle()
		.serviceClient<std_srvs::Empty>("/ardrone/togglecam");
}

void Commands::shutdownROSComponents()
{
	/* Shutdown publishers */
	pub_takeoff_.shutdown();
	pub_land_.shutdown();
	pub_reset_.shutdown();
	pub_trim_.shutdown();
	pub_cmd_vel_.shutdown();

	/* Shutdown services */
	srv_switchCamera_.shutdown();
}

void Commands::connectROSComponents()
{
	/* Connect buttons to the respective action */
	connect(ui_.takeoffButton, SIGNAL(clicked()), this, SLOT(takeoff()));
	connect(ui_.landButton, SIGNAL(clicked()), this, SLOT(land()));
	connect(ui_.resetButton, SIGNAL(clicked()), this, SLOT(reset()));
	connect(ui_.trimButton, SIGNAL(clicked()), this, SLOT(trim()));
	connect(ui_.switchCameraButton, SIGNAL(clicked()), this, SLOT(switchCamera()));

	connect(ui_.goForwardButton, SIGNAL(pressed()),
	                       this, SLOT(onGoForwardButton()));
	connect(ui_.goForwardButton, SIGNAL(released()),
	                       this, SLOT(onGoForwardButton()));

	connect(ui_.goBackwardsButton, SIGNAL(pressed()),
	                         this, SLOT(onGoBackwardsButton()));
	connect(ui_.goBackwardsButton, SIGNAL(released()),
	                         this, SLOT(onGoBackwardsButton()));

	connect(ui_.goLeftButton, SIGNAL(pressed()),
	                    this, SLOT(onGoLeftButton()));
	connect(ui_.goLeftButton, SIGNAL(released()),
	                    this, SLOT(onGoLeftButton()));

	connect(ui_.goRightButton, SIGNAL(pressed()),
	                     this, SLOT(onGoRightButton()));
	connect(ui_.goRightButton, SIGNAL(released()),
	                     this, SLOT(onGoRightButton()));

	connect(ui_.transmitButton, SIGNAL(toggled(bool)),
	                      this, SLOT(toggleTransmission(bool)));

	connect(ui_.stopButton, SIGNAL(clicked()),
	                  this, SLOT(stop()));


	/* Setup sliders' signals */
	connect(ui_.rotationalSpeedSlider, SIGNAL(valueChanged(double)), 
	                             this, SLOT(setRotVel(double)));
	connect(ui_.rotationalSpeedSlider, SIGNAL(sliderReleased()), 
	                             this, SLOT(onRotVelSliderReleased()));

	connect(ui_.xVelSlider, SIGNAL(valueChanged(double)), 
	                  this, SLOT(setXVel(double)));
	connect(ui_.xVelSlider, SIGNAL(sliderReleased()), 
	                  this, SLOT(onXVelSliderReleased()));

	connect(ui_.yVelSlider, SIGNAL(valueChanged(double)), 
	                  this, SLOT(setYVel(double)));
	connect(ui_.yVelSlider, SIGNAL(sliderReleased()), 
	                  this, SLOT(onYVelSliderReleased()));

	connect(ui_.zVelSlider, SIGNAL(valueChanged(double)), 
	                  this, SLOT(setZVel(double)));
	connect(ui_.zVelSlider, SIGNAL(sliderReleased()), 
	                  this, SLOT(onZVelSliderReleased()));
}

void Commands::disconnectROSComponents()
{
	/* Disconnect all buttons */
	disconnect(ui_.takeoffButton, SIGNAL(clicked()), this, SLOT(takeoff()));
	disconnect(ui_.landButton, SIGNAL(clicked()), this, SLOT(land()));
	disconnect(ui_.resetButton, SIGNAL(clicked()), this, SLOT(reset()));
	disconnect(ui_.trimButton, SIGNAL(clicked()), this, SLOT(trim()));
	disconnect(ui_.stopButton, SIGNAL(clicked()), this, SLOT(stop()));
	disconnect(ui_.switchCameraButton, SIGNAL(clicked()), this, SLOT(switchCamera()));

	disconnect(ui_.goForwardButton, SIGNAL(pressed()),
	                          this, SLOT(onGoForwardButton()));
	disconnect(ui_.goForwardButton, SIGNAL(released()),
	                          this, SLOT(onGoForwardButton()));

	disconnect(ui_.goBackwardsButton, SIGNAL(pressed()),
	                            this, SLOT(onGoBackwardsButton()));
	disconnect(ui_.goBackwardsButton, SIGNAL(released()),
	                            this, SLOT(onGoBackwardsButton()));

	disconnect(ui_.goLeftButton, SIGNAL(pressed()),
	                       this, SLOT(onGoLeftButton()));
	disconnect(ui_.goLeftButton, SIGNAL(released()),
	                       this, SLOT(onGoLeftButton()));

	disconnect(ui_.goRightButton, SIGNAL(pressed()),
	                        this, SLOT(onGoRightButton()));
	disconnect(ui_.goRightButton, SIGNAL(released()),
	                        this, SLOT(onGoRightButton()));

	disconnect(ui_.transmitButton, SIGNAL(toggled(bool)),
	                         this, SLOT(toggleTransmission(bool)));


	/* Disconnect sliders' signals */
	disconnect(ui_.rotationalSpeedSlider, SIGNAL(valueChanged(double)), 
	                                this, SLOT(setRotVel(double)));
	disconnect(ui_.rotationalSpeedSlider, SIGNAL(sliderReleased()), 
	                                this, SLOT(onRotVelSliderReleased()));

	disconnect(ui_.xVelSlider, SIGNAL(valueChanged(double)), 
	                     this, SLOT(setXVel(double)));
	disconnect(ui_.xVelSlider, SIGNAL(sliderReleased()), 
	                     this, SLOT(onXVelSliderReleased()));

	disconnect(ui_.yVelSlider, SIGNAL(valueChanged(double)), 
	                     this, SLOT(setYVel(double)));
	disconnect(ui_.yVelSlider, SIGNAL(sliderReleased()), 
	                     this, SLOT(onYVelSliderReleased()));

	disconnect(ui_.zVelSlider, SIGNAL(valueChanged(double)), 
	                     this, SLOT(setZVel(double)));
	disconnect(ui_.zVelSlider, SIGNAL(sliderReleased()), 
	                     this, SLOT(onZVelSliderReleased()));
}


} // namespace

PLUGINLIB_DECLARE_CLASS(rqt_ardrone, Commands, rqt_ardrone::Commands, rqt_gui_cpp::Plugin)
