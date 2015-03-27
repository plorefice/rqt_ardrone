#include "rqt_ardrone/navdata/navdata.h"
#include <pluginlib/class_list_macros.h>
#include <QStringList>

#include <qwt_compass.h>
#include <qwt_compass_rose.h>
#include <qwt_dial_needle.h>

namespace rqt_ardrone {

Navdata::Navdata()
: rqt_gui_cpp::Plugin()
, widget_(0)
{
	setObjectName("ArdroneNavdata");
}

void Navdata::initPlugin(qt_gui_cpp::PluginContext& context)
{
	QStringList argv = context.argv();

	widget_ = new QWidget();
	ui_.setupUi(widget_);
	context.addWidget(widget_);

	/* Setup plot data */
	altdSamples_ = new NavdataStampedSamples(0, 3000.0);
	xVelSamples_ = new NavdataStampedSamples(-1000.0, 1000.0);
	yVelSamples_ = new NavdataStampedSamples(-1000.0, 1000.0);
	zVelSamples_ = new NavdataStampedSamples(-1000.0, 1000.0);

	altdCurve_.setData(altdSamples_);
	xVelCurve_.setData(xVelSamples_);
	yVelCurve_.setData(yVelSamples_);
	zVelCurve_.setData(zVelSamples_);

	/* Setup plot grids */
	altdGrid_.setPen(QPen(Qt::darkGray, 0.25, Qt::DashLine));
	xVelGrid_.setPen(QPen(Qt::darkGray, 0.25, Qt::DashLine));
	yVelGrid_.setPen(QPen(Qt::darkGray, 0.25, Qt::DashLine));
	zVelGrid_.setPen(QPen(Qt::darkGray, 0.25, Qt::DashLine));

	/* Setup plots */
	ui_.altdPlot->setAxisTitle(QwtPlot::yLeft, "Altitude (mm)");
	altdCurve_.attach(ui_.altdPlot);
	altdGrid_.attach(ui_.altdPlot);

	ui_.xVelPlot->setAxisTitle(QwtPlot::yLeft, "X Velocity (mm/s)");
	xVelCurve_.attach(ui_.xVelPlot);
	xVelGrid_.attach(ui_.xVelPlot);

	ui_.yVelPlot->setAxisTitle(QwtPlot::yLeft, "Y Velocity (mm/s)");
	yVelCurve_.attach(ui_.yVelPlot);
	yVelGrid_.attach(ui_.yVelPlot);

	ui_.zVelPlot->setAxisTitle(QwtPlot::yLeft, "Z Velocity (mm/s)");
	zVelCurve_.attach(ui_.zVelPlot);
	zVelGrid_.attach(ui_.zVelPlot);

	/* Setup replot timer */
	connect(&replotTimer_, SIGNAL(timeout()), this, SLOT(replot()));

	/* Setup compass */
	ui_.yawCompass->setScaleComponents(QwtAbstractScaleDraw::Backbone | 
		QwtAbstractScaleDraw::Ticks | QwtAbstractScaleDraw::Labels);
	ui_.yawCompass->setScaleTicks(0, 0, 3);

	QMap<double, QString> map;
	for (double d = 0.0; d < 360.0; d += 60.0)
	{
		QString label;
		label.sprintf("%.0f", d);
		map.insert(d, label);
	}
	ui_.yawCompass->setLabelMap(map);
	ui_.yawCompass->setScale(36, 5, 0);
	
	ui_.yawCompass->setNeedle(
		new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Ray, true, Qt::gray));

	/* Setup navball */
	navballScene_.addItem(&navball_);

	ui_.navballView->setStyleSheet("background-color: transparent");
	ui_.navballView->setScene(&navballScene_);

	/* Subscribe to the required topics */
	updateSubscriptions();

	/* Update the GUI upon receiving a new navdata packet */
	connect(this, SIGNAL(navdataChanged()), this, SLOT(update()));

	/* Start timer */
	replotTimer_.start(50);
}

void Navdata::shutdownPlugin()
{
	/* Disconnect all signals */
	disconnect(this, SIGNAL(navdataChanged()), this, SLOT(update()));
	disconnect(&replotTimer_, SIGNAL(timeout()), this, SLOT(replot()));

	/* Detach plot items */
	altdCurve_.detach();
	xVelCurve_.detach();
	yVelCurve_.detach();
	zVelCurve_.detach();

	altdGrid_.detach();
	xVelGrid_.detach();
	yVelGrid_.detach();
	zVelGrid_.detach();

	/* Stop timer */
	replotTimer_.stop();
}

void Navdata::saveSettings(qt_gui_cpp::Settings& plugin_settings, qt_gui_cpp::Settings& instance_settings) const
{
	// TODO save intrinsic configuration, usually using:
	// instance_settings.setValue(k, v)
}

void Navdata::restoreSettings(const qt_gui_cpp::Settings& plugin_settings, const qt_gui_cpp::Settings& instance_settings)
{
	// TODO restore intrinsic configuration, usually using:
	// v = instance_settings.value(k)
}

void Navdata::updateSubscriptions()
{
	/* Subscribe to navdata topic */
	sub_navdata_ = getNodeHandle().subscribe<ardrone_autonomy::Navdata>(
		"/ardrone/navdata", 10, &Navdata::receiveNavdata, this);
}

void Navdata::receiveNavdata(ardrone_autonomy::Navdata::ConstPtr const& navdata)
{
	/* Store navdata and emit signal */
	navdata_ = *navdata;

	/* Update */
	emit navdataChanged();
}

void Navdata::update()
{
	/* Plots */
	altdSamples_->append(QPointF(navdata_.tm / 1e6, navdata_.altd));
	xVelSamples_->append(QPointF(navdata_.tm / 1e6, navdata_.vx));
	yVelSamples_->append(QPointF(navdata_.tm / 1e6, navdata_.vy));
	zVelSamples_->append(QPointF(navdata_.tm / 1e6, navdata_.vz));

	/* Environment measurements */
	ui_.batteryBar->setValue(navdata_.batteryPercent);
	ui_.pressureBar->setValue(navdata_.pressure / 1000.0);
	ui_.temperatureBar->setValue(navdata_.temp);

	/* Roll, pitch, yaw */
	navball_.setRoll(navdata_.rotX);
	navball_.setPitch(navdata_.rotY);
	navballScene_.update();

	ui_.yawCompass->setValue(-navdata_.rotZ);
}

void Navdata::replot()
{
	/* Replot everything */
	replot(ui_.altdPlot, altdSamples_);
	replot(ui_.xVelPlot, xVelSamples_);
	replot(ui_.yVelPlot, yVelSamples_);
	replot(ui_.zVelPlot, zVelSamples_);
}

void Navdata::replot(QwtPlot * p, NavdataStampedSamples * s)
{
	/* Adjust axis scale and replot */

	p->setAxisScale(QwtPlot::xBottom,
					s->xMin(),
					s->xMax());
	p->setAxisScale(QwtPlot::yLeft,
					s->yMin(),
					s->yMax());
	p->replot();
}



} // namespace

PLUGINLIB_DECLARE_CLASS(rqt_ardrone, Navdata, rqt_ardrone::Navdata, rqt_gui_cpp::Plugin)
