#ifndef CQChartsAxis_H
#define CQChartsAxis_H

#include <CQChartsTextBoxObj.h>
#include <CQChartsLineObj.h>
#include <QColor>
#include <QFont>
#include <CLineDash.h>
#include <CBBox2D.h>

#include <sys/types.h>

#include <map>
#include <vector>
#include <string>

#include <boost/optional.hpp>

class CQChartsAxis;
class CQChartsPlot;
class CQPropertyViewTree;
class QPainter;

class CQChartsAxisLabel : public CQChartsTextBoxObj {
  Q_OBJECT

 public:
  CQChartsAxisLabel(CQChartsAxis *axis);

  void redrawBoxObj() override;

 private:
  CQChartsAxis *axis_ { nullptr };
};

//---

class CQChartsAxisTickLabel : public CQChartsTextBoxObj {
  Q_OBJECT

 public:
  CQChartsAxisTickLabel(CQChartsAxis *axis);

  void redrawBoxObj() override;

 private:
  CQChartsAxis *axis_ { nullptr };
};

//---

// Axis Data
class CQChartsAxis : public QObject {
  Q_OBJECT

  // general
  Q_PROPERTY(bool      visible     READ getVisible   WRITE setVisible    )
  Q_PROPERTY(Direction direction   READ getDirection WRITE setDirection  )
  Q_PROPERTY(Side      side        READ getSide      WRITE setSide       )
  Q_PROPERTY(bool      hasPosition READ hasPosition  WRITE setHasPosition)
  Q_PROPERTY(double    position    READ position     WRITE setPosition   )
  Q_PROPERTY(bool      integral    READ isIntegral   WRITE setIntegral   )
  Q_PROPERTY(QString   format      READ format       WRITE setFormat     )

  // line
  Q_PROPERTY(bool      lineDisplayed READ getLineDisplayed WRITE setLineDisplayed)
  Q_PROPERTY(QColor    lineColor     READ getLineColor     WRITE setLineColor    )
  Q_PROPERTY(double    lineWidth     READ getLineWidth     WRITE setLineWidth    )
  Q_PROPERTY(CLineDash lineDash      READ getLineDash      WRITE setLineDash     )

  // ticks
  Q_PROPERTY(bool minorTicksDisplayed READ getMinorTicksDisplayed WRITE setMinorTicksDisplayed)
  Q_PROPERTY(bool majorTicksDisplayed READ getMajorTicksDisplayed WRITE setMajorTicksDisplayed)
  Q_PROPERTY(int  minorTickLen        READ getMinorTickLen        WRITE setMinorTickLen       )
  Q_PROPERTY(int  majorTickLen        READ getMajorTickLen        WRITE setMajorTickLen       )

  // ticks label
  Q_PROPERTY(bool   tickLabelDisplayed READ isTickLabelDisplayed WRITE setTickLabelDisplayed)
  Q_PROPERTY(QFont  tickLabelFont      READ getTickLabelFont     WRITE setTickLabelFont     )
  Q_PROPERTY(QColor tickLabelColor     READ getTickLabelColor    WRITE setTickLabelColor    )
  Q_PROPERTY(double tickLabelAngle     READ getTickLabelAngle    WRITE setTickLabelAngle    )
  Q_PROPERTY(bool   tickLabelAutoHide  READ isTickLabelAutoHide  WRITE setTickLabelAutoHide )

  // label
  Q_PROPERTY(bool    labelDisplayed READ isLabelDisplayed WRITE setLabelDisplayed)
  Q_PROPERTY(QString label          READ getLabel         WRITE setLabel         )
  Q_PROPERTY(QFont   labelFont      READ getLabelFont     WRITE setLabelFont     )
  Q_PROPERTY(QColor  labelColor     READ getLabelColor    WRITE setLabelColor    )

  // grid line/fill
  Q_PROPERTY(bool      gridDisplayed READ getGridDisplayed WRITE setGridDisplayed)
  Q_PROPERTY(QColor    gridColor     READ getGridColor     WRITE setGridColor    )
  Q_PROPERTY(double    gridWidth     READ getGridWidth     WRITE setGridWidth    )
  Q_PROPERTY(CLineDash gridDash      READ getGridDash      WRITE setGridDash     )
  Q_PROPERTY(bool      gridAbove     READ isGridAbove      WRITE setGridAbove    )
  Q_PROPERTY(bool      gridFill      READ isGridFill       WRITE setGridFill     )
  Q_PROPERTY(QColor    gridFillColor READ gridFillColor    WRITE setGridFillColor)
  Q_PROPERTY(double    gridFillAlpha READ gridFillAlpha    WRITE setGridFillAlpha)

