#ifndef CQChartsPiePlot_H
#define CQChartsPiePlot_H

#include <CQChartsGroupPlot.h>
#include <CQChartsPlotObj.h>
#include <CQChartsTextBoxObj.h>
#include <CQChartsGeom.h>

#include <boost/optional.hpp>
#include <string>

//---

class CQChartsPiePlotType : public CQChartsGroupPlotType {
 public:
  CQChartsPiePlotType();

  QString name() const override { return "pie"; }
  QString desc() const override { return "Pie"; }

  Dimension dimension() const override { return Dimension::ONE_D; }

  void addParameters() override;

  bool hasAxes() const override { return false; }

  QString description() const override;

  CQChartsPlot *create(CQChartsView *view, const ModelP &model) const override;
};

//---

class CQChartsPiePlot;

//---

class CQChartsPieTextObj : public CQChartsRotatedTextBoxObj {
 public:
  CQChartsPieTextObj(CQChartsPiePlot *plot);

  CQChartsPiePlot *plot() const { return plot_; }

 private:
  CQChartsPiePlot* plot_ { nullptr };
};

//---

class CQChartsPieGroupObj;

class CQChartsPieObj : public CQChartsPlotObj {
 public:
  using OptReal = boost::optional<double>;

 public:
  CQChartsPieObj(CQChartsPiePlot *plot, const CQChartsGeom::BBox &rect, const QModelIndex &ind);

  QString calcId() const override;

  QString calcTipId() const override;

  double angle1() const { return angle1_; }
  void setAngle1(double a) { angle1_ = a; }

  double angle2() const { return angle2_; }
  void setAngle2(double a) { angle2_ = a; }

  double innerRadius() const { return ri_; }
  void setInnerRadius(double r) { ri_ = r; }

  double outerRadius() const { return ro_; }
  void setOuterRadius(double r) { ro_ = r; }

  double valueRadius() const { return rv_; }
  void setValueRadius(double r) { rv_ = r; }

  const QString &label() const { return label_; }
  void setLabel(const QString &s) { label_ = s; }

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  bool isMissing() const { return missing_; }
  void setMissing(bool b) { missing_ = b; }

  const OptReal &radius() const { return radius_; }
  void setRadius(const OptReal &r) { radius_ = r; }

  const QString &keyLabel() const { return keyLabel_; }
  void setKeyLabel(const QString &s) { keyLabel_ = s; }

  const CQChartsColor &color() const { return color_; }
  void setColor(const CQChartsColor &c) { color_ = c; }

  CQChartsPieGroupObj *groupObj() const { return groupObj_; }
  void setGroupObj(CQChartsPieGroupObj *p) { groupObj_ = p; }

  bool isExploded() const { return exploded_; }
  void setExploded(bool b) { exploded_ = b; }

  //---

  bool calcExploded() const;

  bool inside(const CQChartsGeom::Point &p) const override;

  void getSelectIndices(Indices &inds) const override;

  void addColumnSelectIndex(Indices &inds, const CQChartsColumn &column) const override;

  void draw(QPainter *painter) override;

  void drawFg(QPainter *painter) override;

  void drawSegmentLabel(QPainter *painter, const CQChartsGeom::Point &c);

  CQChartsGeom::Point getCenter() const;

 protected:
  CQChartsPiePlot*     plot_     { nullptr }; // parent plot
  QModelIndex          ind_;                  // model index
  double               angle1_   { 0 };       // wedge start angle
  double               angle2_   { 360 };     // wedge end angle
  double               ri_       { 0.0 };     // inner radius
  double               ro_       { 0.0 };     // outer radius
  double               rv_       { 0.0 };     // value radius
  QString              label_    { "" };      // label
  double               value_    { 0 };       // value
  bool                 missing_  { false };   // value missing
  OptReal              radius_;               // radius
  QString              keyLabel_ { "" };      // key label
  CQChartsColor        color_;                // color
  CQChartsPieGroupObj* groupObj_ { nullptr }; // parent group object
  bool                 exploded_ { false };   // exploded
};

