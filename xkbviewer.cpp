#include "xkbviewer.h"
#include <QX11Info>
#include <QGraphicsPolygonItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsTextItem>
#include <QtDebug>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBgeom.h>
#include "xkbinfo.h"


XkbViewer::XkbViewer(QWidget *parent) :
	QGraphicsView(parent)
{
	setScene(new QGraphicsScene(this));

	connect(scene(), SIGNAL(selectionChanged()), this, SLOT(sceneSelectionChanged()));

	m_info = new XkbInfo(0, this);

#if 0
	int keycodesNumber = m_xkb->max_key_code - m_xkb->min_key_code + 1;

	for (int i = 0; false && i < m_xkb->map->num_types; i++) {
		XkbKeyTypeRec *type = m_xkb->map->types + i;
		i++; i--;
	}

	for (int i = 0; i < m_xkb->map->num_syms; i++) {
		KeySym *sym = m_xkb->map->syms + i;
		i++; i--;
	}
#endif
}

QList<QPolygon> XkbViewer::getShapes() {
	QList<QPolygon> shapes;

	XkbGeometryRec *geom = m_info->xkb()->geom;

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

	XkbGeometryRec *geom = m_info->xkb()->geom;

	for (int i = 0; i < geom->num_sections; i++) {
		XkbSectionRec *section = geom->sections + i;

		for (int j = 0; j < section->num_rows; j++) {
			XkbRowRec *row = section->rows + j;

			int leftFromRow = 0;
			for (int k = 0; k < row->num_keys; k++) {
				XkbKeyRec *key = row->keys + k;

				QString label = m_info->getName4(key->name);

				QPolygon shape = shapes[key->shape_ndx];
				//shape.translate(section->left + row->left + leftFromRow, section->top + row->top);

				QGraphicsPolygonItem *keyItem = scene()->addPolygon(shape);
				// position
				leftFromRow += key->gap;
				keyItem->translate(section->left + row->left + leftFromRow, section->top + row->top);
				leftFromRow += shape.boundingRect().width();

				// flags
				keyItem->setFlag(QGraphicsItem::ItemIsSelectable);
				keyItem->setData(Qt::UserRole, label);

				// appearance
				QColor color = colors[qrand() % colors.size()];
				color.setAlpha(50);
				keyItem->setBrush(color);

				// label
				QGraphicsSimpleTextItem *labelItem = new QGraphicsSimpleTextItem(label, keyItem);// scene()->addSimpleText(label);
				labelItem->setTransformOriginPoint(labelItem->boundingRect().center());
				labelItem->setScale(3);
				//labelItem->setParentItem(keyItem);
				QRectF labelRect = labelItem->boundingRect();
				labelRect.moveCenter(keyItem->boundingRect().center());
				labelItem->setPos(labelRect.topLeft());
			}
		}
	}
}

void XkbViewer::setScalePercent(int percent) {
	resetTransform();
	scale(percent / 100., percent / 100.);
}

void XkbViewer::sceneSelectionChanged() {
	QList<QGraphicsItem*> items = scene()->selectedItems();
	if (items.size() != 1)
		return;

	QGraphicsItem *item = items.at(0);
	//if (item->data(Qt::UserRole))
	qDebug() << item->data(Qt::UserRole).toString();
}
