#include <CQChartsBoxPlot.h>
#include <CQChartsView.h>
#include <CQChartsAxis.h>
#include <CQChartsUtil.h>
#include <CQCharts.h>
#include <CQChartsTextBoxObj.h>
#include <CQChartsRoundedPolygon.h>
#include <CHRTimer.h>
#include <QPainter>

CQChartsBoxPlotType::
CQChartsBoxPlotType()
{
}

void
CQChartsBoxPlotType::
addParameters()
{
  addColumnParameter("x", "X", "xColumn", 0).setRequired().setTip("X Values");
  addColumnParameter("y", "Y", "yColumn", 1).setRequired().setTip("Y Values");

  addColumnParameter("group", "Group", "groupColumn").setTip("Extra grouping column");

  CQChartsPlotType::addParameters();
}

CQChartsPlot *
CQChartsBoxPlotType::
create(CQChartsView *view, const ModelP &model) const
{
  return new CQChartsBoxPlot(view, model);
}

//---

CQChartsBoxPlot::
CQChartsBoxPlot(CQChartsView *view, const ModelP &model) :
 CQChartsPlot(view, view->charts()->plotType("box"), model)
{
  setBoxColor(CQChartsColor(CQChartsColor::Type::PALETTE));

  setBorderStroked(true);
  setBoxFilled    (true);

  addAxes();

  addKey();

  addTitle();

  xAxis_->setIntegral(true);
}

CQChartsBoxPlot::
~CQChartsBoxPlot()
{
}

//------

bool
CQChartsBoxPlot::
isBoxFilled() const
{
  return boxData_.shape.background.visible;
}

void
CQChartsBoxPlot::
setBoxFilled(bool b)
{
  CQChartsUtil::testAndSet(boxData_.shape.background.visible, b, [&]() { update(); } );
}

const CQChartsColor &
CQChartsBoxPlot::
boxColor() const
{
  return boxData_.shape.background.color;
}

void
CQChartsBoxPlot::
setBoxColor(const CQChartsColor &c)
{
  CQChartsUtil::testAndSet(boxData_.shape.background.color, c, [&]() { update(); } );
}

QColor
CQChartsBoxPlot::
interpBoxColor(int i, int n) const
{
  return boxColor().interpColor(this, i, n);
}

double
CQChartsBoxPlot::
boxAlpha() const
{
  return boxData_.shape.background.alpha;
}

void
CQChartsBoxPlot::
setBoxAlpha(double a)
{
  CQChartsUtil::testAndSet(boxData_.shape.background.alpha, a, [&]() { update(); } );
}

CQChartsBoxPlot::Pattern
CQChartsBoxPlot::
boxPattern() const
{
  return (Pattern) boxData_.shape.background.pattern;
}

void
CQChartsBoxPlot::
setBoxPattern(Pattern pattern)
{
  if (pattern != (Pattern) boxData_.shape.background.pattern) {
    boxData_.shape.background.pattern = (CQChartsFillData::Pattern) pattern;

    update();
  }
}

//------

bool
CQChartsBoxPlot::
isBorderStroked() const
{
  return boxData_.shape.border.visible;
}

void
CQChartsBoxPlot::
setBorderStroked(bool b)
{
  CQChartsUtil::testAndSet(boxData_.shape.border.visible, b, [&]() { update(); } );
}

const CQChartsColor &
CQChartsBoxPlot::
borderColor() const
{
  return boxData_.shape.border.color;
}

void
CQChartsBoxPlot::
setBorderColor(const CQChartsColor &c)
{
  CQChartsUtil::testAndSet(boxData_.shape.border.color, c, [&]() { update(); } );
}

QColor
CQChartsBoxPlot::
interpBorderColor(int i, int n) const
{
  return borderColor().interpColor(this, i, n);
}

double
CQChartsBoxPlot::
borderAlpha() const
{
  return boxData_.shape.border.alpha;
}

void
CQChartsBoxPlot::
setBorderAlpha(double a)
{
  CQChartsUtil::testAndSet(boxData_.shape.border.alpha, a, [&]() { update(); } );
}

const CQChartsLength &
CQChartsBoxPlot::
borderWidth() const
{
  return boxData_.shape.border.width;
}

