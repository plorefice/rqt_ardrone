#ifndef rqt_ardrone__navdata__status_panel_H
#define rqt_ardrone__navdata__status_panel_H

#include <QGraphicsItem>

namespace rqt_ardrone {

class StatusPanel : public QGraphicsItem
{
public:

	enum DroneStatus
	{
		EMERGENCY = 0,
		INITED,
		LANDED,
		FLYING,
		HOVERING,
		TEST,
		TAKINGOFF,
		GOTOHOVER,
		LANDING,
		LOOPING
	};

	StatusPanel();

	QRectF boundingRect() const;

	void paint(QPainter *painter, 
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

	void setStatus(DroneStatus s) { currentStatus_ = s; }

private:

	DroneStatus currentStatus_;

};


} // namespaces

#endif // rqt_ardrone__navdata__status_panel_H
