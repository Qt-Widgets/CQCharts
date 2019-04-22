#ifndef CQChartsImagePlot_H
#define CQChartsImagePlot_H

#include <CQChartsPlot.h>
#include <CQChartsPlotType.h>
#include <CQChartsPlotObj.h>

class QMenu;

//---

/*!
 * \brief Image plot type
 */
class CQChartsImagePlotType : public CQChartsPlotType {
 public:
  CQChartsImagePlotType();

  QString name() const override { return "image"; }
  QString desc() const override { return "Image"; }

  Dimension dimension() const override { return Dimension::NONE; }

  void addParameters() override;

  bool hasAxes() const override { return false; }

  QString description() const override;

  CQChartsPlot *create(CQChartsView *view, const ModelP &model) const override;
};

//---

class CQChartsImagePlot;

/*!
 * \brief Image Plot object
 */
class CQChartsImageObj : public CQChartsPlotObj {
  Q_OBJECT

 public:
  CQChartsImageObj(const CQChartsImagePlot *plot, const CQChartsGeom::BBox &rect,
                   int row, int col, double value, const QModelIndex &ind);

  QString typeName() const override { return "image"; }

  QString calcId() const override;

  QString calcTipId() const override;

  void getSelectIndices(Indices &inds) const override;

  void addColumnSelectIndex(Indices &inds, const CQChartsColumn &column) const override;

  void draw(QPainter *painter) override;

 private:
  const CQChartsImagePlot* plot_  { nullptr };
  int                      row_   { -1 };
  int                      col_   { -1 };
  double                   value_ { 0.0 };
  QModelIndex              ind_;
};

//---

/*!
 * \brief Image Plot
 */
class CQChartsImagePlot : public CQChartsPlot,
 public CQChartsObjTextData<CQChartsImagePlot> {
  Q_OBJECT

  Q_PROPERTY(double minValue        READ minValue          WRITE setMinValue       )
  Q_PROPERTY(double maxValue        READ maxValue          WRITE setMaxValue       )
  Q_PROPERTY(bool   xLabels         READ isXLabels         WRITE setXLabels        )
  Q_PROPERTY(bool   yLabels         READ isYLabels         WRITE setYLabels        )
  Q_PROPERTY(bool   cellLabels      READ isCellLabels      WRITE setCellLabels     )
  Q_PROPERTY(bool   scaleCellLabels READ isScaleCellLabels WRITE setScaleCellLabels)
  Q_PROPERTY(bool   balloon         READ isBalloon         WRITE setBalloon        )

  CQCHARTS_TEXT_DATA_PROPERTIES

 public:
  CQChartsImagePlot(CQChartsView *view, const ModelP &model);

  //---

  double minValue() const { return minValue_; }
  void setMinValue(double r);

  double maxValue() const { return maxValue_; }
  void setMaxValue(double r);

  //---

  bool isXLabels() const { return xLabels_; }
  bool isYLabels() const { return yLabels_; }

  bool isCellLabels() const { return cellLabels_; }

  bool isScaleCellLabels() const { return scaleCellLabels_; }

  //---

  bool isBalloon() const { return balloon_; }
  void setBalloon(bool b);

  double minBalloonSize() const { return minBalloonSize_; }
  void setMinBalloonSize(double r) { minBalloonSize_ = r; }

  double maxBalloonSize() const { return maxBalloonSize_; }
  void setMaxBalloonSize(double r) { maxBalloonSize_ = r; }

  //---

  void addProperties() override;

  CQChartsGeom::Range calcRange() const override;

  bool createObjs(PlotObjs &objs) const override;

  //---

  bool addMenuItems(QMenu *menu) override;

  //---

  bool hasForeground() const override;

  void execDrawForeground(QPainter *) const override;

  //---

  CQChartsGeom::BBox annotationBBox() const override;

 public slots:
  void setXLabels(bool b);
  void setYLabels(bool b);

  void setCellLabels(bool b);

  void setScaleCellLabels(bool b);

 private:
  void addImageObj(int row, int col, double x, double y, double dx, double dy,
                   double value, const QModelIndex &ind, PlotObjs &objs) const;

  void drawXLabels(QPainter *) const;
  void drawYLabels(QPainter *) const;

 private:
  bool   xLabels_         { false }; //! x labels
  bool   yLabels_         { false }; //! y labels
  bool   cellLabels_      { false }; //! cell labels
  bool   scaleCellLabels_ { false }; //! scale cell labels
  bool   balloon_         { false }; //! draw balloon
  int    nc_              { 0 };     //! number of grid columns
  int    nr_              { 0 };     //! number of grid rows
  double minValue_        { 0.0 };   //! min value
  double maxValue_        { 0.0 };   //! max value
  double minBalloonSize_  { 0.1 };   //! min balloon size (cell fraction)
  double maxBalloonSize_  { 1.0 };   //! max balloon size (cell fraction)
};

#endif
