#include <CQChartsLineDataEdit.h>

#include <CQChartsColorEdit.h>
#include <CQChartsAlphaEdit.h>
#include <CQChartsLengthEdit.h>
#include <CQChartsLineDashEdit.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQCharts.h>

#include <CQPropertyView.h>
#include <CQWidgetMenu.h>
#include <CQGroupBox.h>

#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>

CQChartsLineDataLineEdit::
CQChartsLineDataLineEdit(QWidget *parent) :
 CQChartsLineEditBase(parent)
{
  setObjectName("lineDataLineEdit");

  //---

  menuEdit_ = dataEdit_ = new CQChartsLineDataEdit;

  menu_->setWidget(dataEdit_);

  connect(dataEdit_, SIGNAL(lineDataChanged()), this, SLOT(menuEditChanged()));

  //---

  lineDataToWidgets();
}

const CQChartsLineData &
CQChartsLineDataLineEdit::
lineData() const
{
  return dataEdit_->data();
}

void
CQChartsLineDataLineEdit::
setLineData(const CQChartsLineData &lineData)
{
  updateLineData(lineData, /*updateText*/ true);
}

void
CQChartsLineDataLineEdit::
updateLineData(const CQChartsLineData &lineData, bool updateText)
{
  connectSlots(false);

  dataEdit_->setData(lineData);

  if (updateText)
    lineDataToWidgets();

  connectSlots(true);

  emit lineDataChanged();
}

void
CQChartsLineDataLineEdit::
textChanged()
{
  CQChartsLineData lineData(edit_->text());

  if (! lineData.isValid())
    return;

  updateLineData(lineData, /*updateText*/ false);
}

void
CQChartsLineDataLineEdit::
lineDataToWidgets()
{
  connectSlots(false);

  if (lineData().isValid())
    edit_->setText(lineData().toString());
  else
    edit_->setText("");

  setToolTip(lineData().toString());

  connectSlots(true);
}

void
CQChartsLineDataLineEdit::
menuEditChanged()
{
  lineDataToWidgets();

  emit lineDataChanged();
}

void
CQChartsLineDataLineEdit::
connectSlots(bool b)
{
  CQChartsLineEditBase::connectSlots(b);

  if (b)
    connect(dataEdit_, SIGNAL(lineDataChanged()), this, SLOT(menuEditChanged()));
  else
    disconnect(dataEdit_, SIGNAL(lineDataChanged()), this, SLOT(menuEditChanged()));
}

void
CQChartsLineDataLineEdit::
drawPreview(QPainter *painter, const QRect &rect)
{
  CQChartsLineDataEditPreview::draw(painter, lineData(), rect, plot(), view());
}

//------

CQPropertyViewEditorFactory *
CQChartsLineDataPropertyViewType::
getEditor() const
{
  return new CQChartsLineDataPropertyViewEditor;
}

void
CQChartsLineDataPropertyViewType::
drawPreview(QPainter *painter, const QRect &rect, const QVariant &value,
            CQChartsPlot *plot, CQChartsView *view)
{
  CQChartsLineData data = value.value<CQChartsLineData>();

  CQChartsLineDataEditPreview::draw(painter, data, rect, plot, view);
}

QString
CQChartsLineDataPropertyViewType::
tip(const QVariant &value) const
{
  QString str = value.value<CQChartsLineData>().toString();

  return str;
}

//------

CQChartsLineEditBase *
CQChartsLineDataPropertyViewEditor::
createPropertyEdit(QWidget *parent)
{
  return new CQChartsLineDataLineEdit(parent);
}

void
CQChartsLineDataPropertyViewEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQChartsLineDataLineEdit *edit = qobject_cast<CQChartsLineDataLineEdit *>(w);
  assert(edit);

  QObject::connect(edit, SIGNAL(lineDataChanged()), obj, method);
}

QVariant
CQChartsLineDataPropertyViewEditor::
getValue(QWidget *w)
{
  CQChartsLineDataLineEdit *edit = qobject_cast<CQChartsLineDataLineEdit *>(w);
  assert(edit);

  return QVariant::fromValue(edit->lineData());
}

void
CQChartsLineDataPropertyViewEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQChartsLineDataLineEdit *edit = qobject_cast<CQChartsLineDataLineEdit *>(w);
  assert(edit);

  CQChartsLineData data = var.value<CQChartsLineData>();

  edit->setLineData(data);
}

//------