void
CQChartsBoxPlot::
setBorderWidth(const CQChartsLength &l)
{
  CQChartsUtil::testAndSet(boxData_.shape.border.width, l, [&]() { update(); } );
}

const CQChartsLength &
CQChartsBoxPlot::
cornerSize() const
{
  return boxData_.cornerSize;
}

void
CQChartsBoxPlot::
setCornerSize(const CQChartsLength &s)
{
  CQChartsUtil::testAndSet(boxData_.cornerSize, s, [&]() { update(); } );
}

//------

const CQChartsColor &
CQChartsBoxPlot::
whiskerColor() const
{
  return whiskerData_.color;
}

void
CQChartsBoxPlot::
setWhiskerColor(const CQChartsColor &c)
{
  whiskerData_.color = c;

  update();
}

double
CQChartsBoxPlot::
whiskerAlpha() const
{
  return whiskerData_.alpha;
}

void
CQChartsBoxPlot::
setWhiskerAlpha(double a)
{
  CQChartsUtil::testAndSet(whiskerData_.alpha, a, [&]() { update(); } );
}

QColor
CQChartsBoxPlot::
interpWhiskerColor(int i, int n) const
{
  return whiskerColor().interpColor(this, i, n);
}

//------

bool
CQChartsBoxPlot::
isTextVisible() const
{
  return textData_.visible;
}

void
CQChartsBoxPlot::
setTextVisible(bool b)
{
  CQChartsUtil::testAndSet(textData_.visible, b, [&]() { update(); } );
}

const CQChartsColor &
CQChartsBoxPlot::
textColor() const
{
  return textData_.color;
}

void
CQChartsBoxPlot::
setTextColor(const CQChartsColor &c)
{
  CQChartsUtil::testAndSet(textData_.color, c, [&]() { update(); } );
}

double
CQChartsBoxPlot::
textAlpha() const
{
  return textData_.alpha;
}

void
CQChartsBoxPlot::
setTextAlpha(double a)
{
  CQChartsUtil::testAndSet(textData_.alpha, a, [&]() { update(); } );
}

QColor
CQChartsBoxPlot::
interpTextColor(int i, int n) const
{
  return textColor().interpColor(this, i, n);
}

const QFont &
CQChartsBoxPlot::
textFont() const
{
  return textData_.font;
}

void
CQChartsBoxPlot::
setTextFont(const QFont &f)
{
  CQChartsUtil::testAndSet(textData_.font, f, [&]() { update(); } );
}

//---

void
CQChartsBoxPlot::
addProperties()
{
  CQChartsPlot::addProperties();

  addProperty("columns", this, "xColumn"    , "x"    );
  addProperty("columns", this, "yColumn"    , "y"    );
  addProperty("columns", this, "groupColumn", "group");

  // connect multiple whiskers
  addProperty("", this, "connected", "connected");

  // whisker box
  addProperty("box", this, "whiskerRange", "range" );
  addProperty("box", this, "boxWidth"    , "width" );
  addProperty("box", this, "boxLabels"   , "labels");

  // whisker box stroke
  addProperty("box/stroke", this, "boxStroked" , "visible"   );
  addProperty("box/stroke", this, "borderColor", "color"     );
  addProperty("box/stroke", this, "borderAlpha", "alpha"     );
  addProperty("box/stroke", this, "borderWidth", "width"     );
  addProperty("box/stroke", this, "cornerSize" , "cornerSize");

  // whisker box fill
  addProperty("box/fill", this, "boxFilled" , "visible");
  addProperty("box/fill", this, "boxColor"  , "color"  );
  addProperty("box/fill", this, "boxAlpha"  , "alpha"  );
  addProperty("box/fill", this, "boxPattern", "pattern");

  // whisker line
  addProperty("whisker", this, "whiskerColor"    , "color" );
  addProperty("whisker", this, "whiskerAlpha"    , "alpha" );
  addProperty("whisker", this, "whiskerLineWidth", "width" );
  addProperty("whisker", this, "whiskerExtent"   , "extent");

  // value labels
  addProperty("labels", this, "textVisible", "visible");
  addProperty("labels", this, "textFont"   , "font"   );
  addProperty("labels", this, "textColor"  , "color"  );
  addProperty("labels", this, "textAlpha"  , "alpha"  );
  addProperty("labels", this, "textMargin" , "margin" );

  addProperty("outlier", this, "skipOutliers", "skip");
  addProperty("outlier", this, "symbolSize"  , "size");
}

