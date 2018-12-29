#include <CQChartsResizeHandle.h>
#include <CQChartsPlot.h>
#include <CQChartsView.h>
#include <CQChartsUtil.h>
#include <QPainter>

CQChartsResizeHandle::
CQChartsResizeHandle(const CQChartsView *view, Side side) :
 view_(view), side_(side)
{
}

CQChartsResizeHandle::
CQChartsResizeHandle(const CQChartsPlot *plot, Side side) :
 plot_(plot), side_(side)
{
}

void
CQChartsResizeHandle::
draw(QPainter *painter)
{
  // set pen and brush
  QPen   pen;
  QBrush brush;

  QColor pc = borderColor();
  QColor bc = fillColor();

  if (isSelected())
    bc = CQChartsUtil::invColor(bc);

  if      (plot()) {
    plot()->setPen  (pen  , true, pc, 1.0);
    plot()->setBrush(brush, true, bc, fillAlpha());
  }
  else if (view()) {
    view()->setPen  (pen  , true, pc, 1.0);
    view()->setBrush(brush, true, bc, fillAlpha());
  }

  painter->setPen  (pen);
  painter->setBrush(brush);

  //---

  double cs = 16;

  path_ = QPainterPath();

  if      (side() == Side::MOVE) {
    double ms1 = 12;
    double ms2 = 4;

    QPointF c = CQChartsUtil::toQPoint(windowToPixel(bbox_.getCenter()));

    path_.moveTo(c.x() - ms1, c.y()      );
    path_.lineTo(c.x() - ms2, c.y() + ms2);
    path_.lineTo(c.x()      , c.y() + ms1);
    path_.lineTo(c.x() + ms2, c.y() + ms2);
    path_.lineTo(c.x() + ms1, c.y()      );
    path_.lineTo(c.x() + ms2, c.y() - ms2);
    path_.lineTo(c.x()      , c.y() - ms1);
    path_.lineTo(c.x() - ms2, c.y() - ms2);
    path_.closeSubpath();
  }
  else if (side() == Side::LL) {
    QPointF ll = CQChartsUtil::toQPoint(windowToPixel(bbox_.getLL()));

    path_.addEllipse(ll.x() - cs/2, ll.y() - cs/2, cs, cs);

    //path_.moveTo(ll.x()     , ll.y()     );
    //path_.lineTo(ll.x() + cs, ll.y()     );
    //path_.lineTo(ll.x()     , ll.y() - cs);
    //path_.closeSubpath();
  }
  else if (side() == Side::LR) {
    QPointF lr = CQChartsUtil::toQPoint(windowToPixel(bbox_.getLR()));

    path_.addEllipse(lr.x() - cs/2, lr.y() - cs/2, cs, cs);

    //path_.moveTo(lr.x()     , lr.y()     );
    //path_.lineTo(lr.x() - cs, lr.y()     );
    //path_.lineTo(lr.x()     , lr.y() - cs);
    //path_.closeSubpath();
  }
  else if (side() == Side::UL) {
    QPointF ul = CQChartsUtil::toQPoint(windowToPixel(bbox_.getUL()));

    path_.addEllipse(ul.x() - cs/2, ul.y() - cs/2, cs, cs);

    //path_.moveTo(ul.x()     , ul.y()     );
    //path_.lineTo(ul.x() + cs, ul.y()     );
    //path_.lineTo(ul.x()     , ul.y() + cs);
    //path_.closeSubpath();
  }
  else if (side() == Side::UR) {
    QPointF ur = CQChartsUtil::toQPoint(windowToPixel(bbox_.getUR()));

    path_.addEllipse(ur.x() - cs/2, ur.y() - cs/2, cs, cs);

    //path_.moveTo(ur.x()     , ur.y()     );
    //path_.lineTo(ur.x() - cs, ur.y()     );
    //path_.lineTo(ur.x()     , ur.y() + cs);
    //path_.closeSubpath();
  }
  else {
    return;
  }

  painter->drawPath(path_);
}

bool
CQChartsResizeHandle::
selectInside(const CQChartsGeom::Point &p)
{
  bool selected = inside(p);

  if (selected == isSelected())
    return false;

  setSelected(selected);

  return true;
}

bool
CQChartsResizeHandle::
inside(const CQChartsGeom::Point &w) const
{
  QPointF p = CQChartsUtil::toQPoint(windowToPixel(w));

  return path_.contains(p);
}

CQChartsGeom::Point
CQChartsResizeHandle::
windowToPixel(const CQChartsGeom::Point &p) const
{
  return (view_ ? view_->windowToPixel(p) : plot_->windowToPixel(p));
}
