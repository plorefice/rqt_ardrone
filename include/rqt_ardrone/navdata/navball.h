#ifndef rqt_ardrone__navdata__navball_H
#define rqt_ardrone__navdata__navball_H

#include <QGraphicsItem>

namespace rqt_ardrone {

class Navball : public QGraphicsItem
{
	float roll_;
	float pitch_;

public:
	Navball();

	QRectF boundingRect() const;

	void paint(QPainter *painter, 
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

	void setRoll(float angle) { roll_ = angle; }

	void setPitch(float angle) { pitch_ = angle; }
};


} // namespaces

#endif // rqt_ardrone__navdata__navball_H
