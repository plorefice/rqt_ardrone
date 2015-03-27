#ifndef rqt_ardrone__navdata_H
#define rqt_ardrone__navdata_H

#include "rqt_ardrone/navdata/navball.h"

#include "ros/ros.h"
#include "ardrone_autonomy/Navdata.h"
#include <rqt_gui_cpp/plugin.h>

#include "ui_navdata.h"
#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <cmath>

#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

namespace Ui {
	class NavdataWidget;
}

namespace rqt_ardrone 
{
	class NavdataStampedSamples : public QwtSeriesData<QPointF>
	{
		QList<QPointF> samples_;

		float xMin_, xMax_;
		float yMin_, yMax_;

		static const int MAX_NUM_SAMPLES = 500;

	public:
		NavdataStampedSamples(float ymin = -1.0, float ymax = 1.0)
		{
			xMin_ = yMin_ = 0.0;
			yMin_ = ymin;
			yMax_ = ymax;
		}

		virtual size_t size() const { return samples_.count(); }

		virtual QPointF sample(size_t i) const { return samples_[i]; }

		virtual QRectF boundingRect() const
		{
			return QRectF(QPointF(xMin_, yMin_),
			 			  QPointF(xMax_, yMax_));
		}

		float xMin() const { return xMin_; }
		float xMax() const { return xMax_; }
		float yMin() const { return yMin_; }
		float yMax() const { return yMax_; }

		void append(QPointF const& p) 
		{ 
			samples_.append(p);

			if (samples_.count() > MAX_NUM_SAMPLES)
				samples_.removeFirst();

			xMin_ = samples_.at(0).x();
			xMax_ = samples_.at(samples_.count() - 1).x();

			yMin_ = std::min((float) p.y(), yMin_);
			yMax_ = std::max((float) p.y(), yMax_);
		}
	};

	class Navdata : public rqt_gui_cpp::Plugin
	{

		Q_OBJECT

	public:

		Navdata();

		virtual void initPlugin(qt_gui_cpp::PluginContext& context);

		virtual void shutdownPlugin();

		virtual void saveSettings(qt_gui_cpp::Settings& plugin_settings, qt_gui_cpp::Settings& instance_settings) const;

		virtual void restoreSettings(const qt_gui_cpp::Settings& plugin_settings, const qt_gui_cpp::Settings& instance_settings);

	signals:

		void navdataChanged();

	public slots:

		void update();

		void replot();

	private:

		Ui::NavdataWidget ui_;

		QWidget* widget_;

		ros::Subscriber sub_navdata_;
		ardrone_autonomy::Navdata navdata_;

		/* Data points */
		NavdataStampedSamples * altdSamples_;
		NavdataStampedSamples * xVelSamples_;
		NavdataStampedSamples * yVelSamples_;
		NavdataStampedSamples * zVelSamples_;

		/* Qwt curves */
		QwtPlotCurve altdCurve_;
		QwtPlotCurve xVelCurve_;
		QwtPlotCurve yVelCurve_;
		QwtPlotCurve zVelCurve_;

		/* Plot grids */
		QwtPlotGrid altdGrid_;
		QwtPlotGrid xVelGrid_;
		QwtPlotGrid yVelGrid_;
		QwtPlotGrid zVelGrid_;

		/* Graphics scenes */
		QGraphicsScene navballScene_;
		Navball navball_;

		/* Update timer */
		QTimer replotTimer_;

		void updateSubscriptions();

		void receiveNavdata(ardrone_autonomy::Navdata::ConstPtr const& navdata);

		void replot(QwtPlot * p, NavdataStampedSamples * s);

	};

} // namespace

#endif // rqt_ardrone_navdata__navdata_H
