#include "rqt_ardrone/navdata/status_panel.h"

#include <QPainter>
#include <QFont>

namespace rqt_ardrone {

StatusPanel::StatusPanel()
: currentStatus_(EMERGENCY)
{
}

QRectF StatusPanel::boundingRect() const
{
	return QRectF(-100.0, -100.0, 200.0, 200.0);
	//throw std::logic_error("Not implemented yet");
}

void StatusPanel::paint(QPainter *painter, 
						const QStyleOptionGraphicsItem *option,
						QWidget *widget)
{
	QSizeF size(80.0, 30.0);
	QPointF center(0.0, 0.0);
	QSizeF spacing(30.0, 20.0);

	float panelHeight;
	QRectF rect;

	QBrush greenOff(QColor(0, 60, 0));
	QBrush greenOn(QColor(0, 200, 0));
	QBrush redOff(QColor(60, 0, 0));
	QBrush redOn(QColor(250, 0, 0));
	QBrush orangeOff(QColor(70, 50, 0));
	QBrush orangeOn(QColor(250, 150, 0));

	QFont font = painter->font();
	font.setPointSize(9);

	painter->save();

	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter->setFont(font);

	/* Top line parameters */
	panelHeight = -spacing.height() - 1.5 * size.height();

	/* Top-left */
	rect = QRectF(
		QPointF(-spacing.width() - 1.5 * size.width(), 
			    panelHeight), 
		size);

	if (currentStatus_ == EMERGENCY)
		painter->setBrush(redOn);
	else
		painter->setBrush(redOff);

	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "EMERGENCY");

	/* Top-center */
	rect = QRectF(
		QPointF(- 0.5 * size.width(),
			    panelHeight),
		size);

	if (currentStatus_ == INITED)
		painter->setBrush(greenOn);
	else
		painter->setBrush(greenOff);
	
	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "INITED");

	/* Top-right */
	rect = QRectF(
		QPointF(spacing.width() + 0.5 * size.width(),
			    panelHeight),
		size);

	if (currentStatus_ == TEST)
		painter->setBrush(orangeOn);
	else
		painter->setBrush(orangeOff);
	
	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "TEST");

	/* Center line parameter */
	panelHeight = - 0.5 * size.height();

	/* Center-left */
	rect = QRectF(
		QPointF(-spacing.width() - 1.5 * size.width(), 
			    panelHeight), 
		size);

	if (currentStatus_ == LANDED)
		painter->setBrush(greenOn);
	else
		painter->setBrush(greenOff);
	
	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "LANDED");

	/* Center-center */
	rect = QRectF(
		QPointF(- 0.5 * size.width(),
			    panelHeight),
		size);

	if (currentStatus_ == TAKINGOFF)
		painter->setBrush(greenOn);
	else
		painter->setBrush(greenOff);
	
	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "TAKING OFF");

	/* Center-right */
	rect = QRectF(
		QPointF(spacing.width() + 0.5 * size.width(),
			    panelHeight),
		size);

	if (currentStatus_ == LANDING)
		painter->setBrush(greenOn);
	else
		painter->setBrush(greenOff);
	
	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "LANDING");

	/* Bottom line parameter */
	panelHeight = spacing.height() + 0.5 * size.height();

	/* Bottom-left */
	rect = QRectF(
		QPointF(-spacing.width() - 1.5 * size.width(), 
			    panelHeight), 
		size);

	if (currentStatus_ == FLYING)
		painter->setBrush(greenOn);
	else
		painter->setBrush(greenOff);
	
	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "FLYING");

	/* Bottom-center */
	rect = QRectF(
		QPointF(- 0.5 * size.width(),
			    panelHeight),
		size);

	if (currentStatus_ == GOTOHOVER)
		painter->setBrush(greenOn);
	else
		painter->setBrush(greenOff);
	
	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "GOTO HOVER");

	/* Bottom-right */
	rect = QRectF(
		QPointF(spacing.width() + 0.5 * size.width(),
			    panelHeight),
		size);

	if (currentStatus_ == HOVERING)
		painter->setBrush(greenOn);
	else
		painter->setBrush(greenOff);
	
	painter->drawRect(rect);
	painter->drawText(rect, Qt::AlignCenter, "HOVERING");

	painter->restore();
}

} // namespace
