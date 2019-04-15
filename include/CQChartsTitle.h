#ifndef CQChartsTitle_H
#define CQChartsTitle_H

#include <CQChartsTextBoxObj.h>
#include <CQChartsTitleLocation.h>
#include <CQChartsPosition.h>
#include <CQChartsRect.h>
#include <CQChartsGeom.h>
#include <QPointF>
#include <QSizeF>
#include <vector>

class CQChartsPlot;
class CQChartsEditHandles;
class CQPropertyViewModel;
class QPainter;

/*!
 * \brief Title Object
 */
class CQChartsTitle : public CQChartsTextBoxObj {
  Q_OBJECT

  Q_PROPERTY(CQChartsTitleLocation location    READ location     WRITE setLocation   )
  Q_PROPERTY(CQChartsPosition      absPosition READ absPosition  WRITE setAbsPosition)
  Q_PROPERTY(CQChartsRect          absRect     READ absRect      WRITE setAbsRect    )
  Q_PROPERTY(bool                  insidePlot  READ isInsidePlot WRITE setInsidePlot )

 public:
  CQChartsTitle(CQChartsPlot *plot);
 ~CQChartsTitle();

  QString calcId() const override;

  void setSelected(bool b) override;

  //---

  const CQChartsTitleLocation &location() const { return location_; }
  void setLocation(const CQChartsTitleLocation &l) { location_ = l; redraw(); }

  const CQChartsPosition &absPosition() const { return absPosition_; }
  void setAbsPosition(const CQChartsPosition &p) { absPosition_ = p; redraw(); }

  const CQChartsRect &absRect() const { return absRect_; }
  void setAbsRect(const CQChartsRect &r) { absRect_ = r; redraw(); }

  bool isInsidePlot() const { return insidePlot_; }
  void setInsidePlot(bool b) { insidePlot_ = b; }

  //---

  const QPointF &position() const { return position_; }
  void setPosition(const QPointF &p) { position_ = p; }

  CQChartsEditHandles *editHandles() { return editHandles_; }

  //---

  QString locationStr() const;
  void setLocationStr(const QString &s);

  //---

  QPointF absPlotPosition() const;
  void setAbsPlotPosition(const QPointF &p);

  //---

  const CQChartsGeom::BBox &bbox() const { return bbox_; }
  void setBBox(const CQChartsGeom::BBox &b) { bbox_ = b; }

  //---

  void boxDataInvalidate() override { redraw(); }

  //---

  void addProperties(CQPropertyViewModel *model, const QString &path) override;

  //---

  QSizeF calcSize();

  //---

  void redraw(bool wait=true);

  //---

  bool contains(const CQChartsGeom::Point &p) const;

  //---

  virtual bool selectPress  (const CQChartsGeom::Point &) { return false; }
  virtual bool selectMove   (const CQChartsGeom::Point &) { return false; }
  virtual bool selectRelease(const CQChartsGeom::Point &) { return false; }

  virtual bool editPress  (const CQChartsGeom::Point &);
  virtual bool editMove   (const CQChartsGeom::Point &);
  virtual bool editMotion (const CQChartsGeom::Point &);
  virtual bool editRelease(const CQChartsGeom::Point &);

  virtual void editMoveBy(const QPointF &d);

  //---

  bool isDrawn() const;

  void draw(QPainter *painter);

  void drawEditHandles(QPainter *painter) const;

 private:
  void updateLocation();

 private:
  CQChartsTitleLocation      location_;                //! location type
  CQChartsPosition           absPosition_;             //! position (relative to plot box)
  CQChartsRect               absRect_;                 //! rect (relative to plot box)
  bool                       insidePlot_  { false };   //! is placed inside plot
  QPointF                    position_    { 0, 0 };    //! position
  QSizeF                     size_;                    //! size
  mutable CQChartsGeom::BBox bbox_;                    //! bbox
  CQChartsEditHandles*       editHandles_ { nullptr }; //! edit handles
};

#endif
