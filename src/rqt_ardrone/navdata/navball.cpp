#include "rqt_ardrone/navdata/navball.h"

#include <QPainter>
#include <QPainterPath>

#include <cmath>

namespace rqt_ardrone {

#define DEG2RAD (M_PI / 180.0)

Navball::Navball()
: roll_(0.0)
, pitch_(0.0)
{
}

QRectF Navball::boundingRect() const
{
	return QRectF(-85.0, -85.0, 170.0, 170.0);
}

void Navball::paint(QPainter *painter, 
					const QStyleOptionGraphicsItem *option,
					QWidget *widget)
{
	QPainterPath path;
	QPointF center(0.0, 0.0);

	/* Save painter */
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);

	/* Move ball according to pitch */
	painter->translate(0.0, 70.0 * sin(-pitch_ * DEG2RAD));

	/* Draw top half of the ball */
	path.moveTo(center);
	path.arcTo(QRectF(-70.0, -70.0, 140.0, 140.0), 0.0 + roll_, 180.0);
	path.lineTo(center);

	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter->setBrush(QColor(30, 144, 255));
	painter->drawPath(path);

	/* Draw bottom half of the ball */
	path = QPainterPath();
	path.moveTo(center);
	path.arcTo(QRectF(-70.0, -70.0, 140.0, 140.0), 0.0 + roll_, -180.0);
	path.lineTo(center);

	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter->setBrush(QColor(160,82,45));
	painter->drawPath(path);

	/* Draw white separation line */
	float x = 70.0 * cos(-roll_ * DEG2RAD);
	float y = 70.0 * sin(-roll_ * DEG2RAD);
	painter->setPen(QPen(Qt::white, 3, Qt::SolidLine));
	painter->drawLine(x, y, -x, -y);

	/* Everything else must be static */
	painter->translate(0.0, 70.0 * sin(pitch_ * DEG2RAD));

	/* Draw top outer ring */
	path = QPainterPath();
	path.moveTo(60.0, 0.0);
	path.lineTo(80.0, 0.0);
	path.arcTo(QRectF(-80.0, -80.0, 160.0, 160.0), 0.0, 180.0);
	path.lineTo(-60.0, 0.0);
	path.arcTo(QRectF(-60.0, -60.0, 120.0, 120.0), 180, -180.0);

	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter->setBrush(QColor(30, 144, 255));
	painter->drawPath(path);

	/* Draw bottom outer ring */
	path = QPainterPath();
	path.moveTo(60.0, 0.0);
	path.lineTo(80.0, 0.0);
	path.arcTo(QRectF(-80.0, -80.0, 160.0, 160.0), 0.0, -180.0);
	path.lineTo(-60.0, 0.0);
	path.arcTo(QRectF(-60.0, -60.0, 120.0, 120.0), 180, 180.0);

	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter->setBrush(QColor(160,82,45));
	painter->drawPath(path);

	/* Draw white separation line */
	painter->setPen(QPen(Qt::white, 3, Qt::SolidLine));
	painter->drawLine(-80.0, 0.0, -60.0, 0.0);
	painter->drawLine(60.0, 0.0, 80.0, 0.0);

	/* Draw drone */
	path = QPainterPath();
	path.moveTo(-40.0, 0.0);
	path.lineTo(-14.0, 0.0);
	path.moveTo(+14.0, 0.0);
	path.arcTo(QRectF(-14.0, -14.0, 28.0, 28.0), 0, -180.0);
	path.moveTo(+14.0, 0.0);
	path.lineTo(+40.0, 0.0);

	painter->setPen(QPen(QColor(255, 140, 0), 4, Qt::SolidLine));
	painter->setBrush(Qt::transparent);
	painter->drawPath(path);

	painter->setPen(QPen(Qt::transparent, 0));
	painter->setBrush(QColor(255, 140, 0));
	painter->drawEllipse(-3.0, -3.0, 6.0, 6.0);

	/* Restore painter */
	painter->restore();
}

} // namespace