void
CQChartsBoxPlot::
updateRange(bool apply)
{
  CScopeTimer timer("updateRange");

  //---

  updateWhiskers();

  //---

  dataRange_.reset();

  xAxis_->clearTickLabels();

  int x  = 0;
  int ig = 0;

  for (const auto &groupWhiskers : this->groupWhiskers()) {
    const SetWhiskerMap &setWhiskerMap = groupWhiskers.second;

    int is = 0;

    for (const auto &setWhiskers : setWhiskerMap) {
      bool hidden = (isGrouped() ? isSetHidden(ig) : isSetHidden(is));

      if (! hidden) {
        int setId = setWhiskers.first;

        QString setIdName = this->setIdName(setId);

        xAxis_->setTickLabel(setId, setIdName);

        //---

        const CQChartsBoxPlotWhisker &whisker = setWhiskers.second;

        double min, max;

        if (! isSkipOutliers()) {
          min = whisker.rvalue(0);
          max = whisker.rvalue(whisker.numValues() - 1);
        }
        else {
          min = whisker.min();
          max = whisker.max();
        }

        dataRange_.updateRange(x - 0.5, min);
        dataRange_.updateRange(x + 0.5, max);

        ++x;
      }

      ++is;
    }

    ++ig;
  }

  //---

//xAxis_->setColumn(xColumn());
  yAxis_->setColumn(yColumn());

  QAbstractItemModel *model = this->model().data();

  if (model) {
    bool ok;

    QString xname = modelHeaderString(model, xColumn(), Qt::Horizontal, Qt::DisplayRole, ok);
    QString yname = modelHeaderString(model, yColumn(), Qt::Horizontal, Qt::DisplayRole, ok);

    xAxis_->setLabel(xname);
    yAxis_->setLabel(yname);
  }

  //---

  if (apply)
    applyDataRange();
}

void
CQChartsBoxPlot::
updateWhiskers()
{
  CScopeTimer timer("updateWhiskers");

  //---

  groupWhiskers_.clear();

  //---

  QAbstractItemModel *model = this->model().data();

  if (! model)
    return;

  //---

  // determine group and x data type
  groupType_ = columnValueType(groupColumn());
  xType_     = columnValueType(xColumn    ());

  groupValueInd_.clear();
  xValueInd_    .clear();

  //---

  // process model data
  class BoxPlotVisitor : public ModelVisitor {
   public:
    BoxPlotVisitor(CQChartsBoxPlot *plot) :
     plot_(plot) {
    }

    State visit(QAbstractItemModel *model, const QModelIndex &ind, int row) override {
      plot_->addWhiskerRow(model, ind, row);

      return State::OK;
    }

   private:
    CQChartsBoxPlot *plot_ { nullptr };
  };

  BoxPlotVisitor boxPlotVisitor(this);

  visitModel(boxPlotVisitor);

  //---

  grouped_ = (numGroups() > 1);

  //---

  for (auto &groupWhiskers : groupWhiskers_) {
    SetWhiskerMap &setWhiskerMap = groupWhiskers.second;

    for (auto &setWhiskers : setWhiskerMap)
      setWhiskers.second.init();
  }
}