//---

class CQChartsPieGroupObj : public CQChartsGroupObj {
 public:
  using PieObjs = std::vector<CQChartsPieObj *>;

 public:
  CQChartsPieGroupObj(CQChartsPiePlot *plot, const CQChartsGeom::BBox &bbox,
                      int groupInd, const QString &name, int ig, int ng);

  CQChartsPiePlot *plot() const { return plot_; }

  int groupInd() const { return groupInd_; }
  void setGroupInd(int i) { groupInd_ = i; }

  const QString &name() const { return name_; }
  void setName(const QString &s) { name_ = s; }

  double dataTotal() const { return dataTotal_; }
  void setDataTotal(double r) { dataTotal_ = r; }

  int numValues() const { return numValues_; }
  void setNumValues(int n) { numValues_ = n; }

  double radiusMax() const { return radiusMax_; }
  void setRadiusMax(double r) { radiusMax_ = r; }

  bool isRadiusScaled() const { return radiusScaled_; }
  void setRadiusScaled(bool b) { radiusScaled_ = b; }

  double innerRadius() const { return innerRadius_; }
  void setInnerRadius(double r) { innerRadius_ = r; }

  double outerRadius() const { return outerRadius_; }
  void setOuterRadius(double r) { outerRadius_ = r; }

  void addObject(CQChartsPieObj *obj);

  CQChartsPieObj *lookupObj(const QString &name) const;

  int numObjs() const { return objs_.size(); }

  const PieObjs &objs() const { return objs_; }

  void setAngles(double a1, double a2) { startAngle_ = a1; endAngle_ = a2; }

  QString calcId() const override;

  QString calcTipId() const override;

  void getSelectIndices(Indices &) const override { }

  void addColumnSelectIndex(Indices &, const CQChartsColumn &) const override { }

  bool inside(const CQChartsGeom::Point &p) const override;

  void draw(QPainter *painter) override;

  void drawFg(QPainter *painter) override;

  QColor bgColor() const;

 private:
  CQChartsPiePlot* plot_         { nullptr }; // parent plot
  int              groupInd_     { -1 };      // groupInd
  QString          name_;                     // group name
  int              ig_           { 0 };       // group index
  int              ng_           { 1 };       // num groups
  double           dataTotal_    { 0.0 };     // value data total
  int              numValues_    { 0 };       // num values
  double           radiusMax_    { 0.0 };     // radius data max
  bool             radiusScaled_ { false };   // radius scaled
  double           innerRadius_  { 0.0 };     // inner radius
  double           outerRadius_  { 0.0 };     // outer radius
  PieObjs          objs_;                     // objects
  double           startAngle_   { 0.0 };
  double           endAngle_     { 0.0 };
};

//---

#include <CQChartsKey.h>

class CQChartsPieKeyColor : public CQChartsKeyColorBox {
  Q_OBJECT

 public:
  CQChartsPieKeyColor(CQChartsPiePlot *plot, CQChartsPlotObj *obj);

  bool selectPress(const CQChartsGeom::Point &p) override;

  QBrush fillBrush() const override;

 private:
  CQChartsPlotObj *obj_ { nullptr };
};

class CQChartsPieKeyText : public CQChartsKeyText {
  Q_OBJECT

 public:
  CQChartsPieKeyText(CQChartsPiePlot *plot, CQChartsPlotObj *obj);

  QColor interpTextColor(int i, int n) const override;

 private:
  CQChartsPlotObj *obj_ { nullptr };
};

//---

class CQChartsPiePlot : public CQChartsGroupPlot {
  Q_OBJECT

  // properties
  //   donut, inner radius, outer radius, label radius, start angle, end angle,
  //   explode/explode radius