CQChartsLineDataEdit::
CQChartsLineDataEdit(QWidget *parent) :
 CQChartsEditBase(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  groupBox_ = new CQGroupBox;

  groupBox_->setCheckable(true);
  groupBox_->setChecked(false);
  groupBox_->setTitle("Visible");

  connect(groupBox_, SIGNAL(clicked(bool)), this, SLOT(widgetsToData()));

  layout->addWidget(groupBox_);

  //---

  QGridLayout *groupLayout = new QGridLayout(groupBox_);

  // color
  QLabel *colorLabel = new QLabel("Color");

  colorEdit_ = new CQChartsColorLineEdit;

  connect(colorEdit_, SIGNAL(colorChanged()), this, SLOT(widgetsToData()));

  groupLayout->addWidget(colorLabel, 0, 0);
  groupLayout->addWidget(colorEdit_, 0, 1);

  // alpha
  QLabel *alphaLabel = new QLabel("Alpha");

  alphaEdit_ = new CQChartsAlphaEdit;

  connect(alphaEdit_, SIGNAL(valueChanged(double)), this, SLOT(widgetsToData()));

  groupLayout->addWidget(alphaLabel, 1, 0);
  groupLayout->addWidget(alphaEdit_, 1, 1);

  // width
  QLabel *widthLabel = new QLabel("Width");

  widthEdit_ = new CQChartsLengthEdit;

  connect(widthEdit_, SIGNAL(lengthChanged()), this, SLOT(widgetsToData()));

  groupLayout->addWidget(widthLabel, 2, 0);
  groupLayout->addWidget(widthEdit_, 2, 1);

  // dash
  QLabel *dashLabel = new QLabel("Dash");

  dashEdit_ = new CQChartsLineDashEdit;

  connect(dashEdit_, SIGNAL(valueChanged(const CQChartsLineDash &)), this, SLOT(widgetsToData()));

  groupLayout->addWidget(dashLabel, 3, 0);
  groupLayout->addWidget(dashEdit_, 3, 1);

  //---

  preview_ = new CQChartsLineDataEditPreview(this);

  groupLayout->addWidget(preview_, 4, 1);

  //---

  groupLayout->setRowStretch(5, 1);

  //---

  layout->addStretch(1);

  dataToWidgets();
}

void
CQChartsLineDataEdit::
setData(const CQChartsLineData &d)
{
  data_ = d;

  dataToWidgets();
}

void
CQChartsLineDataEdit::
setTitle(const QString &title)
{
  groupBox_->setTitle(title);
}

void
CQChartsLineDataEdit::
setPreview(bool b)
{
  preview_->setVisible(b);
}

void
CQChartsLineDataEdit::
dataToWidgets()
{
  disconnect(groupBox_, SIGNAL(clicked(bool)), this, SLOT(widgetsToData()));
  disconnect(colorEdit_, SIGNAL(colorChanged()), this, SLOT(widgetsToData()));
  disconnect(alphaEdit_, SIGNAL(valueChanged(double)), this, SLOT(widgetsToData()));
  disconnect(widthEdit_, SIGNAL(lengthChanged()), this, SLOT(widgetsToData()));
  disconnect(dashEdit_, SIGNAL(valueChanged(const CQChartsLineDash &)),
             this, SLOT(widgetsToData()));

  groupBox_ ->setChecked (data_.isVisible());
  colorEdit_->setColor   (data_.color());
  alphaEdit_->setValue   (data_.alpha());
  widthEdit_->setLength  (data_.width());
  dashEdit_ ->setLineDash(data_.dash());

  preview_->update();

  connect(groupBox_, SIGNAL(clicked(bool)), this, SLOT(widgetsToData()));
  connect(colorEdit_, SIGNAL(colorChanged()), this, SLOT(widgetsToData()));
  connect(alphaEdit_, SIGNAL(valueChanged(double)), this, SLOT(widgetsToData()));
  connect(widthEdit_, SIGNAL(lengthChanged()), this, SLOT(widgetsToData()));
  connect(dashEdit_, SIGNAL(valueChanged(const CQChartsLineDash &)),
          this, SLOT(widgetsToData()));
}

void
CQChartsLineDataEdit::
widgetsToData()
{
  data_.setVisible(groupBox_ ->isChecked());
  data_.setColor  (colorEdit_->color());
  data_.setAlpha  (alphaEdit_->value());
  data_.setWidth  (widthEdit_->length());
  data_.setDash   (dashEdit_ ->getLineDash());

  preview_->update();

  emit lineDataChanged();
}

//------

CQChartsLineDataEditPreview::
CQChartsLineDataEditPreview(CQChartsLineDataEdit *edit) :
 CQChartsEditPreview(edit), edit_(edit)
{
}

void
CQChartsLineDataEditPreview::
draw(QPainter *painter)
{
  const CQChartsLineData &data = edit_->data();

  draw(painter, data, rect(), edit_->plot(), edit_->view());
}

void
CQChartsLineDataEditPreview::
draw(QPainter *painter, const CQChartsLineData &data, const QRect &rect,
     CQChartsPlot *plot, CQChartsView *view)
{
  // set pen
  QColor pc = interpColor(plot, view, data.color());

  double width = CQChartsUtil::limitLineWidth(data.width().value());

  QPen pen;

  CQChartsUtil::setPen(pen, data.isVisible(), pc, data.alpha(), width, data.dash());

  painter->setPen(pen);

  //---

  // draw line
  QPoint p1(rect.left (), rect.center().y());
  QPoint p2(rect.right(), rect.center().y());

  painter->drawLine(p1, p2);
}