  Q_ENUMS(Direction)
  Q_ENUMS(Side)

 public:
  enum class Direction {
    HORIZONTAL,
    VERTICAL
  };

  enum class Side {
    BOTTOM_LEFT,
    TOP_RIGHT
  };

  typedef boost::optional<double> OptReal;

 public:
  CQChartsAxis(CQChartsPlot *plot, Direction direction=Direction::HORIZONTAL,
               double start=0.0, double end=1.0);

 ~CQChartsAxis();

  bool getVisible() const { return visible_; }
  void setVisible(bool b) { visible_ = b; }

  Direction getDirection() const { return direction_; }
  void setDirection(Direction dir) { direction_ = dir; }

  Side getSide() const { return side_; }
  void setSide(Side side) { side_ = side; updatePlotPosition(); }

  double getStart() const { return start_; }
  void setStart(double start) { setRange(start, end_); }

  double getEnd() const { return end_; }
  void setEnd(double end) { setRange(start_, end); }

  void setRange(double start, double end);

  bool isIntegral() const { return integral_; }
  void setIntegral(bool b);

  int column() const { return column_; }
  void setColumn(int i) { column_ = i; }

  bool isDataLabels() const { return dataLabels_; }
  void setDataLabels(bool b) { dataLabels_ = b; }

  QString format() const;
  bool setFormat(const QString &s);

  //---

  // label

  bool isLabelDisplayed() const;
  void setLabelDisplayed(bool b);

  const QString &getLabel() const;
  void setLabel(const QString &str);

  const QFont &getLabelFont() const;
  void setLabelFont(const QFont &font);

  const QColor &getLabelColor() const;
  void setLabelColor(const QColor &color);

  //---

  // line

  bool getLineDisplayed() const { return lineObj_.isDisplayed(); }
  void setLineDisplayed(bool b) { lineObj_.setDisplayed(b); redraw(); }

  const QColor &getLineColor() const { return lineObj_.color(); }
  void setLineColor(const QColor &c) { lineObj_.setColor(c); redraw(); }

  double getLineWidth() const { return lineObj_.width(); }
  void setLineWidth(double r) { lineObj_.setWidth(r); redraw(); }

  const CLineDash &getLineDash() const { return lineObj_.dash(); }
  void setLineDash(const CLineDash &dash) { lineObj_.setDash(dash); redraw(); }

  //---

  // grid

  bool getGridDisplayed() const { return gridDisplayed_; }
  void setGridDisplayed(bool b) { gridDisplayed_ = b; redraw(); }

  const QColor &getGridColor() const { return gridLineObj_.color(); }
  void setGridColor(const QColor &c) { gridLineObj_.setColor(c); redraw(); }

  double getGridWidth() const { return gridLineObj_.width(); }
  void setGridWidth(double r) { gridLineObj_.setWidth(r); redraw(); }

  const CLineDash &getGridDash() const { return gridLineObj_.dash(); }
  void setGridDash(const CLineDash &dash) { gridLineObj_.setDash(dash); redraw(); }

  bool isGridAbove() const { return gridAbove_; }
  void setGridAbove(bool b) { gridAbove_ = b; redraw(); }

  bool isGridFill() const { return gridFill_; }
  void setGridFill(bool b) { gridFill_ = b; redraw(); }

  const QColor &gridFillColor() const { return gridFillColor_; }
  void setGridFillColor(const QColor &c) { gridFillColor_ = c; redraw(); }

  double gridFillAlpha() const { return gridFillAlpha_; }
  void setGridFillAlpha(double r) { gridFillAlpha_ = r; redraw(); }

  //---

  // ticks

  bool getMinorTicksDisplayed() const { return minorTicksDisplayed_; }
  void setMinorTicksDisplayed(bool b) { minorTicksDisplayed_ = b; redraw(); }

  bool getMajorTicksDisplayed() const { return majorTicksDisplayed_; }
  void setMajorTicksDisplayed(bool b) { majorTicksDisplayed_ = b; redraw(); }

  int getMinorTickLen() const { return minorTickLen_; }
  void setMinorTickLen(int i) { minorTickLen_ = i; redraw(); }

  int getMajorTickLen() const { return majorTickLen_; }
  void setMajorTickLen(int i) { majorTickLen_ = i; redraw(); }

  //---

  // ticks label

  bool isTickLabelDisplayed() const;
  void setTickLabelDisplayed(bool b);

  const QFont &getTickLabelFont() const;
  void setTickLabelFont(const QFont &font);

  const QColor &getTickLabelColor() const;
  void setTickLabelColor(const QColor &color);