void
CQChartsBoxPlot::
addWhiskerRow(QAbstractItemModel *model, const QModelIndex &parent, int row)
{
  // get group
  int groupId = -1;

  if (groupColumn().isValid()) {
    // get value set id
    if      (groupType_ == ColumnType::INTEGER) {
      bool ok1;

      int i = modelHierInteger(model, row, groupColumn(), parent, ok1);

      if (ok1)
        groupId = groupValueInd_.calcId(i);
    }
    else if (groupType_ == ColumnType::REAL) {
      bool ok1;

      double r = modelHierReal(model, row, groupColumn(), parent, ok1);

      if (ok1 && ! CQChartsUtil::isNaN(r))
        groupId = groupValueInd_.calcId(r);
    }
    else {
      bool ok1;

      QString s = modelHierString(model, row, groupColumn(), parent, ok1);

      if (ok1)
        groupId = groupValueInd_.calcId(s);
    }
  }

  //---

  // get value set id
  int setId = -1;

  if      (xType_ == ColumnType::INTEGER) {
    bool ok1;

    int i = modelHierInteger(model, row, xColumn(), parent, ok1);

    setId = xValueInd_.calcId(i);
  }
  else if (xType_ == ColumnType::REAL) {
    bool ok1;

    double r = modelHierReal(model, row, xColumn(), parent, ok1);

    if (CQChartsUtil::isNaN(r))
      return;

    setId = xValueInd_.calcId(r);
  }
  else {
    bool ok1;

    QString s = modelHierString(model, row, xColumn(), parent, ok1);

    setId = xValueInd_.calcId(s);
  }

  //---

  // add value to set
  bool ok2;

  double value = modelReal(model, row, yColumn(), parent, ok2);

  if (! ok2) value = row;

  if (CQChartsUtil::isNaN(value))
    return;

  QModelIndex xind  = model->index(row, xColumn().column(), parent);
  QModelIndex xind1 = normalizeIndex(xind);

  CQChartsBoxPlotValue wv(value, xind1);

  auto p = groupWhiskers_.find(groupId);

  if (p == groupWhiskers_.end()) {
    SetWhiskerMap setWhiskerMap;

    p = groupWhiskers_.insert(p, GroupSetWhiskerMap::value_type(groupId, SetWhiskerMap()));
  }

  SetWhiskerMap &setWhiskerMap = (*p).second;

  auto p1 = setWhiskerMap.find(setId);

  if (p1 == setWhiskerMap.end()) {
    CQChartsBoxPlotWhisker whisker;

    whisker.setRange(whiskerRange());

    p1 = setWhiskerMap.insert(p1, SetWhiskerMap::value_type(setId, whisker));
  }

  (*p1).second.addValue(wv);
}

CQChartsGeom::BBox
CQChartsBoxPlot::
annotationBBox() const
{
  CQChartsGeom::BBox bbox;

  for (const auto &plotObj : plotObjs_) {
    CQChartsBoxPlotObj *boxObj = dynamic_cast<CQChartsBoxPlotObj *>(plotObj);

    if (boxObj)
      bbox += boxObj->annotationBBox();
  }

  return bbox;
}

//------

bool
CQChartsBoxPlot::
initObjs()
{
  if (! dataRange_.isSet()) {
    updateRange();

    if (! dataRange_.isSet())
      return false;
  }

  //---

  if (! plotObjs_.empty())
    return false;

  //---

  int ig = 0;
  int ng = numGroups();

  int x = 0;

  for (const auto &groupWhiskers : this->groupWhiskers()) {
    int groupId = groupWhiskers.first;

    const SetWhiskerMap &setWhiskerMap = groupWhiskers.second;

    if (! isConnected()) {
      int is = 0;
      int ns = setWhiskerMap.size();

      for (const auto &setWhiskers : setWhiskerMap) {
        bool hidden = (isGrouped() ? isSetHidden(ig) : isSetHidden(is));

        if (! hidden) {
          int setId = setWhiskers.first;

          const CQChartsBoxPlotWhisker &whisker = setWhiskers.second;

          //----

          CQChartsGeom::BBox rect(x - 0.10, whisker.lower(), x + 0.10, whisker.upper());

          CQChartsBoxPlotObj *boxObj =
            new CQChartsBoxPlotObj(this, rect, setId, whisker, ig, ng, is, ns);

          addPlotObject(boxObj);

          ++x;
        }

        ++is;
      }
    }
    else {
      bool hidden = (isGrouped() ? isSetHidden(ig) : false);

      if (! hidden) {
        CQChartsGeom::BBox rect(dataRange_.xmin(), dataRange_.ymin(),
                                dataRange_.xmax(), dataRange_.ymax());

        CQChartsBoxPlotConnectedObj *connectedObj =
          new CQChartsBoxPlotConnectedObj(this, rect, groupId, ig, ng);

        addPlotObject(connectedObj);
      }
    }

    ++ig;
  }

  //---

  resetKeyItems();

  //---

  return true;
}