  Q_PROPERTY(CQChartsColumn labelColumn    READ labelColumn     WRITE setLabelColumn    )
  Q_PROPERTY(CQChartsColumn valueColumn    READ valueColumn     WRITE setValueColumn    )
  Q_PROPERTY(QString        valueColumns   READ valueColumnsStr WRITE setValueColumnsStr)
  Q_PROPERTY(CQChartsColumn radiusColumn   READ radiusColumn    WRITE setRadiusColumn   )
  Q_PROPERTY(CQChartsColumn keyLabelColumn READ keyLabelColumn  WRITE setKeyLabelColumn )
  Q_PROPERTY(CQChartsColumn colorColumn    READ colorColumn     WRITE setColorColumn    )

  // options
  Q_PROPERTY(bool   donut           READ isDonut           WRITE setDonut          )
  Q_PROPERTY(bool   count           READ isCount           WRITE setCount          )
  Q_PROPERTY(double innerRadius     READ innerRadius       WRITE setInnerRadius    )
  Q_PROPERTY(double outerRadius     READ outerRadius       WRITE setOuterRadius    )
  Q_PROPERTY(double labelRadius     READ labelRadius       WRITE setLabelRadius    )
  Q_PROPERTY(double startAngle      READ startAngle        WRITE setStartAngle     )
  Q_PROPERTY(double angleExtent     READ angleExtent       WRITE setAngleExtent    )
  Q_PROPERTY(bool   rotatedText     READ isRotatedText     WRITE setRotatedText    )
  Q_PROPERTY(bool   explodeSelected READ isExplodeSelected WRITE setExplodeSelected)
  Q_PROPERTY(double explodeRadius   READ explodeRadius     WRITE setExplodeRadius  )

  // grid
  Q_PROPERTY(bool             grid      READ isGrid    WRITE setGrid     )
  Q_PROPERTY(CQChartsColor    gridColor READ gridColor WRITE setGridColor)
  Q_PROPERTY(double           gridAlpha READ gridAlpha WRITE setGridAlpha)
  Q_PROPERTY(CQChartsLength   gridWidth READ gridWidth WRITE setGridWidth)
  Q_PROPERTY(CQChartsLineDash gridDash  READ gridDash  WRITE setGridDash )

  // color map
  Q_PROPERTY(bool   colorMapped READ isColorMapped WRITE setColorMapped)
  Q_PROPERTY(double colorMapMin READ colorMapMin   WRITE setColorMapMin)
  Q_PROPERTY(double colorMapMax READ colorMapMax   WRITE setColorMapMax)

 public:
  CQChartsPiePlot(CQChartsView *view, const ModelP &model);
 ~CQChartsPiePlot();

  //---

  const CQChartsColumn &labelColumn() const { return labelColumn_; }
  void setLabelColumn(const CQChartsColumn &c);

  //---

  const CQChartsColumn &valueColumn() const;
  void setValueColumn(const CQChartsColumn &c);

  const Columns &valueColumns() const { return valueColumns_.columns(); }
  void setValueColumns(const Columns &valueColumns);

  QString valueColumnsStr() const;
  bool setValueColumnsStr(const QString &s);

  const CQChartsColumn &valueColumnAt(int i) const;

  //---

  const CQChartsColumn &radiusColumn() const { return radiusColumn_; }
  void setRadiusColumn(const CQChartsColumn &c);

  const CQChartsColumn &keyLabelColumn() const { return keyLabelColumn_; }
  void setKeyLabelColumn(const CQChartsColumn &c);

  //---

  bool isDonut() const { return donut_; }

  bool isCount() const { return count_; }

  //---

  bool isGrid() const { return gridData_.visible; }
  void setGrid(bool b);

  const CQChartsColor &gridColor() const { return gridData_.color; }
  void setGridColor(const CQChartsColor &c);

  QColor interpGridColor(int i, int n);

  double gridAlpha() const { return gridData_.alpha; }
  void setGridAlpha(double r);

  const CQChartsLength &gridWidth() const { return gridData_.width; }
  void setGridWidth(const CQChartsLength &l);

  const CQChartsLineDash &gridDash() const { return gridData_.dash; }
  void setGridDash(const CQChartsLineDash &l);

  //---

  double innerRadius() const { return innerRadius_; }
  void setInnerRadius(double r);

  double outerRadius() const { return outerRadius_; }
  void setOuterRadius(double r);