  double getTickLabelAngle() const;
  void setTickLabelAngle(double r);

  bool isTickLabelAutoHide() const { return tickLabelAutoHide_; }
  void setTickLabelAutoHide(bool b) { tickLabelAutoHide_ = b; redraw(); }

  //---

  uint getNumMajorTicks() const { return numTicks1_; }
  void setNumMajorTicks(uint n) { numTicks1_ = n; redraw(); }

  uint getNumMinorTicks() const { return numTicks2_; }
  void setNumMinorTicks(uint n) { numTicks2_ = n; redraw(); }

  uint getTickIncrement() const { return tickIncrement_; }
  void setTickIncrement(uint tickIncrement);

  double getMinorIncrement() const;

  double getMajorIncrement() const;
  void setMajorIncrement(double i);

  //---

  // used ?
  const double *getTickSpaces   () const { return &tickSpaces_[0]; }
  uint          getNumTickSpaces() const { return tickSpaces_.size(); }

  double getTickSpace(int i) const { return tickSpaces_[i]; }
  void setTickSpaces(double *tickSpaces, uint numTickSpaces);

  //---

  void clearTickLabels() {
    tickLabels_.clear();
  }

  void setTickLabel(long i, const QString &label) { tickLabels_[i] = label; redraw(); }

  bool hasTickLabel(long i) const { return (tickLabels_.find(i) != tickLabels_.end()); }

  const QString &getTickLabel(long i) const {
    auto p = tickLabels_.find(i);
    assert(p != tickLabels_.end());

    return (*p).second;
  }

  //---

  const OptReal &pos() const { return pos_; }
  void setPos(double r) { pos_ = r; redraw(); }

  void unsetPos() { pos_.reset(); redraw(); }

  bool hasPosition() const { return !!pos_; }

  void setHasPosition(bool b) {
    if (! b) { if (  hasPosition()) unsetPos(); }
    else     { if (! hasPosition()) setPos(0.0); }
  }

  double position() const { return pos_.value_or(0.0); }
  void setPosition(double r) { setPos(r); }

  //---

  QString getValueStr(double pos) const;

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &b) { bbox_ = b; redraw(); }

  void addProperties(CQPropertyViewTree *tree, const QString &path);

  void updatePlotPosition();

  //---

  void redraw();

  //---

  void drawGrid(CQChartsPlot *plot, QPainter *p);

  void draw(CQChartsPlot *plot, QPainter *p);

 private:
  struct AxisGapData {
    double start       { 0.0 };
    double end         { 0.0 };
    double increment   { 0.0 };
    uint   numGaps     { 0 };
    uint   numGapTicks { 0 };
  };

  void calc();

  bool testAxisGaps(double start, double end, double testIncrement, uint testNumGapTicks,
                    AxisGapData &axisGapData);

 private:
  typedef std::vector<double>   TickSpaces;
  typedef std::map<int,QString> TickLabels;

  CQChartsPlot*          plot_                { nullptr };
  bool                   visible_             { true };
  Direction              direction_           { Direction::HORIZONTAL };
  Side                   side_                { Side::BOTTOM_LEFT };
  double                 start_               { 0.0 };
  double                 end_                 { 1.0 };
  double                 start1_              { 0 };
  double                 end1_                { 1 };
  bool                   integral_            { false };
  bool                   dataLabels_          { false };
  int                    column_              { -1 };

  // label
  bool                   labelDisplayed_  { true };
  CQChartsAxisLabel*     label_           { nullptr };

  // line
  CQChartsLineObj        lineObj_;

  // grid
  bool                   gridDisplayed_       { false };
  CQChartsLineObj        gridLineObj_;
  bool                   gridAbove_           { false };
  bool                   gridFill_            { false };
  QColor                 gridFillColor_       { 128, 128, 128 };
  double                 gridFillAlpha_       { 0.5 };

  // ticks
  bool                   minorTicksDisplayed_ { true };
  bool                   majorTicksDisplayed_ { true };
  int                    minorTickLen_        { 4 };
  int                    majorTickLen_        { 8 };

  // tick label
  bool                   tickLabelDisplayed_ { true };
  CQChartsAxisTickLabel *tickLabel_          { nullptr };
  bool                   tickLabelAutoHide_  { false };

  uint                   numTicks1_           { 1 };
  uint                   numTicks2_           { 0 };
  uint                   tickIncrement_       { 0 };
  double                 majorIncrement_      { 0 };
  TickSpaces             tickSpaces_;
  TickLabels             tickLabels_;

  OptReal                pos_;
  mutable CBBox2D        bbox_;
};

#endif