void
CQChartsBoxPlot::
addKeyItems(CQChartsPlotKey *key)
{
  int ig = 0;
  int ng = numGroups();

  for (const auto &groupWhiskers : this->groupWhiskers()) {
    int groupId = groupWhiskers.first;

    QString groupName = this->groupIdName(groupId);

    const SetWhiskerMap &setWhiskerMap = groupWhiskers.second;

    int is = 0;
    int ns = setWhiskerMap.size();

    for (const auto &setWhiskers : setWhiskerMap) {
      int setId = setWhiskers.first;

      QString setName = this->setIdName(setId);

      QString name = (isGrouped() ? groupName : setName);

      int i = (isGrouped() ? ig : is);
      int n = (isGrouped() ? ng : ns);

      CQChartsBoxKeyColor *color = new CQChartsBoxKeyColor(this, i, n);
      CQChartsBoxKeyText  *text  = new CQChartsBoxKeyText (this, i, name);

      key->addItem(color, i, 0);
      key->addItem(text , i, 1);

      if (isGrouped())
        break;

      ++is;
    }

    ++ig;
  }

  key->plot()->updateKeyPosition(/*force*/true);
}

bool
CQChartsBoxPlot::
probe(ProbeData &probeData) const
{
  if (! dataRange_.isSet())
    return false;

  if (probeData.x < dataRange_.xmin() + 0.5)
    probeData.x = dataRange_.xmin() + 0.5;

  if (probeData.x > dataRange_.xmax() - 0.5)
    probeData.x = dataRange_.xmax() - 0.5;

  probeData.x = std::round(probeData.x);

  return true;
}

void
CQChartsBoxPlot::
draw(QPainter *painter)
{
  initPlotObjs();

  //---

  drawParts(painter);
}

//------

CQChartsBoxPlotObj::
CQChartsBoxPlotObj(CQChartsBoxPlot *plot, const CQChartsGeom::BBox &rect, int setId,
                   const CQChartsBoxPlotWhisker &whisker, int ig, int ng, int is, int ns) :
 CQChartsPlotObj(plot, rect), plot_(plot), setId_(setId), whisker_(whisker),
 ig_(ig), ng_(ng), is_(is), ns_(ns)
{
}

QString
CQChartsBoxPlotObj::
calcId() const
{
  QString setName = plot_->setIdName(setId_);

  return QString("%1:%2:%3").arg(setName).arg(whisker_.lower()).arg(whisker_.upper());
}

void
CQChartsBoxPlotObj::
getSelectIndices(Indices &inds) const
{
  addColumnSelectIndex(inds, plot_->xColumn());
  addColumnSelectIndex(inds, plot_->yColumn());
}

void
CQChartsBoxPlotObj::
addColumnSelectIndex(Indices &inds, const CQChartsColumn &column) const
{
  if (column.isValid()) {
    for (auto value : whisker_.values()) {
      addSelectIndex(inds, value.ind.row(), column, value.ind.parent());
    }
  }
}

