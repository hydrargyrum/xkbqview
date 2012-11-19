#include "xkbviewer.h"
#include <QX11Info>
#include <QGraphicsPolygonItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsTextItem>
#include <QtDebug>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBgeom.h>


XkbViewer::XkbViewer(QWidget *parent) :
	QGraphicsView(parent)
{
	setScene(new QGraphicsScene(this));

	Display *xdisplay = QX11Info::display();

	/*
	XkbFileInfo info;
	memset(&info, 0, sizeof(info));
	XkbReadFromServer(xdisplay, 1, 1, &info);
	*/

	m_xkb = XkbGetMap(xdisplay, XkbAllMapComponentsMask, XkbUseCoreKbd);
	XkbGetGeometry(xdisplay, m_xkb);
}

QList<QPolygon> XkbViewer::getShapes() {
	QList<QPolygon> shapes;

	XkbGeometryRec *geom = m_xkb->geom;

	for (int i = 0; i < geom->num_shapes; i++) {
		XkbShapeRec *shape = geom->shapes + i;

		//for (int j = 0; j < shape->num_outlines; j++) {
		for (int j = 0; j < 1; j++) {
			XkbOutlineRec *outline = shape->outlines + j;

			QPolygon polygon;
			if (outline->num_points == 1) {
				polygon = QRect(0, 0, outline->points[0].x, outline->points[0].y);
			} else if (outline->num_points == 2) {
				polygon = QRect(QPoint(outline->points[0].x, outline->points[0].y),
							   QPoint(outline->points[1].x, outline->points[1].y));
			} else
				for (int k = 0; k < outline->num_points; k++) {
					polygon.append(QPoint(outline->points[k].x, outline->points[k].y));
				}

			if (polygon.first() != polygon.last())
				polygon.append(polygon.last());

			shapes.append(polygon);
		}
	}

	return shapes;
}

void XkbViewer::fill() {
	QList<QColor> colors;
	colors << Qt::red; colors << Qt::green; colors << Qt::blue; colors << Qt::magenta; colors << Qt::yellow; colors << Qt::cyan;

	scene()->clear();

	QList<QPolygon> shapes = getShapes();

	XkbGeometryRec *geom = m_xkb->geom;

	for (int i = 0; i < geom->num_sections; i++) {
		XkbSectionRec *section = geom->sections + i;

		for (int j = 0; j < section->num_rows; j++) {
			XkbRowRec *row = section->rows + j;

			int leftFromRow = 0;
			for (int k = 0; k < row->num_keys; k++) {
				XkbKeyRec *key = row->keys + k;

				QString label = QString::fromAscii(key->name.name, qstrnlen(key->name.name, XkbKeyNameLength));

				QPolygon shape = shapes[key->shape_ndx];
				//shape.translate(section->left + row->left + leftFromRow, section->top + row->top);

				QGraphicsPolygonItem *keyItem = scene()->addPolygon(shape);
				keyItem->translate(section->left + row->left + leftFromRow, section->top + row->top);

				QColor color = colors[qrand() % colors.size()];
				color.setAlpha(50);
				keyItem->setBrush(color);

				leftFromRow += shape.boundingRect().width() + key->gap;

				QGraphicsSimpleTextItem *labelItem = new QGraphicsSimpleTextItem(label, keyItem);// scene()->addSimpleText(label);
				labelItem->scale(2, 2);
				//labelItem->setParentItem(keyItem);
				QRectF labelRect = labelItem->boundingRect();
				labelRect.moveCenter(keyItem->boundingRect().center());
				labelItem->setPos(labelRect.topLeft());

				//keyItem->set
			}
		}
	}
}

void XkbViewer::setScalePercent(int percent) {
	resetTransform();
	scale(percent / 100., percent / 100.);
}
