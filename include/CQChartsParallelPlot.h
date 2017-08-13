#ifndef CQChartsParallelPlot_H
#define CQChartsParallelPlot_H

#include <CQChartsPlot.h>
#include <CQChartsPlotObj.h>
#include <CQUtil.h>
#include <CPen.h>
#include <CBrush.h>

class CQChartsParallelPlot;

class CQChartsParallelLineObj : public CQChartsPlotObj {
  Q_OBJECT

 public:
  CQChartsParallelLineObj(CQChartsParallelPlot *plot, const CBBox2D &rect,
                          const QPolygonF &poly, int ind);

  bool inside(const CPoint2D &p) const override;

  void draw(QPainter *p) override;

 private:
  CQChartsParallelPlot *plot_ { nullptr };
  QPolygonF             poly_;
  int                   ind_  { -1 };
};

//---

class CQChartsParallelPlot : public CQChartsPlot {
  Q_OBJECT

  // properties
  //  line
  //   display, stroke
  //  fill under
  //   display, brush
  //  symbol
  //   display, color, shape. ...
  //  margin
  //  key

  Q_PROPERTY(int xColumn READ xColumn WRITE setXColumn)
  Q_PROPERTY(int yColumn READ yColumn WRITE setYColumn)

 public:
  CQChartsParallelPlot(QAbstractItemModel *model);

  int xColumn() const { return xColumn_; }
  void setXColumn(int i) { xColumn_ = i; update(); }

  int yColumn() const { return yColumn_; }
  void setYColumn(int i) { yColumn_ = i; update(); }

  const Columns &yColumns() const { return yColumns_; }
  void setYColumns(const Columns &yColumns) { yColumns_ = yColumns; update(); }

  const CRange2D &yRange(int i) { return yRanges_[i]; }

  CQChartsAxis *yAxis(int i) { return yAxes_[i]; }

  void updateRange();

  void initObjs();

  int numSets() const;

  int getSetColumn(int i) const;

  int numValues() const;

  void paintEvent(QPaintEvent *) override;

 private:
  typedef std::vector<CRange2D>       Ranges;
  typedef std::vector<CQChartsAxis *> YAxes;

  int     xColumn_    { 0 };
  int     yColumn_    { 1 };
  Columns yColumns_;
  Ranges  yRanges_;
  YAxes   yAxes_;
};

#endif
