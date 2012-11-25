#include "xkbgeom.h"
#include <X11/XKBlib.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBgeom.h>
#include "xkbinfo.h"

XkbGeom::XkbGeom(QSharedPointer<_XkbDesc> xkb) :
	m_xkb(xkb)
{
}

QList<XkbGeom::Section> XkbGeom::getSections() const {
	QList<XkbGeom::Shape> shapes = getShapes();

	XkbGeometryRec *xGeom = m_xkb->geom;
	QList<Section> sections;

	QMap<QString, KeyCode> keycodes = XkbInfo(m_xkb).keycodesByName();

	for (int i = 0; i < xGeom->num_sections; i++) {
		XkbSectionRec *xSection = xGeom->sections + i;
		Section section;

		section.x = xSection->left;
		section.y = xSection->top;

		for (int j = 0; j < xSection->num_rows; j++) {
			XkbRowRec *xRow = xSection->rows + j;
			Row row;

			row.x = xRow->left;
			row.y = xRow->top;

			int leftFromRow = 0;
			for (int k = 0; k < xRow->num_keys; k++) {
				XkbKeyRec *xKey = xRow->keys + k;
				Key key;

				key.name = XkbInfo::getName4(xKey->name);
				key.shape = shapes[xKey->shape_ndx];
				key.keycode = keycodes.value(key.name);

				leftFromRow += xKey->gap;
				key.x = leftFromRow;
				leftFromRow += key.shape.boundingRect().width();

				row.keys.append(key);
			}

			section.rows.append(row);
		}

		sections.append(section);
	}

	return sections;
}

QList<XkbGeom::Shape> XkbGeom::getShapes() const {
	QList<XkbGeom::Shape> shapes;

	XkbGeometryRec *xGeom = m_xkb->geom;

	for (int i = 0; i < xGeom->num_shapes; i++) {
		XkbShapeRec *xShape = xGeom->shapes + i;

		//for (int j = 0; j < shape->num_outlines; j++) {
		for (int j = 0; j < 1; j++) {
			XkbOutlineRec *xOutline = xShape->outlines + j;

			XkbGeom::Shape shape;
			if (xOutline->num_points == 1) {
				shape = QRect(0, 0, xOutline->points[0].x, xOutline->points[0].y);
			} else if (xOutline->num_points == 2) {
				shape = QRect(QPoint(xOutline->points[0].x, xOutline->points[0].y),
							   QPoint(xOutline->points[1].x, xOutline->points[1].y));
			} else
				for (int k = 0; k < xOutline->num_points; k++) {
					shape.append(QPoint(xOutline->points[k].x, xOutline->points[k].y));
				}

			if (shape.first() != shape.last())
				shape.append(shape.last());

			shapes.append(shape);
		}
	}

	return shapes;
}

// TODO qpainterpath; rounded rectangles; builtin xkb "bounds"; rotations; doodads; indicators
// TODO design for modifying keyboard?
