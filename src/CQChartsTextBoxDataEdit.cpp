#include <CQChartsTextBoxDataEdit.h>

#include <CQChartsTextDataEdit.h>
#include <CQChartsBoxDataEdit.h>
#include <CQChartsRoundedPolygon.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQCharts.h>

#include <CQPropertyView.h>
#include <CQWidgetMenu.h>
#include <CQUtil.h>

#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>

CQChartsTextBoxDataLineEdit::
CQChartsTextBoxDataLineEdit(QWidget *parent) :
 CQChartsLineEditBase(parent)
{
  setObjectName("textBoxDataLineEdit");

  //---

  menuEdit_ = dataEdit_ = new CQChartsTextBoxDataEdit;

  menu_->setWidget(dataEdit_);

  connect(dataEdit_, SIGNAL(textBoxDataChanged()), this, SLOT(menuEditChanged()));

  //---

  textBoxDataToWidgets();
}

const CQChartsTextBoxData &
CQChartsTextBoxDataLineEdit::
textBoxData() const
{
  return dataEdit_->data();
}

void
CQChartsTextBoxDataLineEdit::
setTextBoxData(const CQChartsTextBoxData &textBoxData)
{
  updateTextBoxData(textBoxData, /*updateText*/ true);
}

void
CQChartsTextBoxDataLineEdit::
updateTextBoxData(const CQChartsTextBoxData &textBoxData, bool updateText)
{
  connectSlots(false);

  dataEdit_->setData(textBoxData);

  if (updateText)
    textBoxDataToWidgets();

  connectSlots(true);

  emit textBoxDataChanged();
}

void
CQChartsTextBoxDataLineEdit::
textChanged()
{
  CQChartsTextBoxData textBoxData(edit_->text());

  if (! textBoxData.isValid())
    return;

  updateTextBoxData(textBoxData, /*updateText*/ false);
}

void
CQChartsTextBoxDataLineEdit::
textBoxDataToWidgets()
{
  connectSlots(false);

  if (textBoxData().isValid())
    edit_->setText(textBoxData().toString());
  else
    edit_->setText("");

  setToolTip(textBoxData().toString());

  connectSlots(true);
}

void
CQChartsTextBoxDataLineEdit::
menuEditChanged()
{
  textBoxDataToWidgets();

  emit textBoxDataChanged();
}

void
CQChartsTextBoxDataLineEdit::
connectSlots(bool b)
{
  CQChartsLineEditBase::connectSlots(b);

  if (b)
    connect(dataEdit_, SIGNAL(textBoxDataChanged()), this, SLOT(menuEditChanged()));
  else
    disconnect(dataEdit_, SIGNAL(textBoxDataChanged()), this, SLOT(menuEditChanged()));
}

void
CQChartsTextBoxDataLineEdit::
drawPreview(QPainter *painter, const QRect &rect)
{
  CQChartsTextBoxDataEditPreview::draw(painter, textBoxData(), rect, plot(), view());
}

//------

CQPropertyViewEditorFactory *
CQChartsTextBoxDataPropertyViewType::
getEditor() const
{
  return new CQChartsTextBoxDataPropertyViewEditor;
}

void
CQChartsTextBoxDataPropertyViewType::
drawPreview(QPainter *painter, const QRect &rect, const QVariant &value,
            CQChartsPlot *plot, CQChartsView *view)
{
  CQChartsTextBoxData data = value.value<CQChartsTextBoxData>();

  CQChartsTextBoxDataEditPreview::draw(painter, data, rect, plot, view);
}

QString
CQChartsTextBoxDataPropertyViewType::
tip(const QVariant &value) const
{
  QString str = value.value<CQChartsTextBoxData>().toString();

  return str;
}

//------

CQChartsLineEditBase *
CQChartsTextBoxDataPropertyViewEditor::
createPropertyEdit(QWidget *parent)
{
  return new CQChartsTextBoxDataLineEdit(parent);
}

void
CQChartsTextBoxDataPropertyViewEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQChartsTextBoxDataLineEdit *edit = qobject_cast<CQChartsTextBoxDataLineEdit *>(w);
  assert(edit);

  QObject::connect(edit, SIGNAL(textBoxDataChanged()), obj, method);
}

QVariant
CQChartsTextBoxDataPropertyViewEditor::
getValue(QWidget *w)
{
  CQChartsTextBoxDataLineEdit *edit = qobject_cast<CQChartsTextBoxDataLineEdit *>(w); assert(edit);

  return QVariant::fromValue(edit->textBoxData());
}

void
CQChartsTextBoxDataPropertyViewEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQChartsTextBoxDataLineEdit *edit = qobject_cast<CQChartsTextBoxDataLineEdit *>(w);
  assert(edit);

  CQChartsTextBoxData data = var.value<CQChartsTextBoxData>();

  edit->setTextBoxData(data);
}

//------