void
CQChartsBoxPlotObj::
draw(QPainter *painter, const CQChartsPlot::Layer &)
{
  QFontMetricsF fm(plot_->textFont());

  double yf = (fm.ascent() - fm.descent())/2.0;

  //---

  double x = rect_.getXMid();

  double wd1 = plot_->whiskerExtent()/2.0;
  double wd2 = plot_->lengthPlotWidth(plot_->boxWidth())/2;

  double px1, py1, px2, py2, px3, py3, px4, py4, px5, py5;

  plot_->windowToPixel(x - wd1, whisker_.min   (), px1, py1);
  plot_->windowToPixel(x - wd2, whisker_.lower (), px2, py2);
  plot_->windowToPixel(x      , whisker_.median(), px3, py3);
  plot_->windowToPixel(x + wd2, whisker_.upper (), px4, py4);
  plot_->windowToPixel(x + wd1, whisker_.max   (), px5, py5);

  //---

  bool hasRange = (fabs(whisker_.max() - whisker_.min()) > 1E-6);

  //---

  QColor whiskerColor = plot_->interpWhiskerColor(0, 1);

  whiskerColor.setAlphaF(plot_->whiskerAlpha());

  double whiskerLineWidth = plot_->lengthPixelWidth(plot_->whiskerLineWidth());

  //---

  // draw extent line
  painter->setPen(QPen(whiskerColor, whiskerLineWidth, Qt::SolidLine));

  painter->drawLine(QPointF(px3, py1), QPointF(px3, py5));

  //---

  // draw lower/upper horizontal lines
  painter->setPen(QPen(whiskerColor, whiskerLineWidth, Qt::SolidLine));

  painter->drawLine(QPointF(px1, py1), QPointF(px5, py1));
  painter->drawLine(QPointF(px1, py5), QPointF(px5, py5));

  //---

  // draw box
  QRectF rect(px2, py2, px4 - px2, py4 - py2);

  // set fill and stroke
  QBrush brush;

  if (plot_->isBoxFilled()) {
    QColor boxColor = (plot_->isGrouped() ? plot_->interpBoxColor(ig_, ng_) :
                                            plot_->interpBoxColor(is_, ns_));

    boxColor.setAlphaF(plot_->boxAlpha());

    brush.setColor(boxColor);
    brush.setStyle(CQChartsFillPattern::toStyle(
     (CQChartsFillPattern::Type) plot_->boxPattern()));
  }
  else {
    brush.setStyle(Qt::NoBrush);
  }

  QPen pen;

  if (plot_->isBorderStroked()) {
    QColor borderColor = (plot_->isGrouped() ? plot_->interpBorderColor(ig_, ng_) :
                                               plot_->interpBorderColor(is_, ns_));

    borderColor.setAlphaF(plot_->borderAlpha());

    double bw = plot_->lengthPixelWidth(plot_->borderWidth());

    pen.setColor (borderColor);
    pen.setWidthF(bw);
  }
  else {
    pen.setStyle(Qt::NoPen);
  }

  plot_->updateObjPenBrushState(this, pen, brush);

  painter->setBrush(brush);
  painter->setPen  (pen);

  double cxs = plot_->lengthPixelWidth (plot_->cornerSize());
  double cys = plot_->lengthPixelHeight(plot_->cornerSize());

  CQChartsRoundedPolygon::draw(painter, rect, cxs, cys);

  //---

  // draw median line
  painter->setPen(QPen(whiskerColor, whiskerLineWidth, Qt::SolidLine));

  painter->drawLine(QPointF(px2, py3), QPointF(px4, py3));

  //---

  // draw labels
  if (plot_->isTextVisible()) {
    double margin = plot_->textMargin();

    painter->setFont(plot_->textFont());

    QColor tc = plot_->interpTextColor(0, 1);

    tc.setAlphaF(plot_->textAlpha());

    painter->setPen(tc);

    if (hasRange) {
      QString strl = QString("%1").arg(whisker_.min   ());
      QString lstr = QString("%1").arg(whisker_.lower ());
      QString mstr = QString("%1").arg(whisker_.median());
      QString ustr = QString("%1").arg(whisker_.upper ());
      QString strh = QString("%1").arg(whisker_.max   ());

      painter->drawText(QPointF(px5 + margin                 , py1 + yf), strl);
      painter->drawText(QPointF(px2 - margin - fm.width(lstr), py2 + yf), lstr);
      painter->drawText(QPointF(px4 + margin                 , py3 + yf), mstr);
      painter->drawText(QPointF(px2 - margin - fm.width(ustr), py4 + yf), ustr);
      painter->drawText(QPointF(px5 + margin                 , py5 + yf), strh);
    }
    else {
      QString strl = QString("%1").arg(whisker_.min());

      painter->drawText(QPointF(px5 + margin, py1 + yf), strl);
    }
  }

  //---

  // draw outlier symbols
  if (! plot_->isSkipOutliers()) {
    painter->setPen(QPen(whiskerColor, whiskerLineWidth, Qt::SolidLine));

    painter->setBrush(brush);
    painter->setPen  (pen);

    double symbolSize = plot_->symbolSize();

    for (auto o : whisker_.outliers()) {
      double px1, py1;

      plot_->windowToPixel(x, whisker_.rvalue(o), px1, py1);

      QRectF rect(px1 - symbolSize, py1 - symbolSize, 2*symbolSize, 2*symbolSize);

      painter->drawEllipse(rect);
    }
  }
}

