#ifndef XKBGEOM_H
#define XKBGEOM_H

#include <QList>
#include <QPolygon>
#include <QString>

class _XDisplay;
class _XkbDesc;

class XkbGeom {
public:
	typedef QPolygon Shape;

	struct Key {
		QString name;
		Shape shape;
		int x;
	};

	struct Row {
		QList<Key> keys;
		int x, y;
	};

	struct Section {
		QList<Row> rows;
		int x, y;
	};


	XkbGeom(_XDisplay *dpy, _XkbDesc *xkb);

	QList<Section> getSections() const;

private:
	QList<Shape> getShapes() const;

	_XDisplay *m_dpy;
	_XkbDesc *m_xkb;
};

#endif // XKBGEOM_H