CQChartsTextBoxDataEdit::
CQChartsTextBoxDataEdit(QWidget *parent, bool tabbed) :
 CQChartsEditBase(parent), tabbed_(tabbed)
{
  setObjectName("textBoxDataEdit");

  QGridLayout *layout = new QGridLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  int row = 0;

  //---

  if (tabbed_) {
    QTabWidget *tab = CQUtil::makeWidget<QTabWidget>("tab");

    layout->addWidget(tab, row, 0, 1, 2); ++row;

    //----

    // text frame
    QFrame *textFrame = CQUtil::makeWidget<QFrame>("textFrame");

    QVBoxLayout *textFrameLayout = new QVBoxLayout(textFrame);
    textFrameLayout->setMargin(0); textFrameLayout->setSpacing(2);

    tab->addTab(textFrame, "Text");

    //--

    // text
    textEdit_ = new CQChartsTextDataEdit;

    textEdit_->setPreview(false);

    textFrameLayout->addWidget(textEdit_);

    //----

    // box frame
    QFrame *boxFrame = CQUtil::makeWidget<QFrame>("boxFrame");

    QVBoxLayout *boxFrameLayout = new QVBoxLayout(boxFrame);
    boxFrameLayout->setMargin(0); boxFrameLayout->setSpacing(2);

    tab->addTab(boxFrame, "Box");

    //--

    // box
    boxEdit_ = new CQChartsBoxDataEdit;

    boxEdit_->setPreview(false);

    boxFrameLayout->addWidget(boxEdit_);
  }
  else {
    //--

    // text
    textEdit_ = new CQChartsTextDataEdit;

    textEdit_->setTitle("Text");
    textEdit_->setPreview(false);

    layout->addWidget(textEdit_, row, 0, 1, 2); ++row;

    //--

    // box
    boxEdit_ = new CQChartsBoxDataEdit;

    boxEdit_->setTitle("Box");
    boxEdit_->setPreview(false);

    layout->addWidget(boxEdit_, row, 0, 1, 2); ++row;
  }

  connect(textEdit_, SIGNAL(textDataChanged()), this, SLOT(widgetsToData()));
  connect(boxEdit_ , SIGNAL(boxDataChanged()) , this, SLOT(widgetsToData()));

  //---

  // preview
  preview_ = new CQChartsTextBoxDataEditPreview(this);

  layout->addWidget(preview_, row, 1);

  //---

  layout->setRowStretch(row, 1);

  //---

  dataToWidgets();
}

void
CQChartsTextBoxDataEdit::
setData(const CQChartsTextBoxData &d)
{
  data_ = d;

  dataToWidgets();
}

void
CQChartsTextBoxDataEdit::
setPlot(CQChartsPlot *plot)
{
  CQChartsEditBase::setPlot(plot);

  textEdit_->setPlot(plot);
  boxEdit_ ->setPlot(plot);
}

void
CQChartsTextBoxDataEdit::
setView(CQChartsView *view)
{
  CQChartsEditBase::setView(view);

  textEdit_->setView(view);
  boxEdit_ ->setView(view);
}

void
CQChartsTextBoxDataEdit::
setTitle(const QString &)
{
  if (! tabbed_) {
    textEdit_->setTitle("Text");
    boxEdit_ ->setTitle("Box" );
  }
}

void
CQChartsTextBoxDataEdit::
setPreview(bool b)
{
  preview_->setVisible(b);

  textEdit_->setPreview(b);
  boxEdit_ ->setPreview(b);
}

void
CQChartsTextBoxDataEdit::
dataToWidgets()
{
  disconnect(textEdit_, SIGNAL(textDataChanged()), this, SLOT(widgetsToData()));
  disconnect(boxEdit_, SIGNAL(boxDataChanged()), this, SLOT(widgetsToData()));

  textEdit_->setData(data_.text());
  boxEdit_ ->setData(data_.box());

  preview_->update();

  connect(textEdit_, SIGNAL(textDataChanged()), this, SLOT(widgetsToData()));
  connect(boxEdit_, SIGNAL(boxDataChanged()), this, SLOT(widgetsToData()));

}

void
CQChartsTextBoxDataEdit::
widgetsToData()
{
  data_.setText(textEdit_->data());
  data_.setBox (boxEdit_ ->data());

  preview_->update();

  emit textBoxDataChanged();
}

//------

CQChartsTextBoxDataEditPreview::
CQChartsTextBoxDataEditPreview(CQChartsTextBoxDataEdit *edit) :
 CQChartsEditPreview(edit), edit_(edit)
{
}

void
CQChartsTextBoxDataEditPreview::
draw(QPainter *painter)
{
  const CQChartsTextBoxData &data = edit_->data();

  draw(painter, data, rect(), edit_->plot(), edit_->view());
}

void
CQChartsTextBoxDataEditPreview::
draw(QPainter *painter, const CQChartsTextBoxData &data, const QRect &rect,
     CQChartsPlot *plot, CQChartsView *view)
{
  const CQChartsBoxData &box = data.box();

  const CQChartsShapeData &shape = box.shape();

  //---

  // set pen
  QColor pc = interpColor(plot, view, shape.border().color());

  QPen pen;

  double width = CQChartsUtil::limitLineWidth(shape.border().width().value());

  CQChartsUtil::setPen(pen, shape.border().isVisible(), pc,
                       shape.border().alpha(), width, shape.border().dash());

  painter->setPen(pen);

  //---

  // set brush
  QColor fc = interpColor(plot, view, shape.background().color());

  QBrush brush;

  CQChartsUtil::setBrush(brush, shape.background().isVisible(), fc,
                         shape.background().alpha(), shape.background().pattern());

  painter->setBrush(brush);

  //---

  // draw text box
  double cxs = shape.border().cornerSize().value();
  double cys = shape.border().cornerSize().value();

  CQChartsRoundedPolygon::draw(painter, rect, cxs, cys);
}