CQChartsGeom::BBox
CQChartsBoxPlotObj::
annotationBBox() const
{
  double x = rect_.getXMid();

  double wd1 = plot_->whiskerExtent()/2.0;
  double wd2 = plot_->lengthPlotWidth(plot_->boxWidth())/2;

  double px1, py1, px2, py2, px3, py3, px4, py4, px5, py5;

  plot_->windowToPixel(x - wd1, whisker_.min   (), px1, py1);
  plot_->windowToPixel(x - wd2, whisker_.lower (), px2, py2);
  plot_->windowToPixel(x      , whisker_.median(), px3, py3);
  plot_->windowToPixel(x + wd2, whisker_.upper (), px4, py4);
  plot_->windowToPixel(x + wd1, whisker_.max   (), px5, py5);

  //---

  bool hasRange = (fabs(whisker_.max() - whisker_.min()) > 1E-6);

  //---

  CQChartsGeom::BBox pbbox;

  if (plot_->isTextVisible()) {
    double margin = plot_->textMargin();

    QFontMetricsF fm(plot_->textFont());

    double fa = fm.ascent ();
    double fd = fm.descent();
    double yf = (fa - fd)/2.0;

    if (hasRange) {
      QString strl = QString("%1").arg(whisker_.min   ());
      QString lstr = QString("%1").arg(whisker_.lower ());
      QString mstr = QString("%1").arg(whisker_.median());
      QString ustr = QString("%1").arg(whisker_.upper ());
      QString strh = QString("%1").arg(whisker_.max   ());

      pbbox += CQChartsGeom::Point(px5 + margin + fm.width(strl), py1 + yf + fd);
      pbbox += CQChartsGeom::Point(px2 - margin - fm.width(lstr), py2 + yf + fd);
      pbbox += CQChartsGeom::Point(px4 + margin + fm.width(mstr), py3 + yf);
      pbbox += CQChartsGeom::Point(px2 - margin - fm.width(ustr), py4 + yf - fa);
      pbbox += CQChartsGeom::Point(px5 + margin + fm.width(strh), py5 + yf - fa);
    }
    else {
      QString strl = QString("%1").arg(whisker_.min   ());

      pbbox += CQChartsGeom::Point(px5 + margin + fm.width(strl), py1 + yf + fd);
    }
  }
  else {
    pbbox += CQChartsGeom::Point(px5, py1);
    pbbox += CQChartsGeom::Point(px2, py2);
    pbbox += CQChartsGeom::Point(px4, py3);
    pbbox += CQChartsGeom::Point(px2, py4);
    pbbox += CQChartsGeom::Point(px5, py5);
  }

  //---

  CQChartsGeom::BBox bbox;

  plot_->pixelToWindow(pbbox, bbox);

  return bbox;
}

//------

CQChartsBoxPlotConnectedObj::
CQChartsBoxPlotConnectedObj(CQChartsBoxPlot *plot, const CQChartsGeom::BBox &rect,
                            int groupId, int i, int n) :
 CQChartsPlotObj(plot, rect), plot_(plot), groupId_(groupId), i_(i), n_(n)
{
  initPolygon();
}

QString
CQChartsBoxPlotConnectedObj::
calcId() const
{
  QString groupName = plot_->groupIdName(groupId_);

  return QString("%1").arg(groupName);
}

void
CQChartsBoxPlotConnectedObj::
initPolygon()
{
  QPolygonF maxPoly, minPoly;

  int i = 0;

  for (const auto &groupWhiskers : plot_->groupWhiskers()) {
    const CQChartsBoxPlot::SetWhiskerMap &setWhiskerMap = groupWhiskers.second;

    if (i == i_) {
      for (const auto &setWhiskers : setWhiskerMap) {
        double                        pos     = setWhiskers.first;
        const CQChartsBoxPlotWhisker &whisker = setWhiskers.second;

        double min    = whisker.min   ();
        double max    = whisker.max   ();
        double median = whisker.median();

        line_ << CQChartsUtil::toQPoint(CQChartsGeom::Point(pos, median));

        maxPoly << CQChartsUtil::toQPoint(CQChartsGeom::Point(pos, max));
        minPoly << CQChartsUtil::toQPoint(CQChartsGeom::Point(pos, min));
      }

      break;
    }

    ++i;
  }

  int np = maxPoly.count();

  for (int i = 0; i < np; ++i)
    poly_ << maxPoly.at(i);

  for (int i = 0; i < np; ++i)
    poly_ << minPoly.at(np - 1 - i);
}