  double labelRadius() const { return labelRadius_; }
  void setLabelRadius(double r);

  double startAngle() const { return startAngle_; }
  void setStartAngle(double r);

  double angleExtent() const { return angleExtent_; }
  void setAngleExtent(double r);

  //---

  bool isRotatedText() const { return rotatedText_; }
  void setRotatedText(bool b);

  bool isExplodeSelected() const { return explodeSelected_; }
  void setExplodeSelected(bool b);

  double explodeRadius() const { return explodeRadius_; }
  void setExplodeRadius(double r);

  //---

  CQChartsPieTextObj *textBox() const { return textBox_; }

  //---

  const CQChartsColumn &colorColumn() const { return valueSetColumn("color"); }
  void setColorColumn(const CQChartsColumn &c) {
    setValueSetColumn("color", c); updateRangeAndObjs(); }

  bool isColorMapped() const { return isValueSetMapped("color"); }
  void setColorMapped(bool b) { setValueSetMapped("color", b); updateObjs(); }

  double colorMapMin() const { return valueSetMapMin("color"); }
  void setColorMapMin(double r) { setValueSetMapMin("color", r); updateObjs(); }

  double colorMapMax() const { return valueSetMapMax("color"); }
  void setColorMapMax(double r) { setValueSetMapMax("color", r); updateObjs(); }

  //---

  void addProperties() override;

  void updateRange(bool apply=true) override;

  void updateObjs() override;

  bool initObjs() override;

  void adjustObjAngles();

  void addKeyItems(CQChartsPlotKey *key) override;

  bool addMenuItems(QMenu *menu) override;

  //---

  int numGroupObjs() const { return groupObjs_.size(); }

  //---

  void handleResize() override;

  //---

  bool selectInvalidateObjs() const override { return true; }

 public slots:
  void setDonut(bool b);

  void setCount(bool b);

 private:
  void addRow(const QModelIndex &parent, int r);

  void addRowColumn(const CQChartsModelIndex &ind);

  void calcDataTotal();

  void addRowDataTotal(const QModelIndex &parent, int row);

  void addRowColumnDataTotal(const CQChartsModelIndex &ind);

  bool getColumnSizeValue(const CQChartsModelIndex &ind, double &value, bool &missing) const;

 private:
  struct GroupData {
    GroupData(const QString &name) :
     name(name) {
    }

    QString              name;                     // name
    double               dataTotal    { 0.0 };     // data column value total
    int                  numValues    { 0 };       // data column num values
    double               radiusMax    { 0.0 };     // radius column value max
    bool                 radiusScaled { false };   // has radius column value max
    CQChartsPieGroupObj *groupObj     { nullptr }; // associated group obj
  };

  using GroupInd   = std::map<QString,int>;
  using GroupDatas = std::map<int,GroupData>;
  using GroupObjs  = std::vector<CQChartsPieGroupObj *>;

  CQChartsColumn      labelColumn_     { 0 };       // label column
  CQChartsColumns     valueColumns_;                // value columns
  CQChartsColumn      radiusColumn_;                // radius value column
  CQChartsColumn      keyLabelColumn_;              // key label column
  bool                donut_           { false };   // is donut
  bool                count_           { false };   // show value counts
  double              innerRadius_     { 0.6 };     // relative inner donut radius
  double              outerRadius_     { 0.9 };     // relative outer donut radius
  double              labelRadius_     { 1.1 };     // label radus
  double              startAngle_      { 90 };      // first pie start angle
  double              angleExtent_     { 360.0 };   // pie angle extent
  bool                rotatedText_     { false };   // is label rotated
  bool                explodeSelected_ { true };    // explode selected pie
  double              explodeRadius_   { 0.05 };    // expose radus
  CQChartsLineData    gridData_;                    // grid line data
  GroupDatas          groupDatas_;                  // data per group
  CQChartsPieTextObj* textBox_         { nullptr }; // text box
  CQChartsGeom::Point center_;                      // center point
  GroupObjs           groupObjs_;                   // group objects
};

#endif
