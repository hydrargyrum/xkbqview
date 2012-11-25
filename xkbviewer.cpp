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
#include "xkbgeom.h"

XkbViewer::XkbViewer(QWidget *parent) :
	QGraphicsView(parent)
{
	setScene(new QGraphicsScene(this));

	connect(scene(), SIGNAL(selectionChanged()), this, SLOT(sceneSelectionChanged()));

	m_info.reset(new XkbInfo());

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

XkbViewer::~XkbViewer() {
}

void XkbViewer::fill() {
	QList<QColor> colors;
	colors << Qt::red; colors << Qt::green; colors << Qt::blue; colors << Qt::magenta; colors << Qt::yellow; colors << Qt::cyan;

	scene()->clear();

	XkbGeom geom = m_info->geom();

	foreach (XkbGeom::Section section, geom.getSections()) {
		foreach (XkbGeom::Row row, section.rows) {
			foreach (XkbGeom::Key key, row.keys) {
				QGraphicsPolygonItem *keyItem = scene()->addPolygon(key.shape);
				keyItem->translate(key.x + row.x + section.x, row.y + section.y);
				keyItem->setFlag(QGraphicsItem::ItemIsSelectable);
				keyItem->setData(KeyCodeNameRole, key.name);
				keyItem->setData(KeyCodeRole, key.keycode);

				QColor color = colors[qrand() % colors.size()];
				color.setAlpha(50);
				keyItem->setBrush(color);

				QGraphicsSimpleTextItem *labelItem = new QGraphicsSimpleTextItem(key.name, keyItem);
				labelItem->setTransformOriginPoint(labelItem->boundingRect().center());
				labelItem->setScale(3);
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
	int kc = item->data(KeyCodeRole).toInt();
	emit selected(kc);


	QMap<KeyCode, QList<KeySym> > allsyms = m_info->keycodeSyms(0);
	qDebug() << m_info->symName(allsyms.value(kc).value(0));
}