bool
CQChartsBoxPlotConnectedObj::
inside(const CQChartsGeom::Point &p) const
{
  return poly_.containsPoint(CQChartsUtil::toQPoint(p), Qt::OddEvenFill);
}

void
CQChartsBoxPlotConnectedObj::
draw(QPainter *painter, const CQChartsPlot::Layer &)
{
  // draw range polygon
  int np = poly_.count();

  if (np) {
    QPainterPath path;

    path.moveTo(plot_->windowToPixel(poly_.at(0)));

    for (int i = 1; i < np; ++i)
      path.lineTo(plot_->windowToPixel(poly_.at(i)));

    path.closeSubpath();

    QColor fillColor = plot_->interpBoxColor(i_, n_);

    QBrush pbrush;

    if (plot_->isBoxFilled()) {
      fillColor.setAlphaF(plot_->boxAlpha());

      pbrush.setColor(fillColor);
      pbrush.setStyle(CQChartsFillPattern::toStyle(
       (CQChartsFillPattern::Type) plot_->boxPattern()));
    }

    QPen ppen;

    if (plot_->isBorderStroked()) {
      QColor borderColor = plot_->interpBorderColor(i_, n_);

      borderColor.setAlphaF(plot_->borderAlpha());

      double bw = plot_->lengthPixelWidth(plot_->borderWidth());

      ppen.setColor (borderColor);
      ppen.setWidthF(bw);
    }
    else {
      ppen.setStyle(Qt::NoPen);
    }

    plot_->updateObjPenBrushState(this, ppen, pbrush);

    painter->setPen  (ppen);
    painter->setBrush(pbrush);

    painter->drawPath(path);
  }

  //---

  // draw median line
  QPolygonF line;

  for (int i = 0; i < line_.count(); ++i)
    line << plot_->windowToPixel(line_.at(i));

  QPen lpen;

  QColor lineColor = plot_->interpBorderColor(i_, n_);

  lineColor.setAlphaF(plot_->borderAlpha());

  double bw = plot_->lengthPixelWidth(plot_->borderWidth());

  lpen.setColor (lineColor);
  lpen.setWidthF(bw);

  QBrush lbrush;

  plot_->updateObjPenBrushState(this, lpen, lbrush);

  painter->setPen(lpen);

  painter->drawPolyline(line);
}

//------

CQChartsBoxKeyColor::
CQChartsBoxKeyColor(CQChartsBoxPlot *plot, int i, int n) :
 CQChartsKeyColorBox(plot, i, n)
{
}

bool
CQChartsBoxKeyColor::
selectPress(const CQChartsGeom::Point &)
{
  CQChartsBoxPlot *plot = qobject_cast<CQChartsBoxPlot *>(plot_);

  plot->setSetHidden(i_, ! plot->isSetHidden(i_));

  plot->updateObjs();

  return true;
}

QBrush
CQChartsBoxKeyColor::
fillBrush() const
{
  QColor c = CQChartsKeyColorBox::fillBrush().color();

  CQChartsBoxPlot *plot = qobject_cast<CQChartsBoxPlot *>(plot_);

  if (plot->isSetHidden(i_))
    c = CQChartsUtil::blendColors(c, key_->interpBgColor(), 0.5);

  return c;
}

//------

CQChartsBoxKeyText::
CQChartsBoxKeyText(CQChartsBoxPlot *plot, int i, const QString &text) :
 CQChartsKeyText(plot, text), i_(i)
{
}

QColor
CQChartsBoxKeyText::
interpTextColor(int i, int n) const
{
  CQChartsBoxPlot *plot = qobject_cast<CQChartsBoxPlot *>(plot_);

  QColor c = CQChartsKeyText::interpTextColor(i, n);

  if (plot->isSetHidden(i_))
    c = CQChartsUtil::blendColors(c, key_->interpBgColor(), 0.5);

  return c;
}
