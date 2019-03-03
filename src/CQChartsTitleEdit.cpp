#include <CQChartsTitleEdit.h>
#include <CQChartsTitle.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQChartsTitleLocationEdit.h>
#include <CQChartsPositionEdit.h>
#include <CQChartsRectEdit.h>
#include <CQChartsTextDataEdit.h>
#include <CQCharts.h>
#include <CQChartsWidgetUtil.h>

#include <CQGroupBox.h>
#include <CQCheckBox.h>
#include <CQUtil.h>

#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>

CQChartsEditTitleDlg::
CQChartsEditTitleDlg(CQChartsTitle *title) :
 QDialog(), title_(title)
{
  setWindowTitle(QString("Edit Plot Title (%1)").arg(title->plot()->id()));

  //---

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  //---

  edit_ = new CQChartsTitleEdit(nullptr, title_);

  layout->addWidget(edit_);

  //---

  CQChartsDialogButtons *buttons = new CQChartsDialogButtons;

  buttons->connect(this, SLOT(okSlot()), SLOT(applySlot()), SLOT(cancelSlot()));

  layout->addWidget(buttons);
}

void
CQChartsEditTitleDlg::
okSlot()
{
  if (applySlot())
    cancelSlot();
}

bool
CQChartsEditTitleDlg::
applySlot()
{
  edit_->applyData();

  return true;
}

void
CQChartsEditTitleDlg::
cancelSlot()
{
  close();
}

//------

CQChartsTitleEdit::
CQChartsTitleEdit(QWidget *parent, CQChartsTitle *title) :
 QFrame(parent), title_(title)
{
  setObjectName("titleEdit");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  //---

  data_.visible    = title->isVisible();
  data_.location   = title->location();
  data_.position   = title->absPosition();
  data_.rect       = title->absRect();
  data_.insidePlot = title->isInsidePlot();
  data_.textData   = title->textData();

  //---

  // visible
  groupBox_ = CQUtil::makeWidget<CQGroupBox>("groupBox");

  groupBox_->setCheckable(true);
  groupBox_->setChecked(data_.visible);
  groupBox_->setTitle("Visible");

  connect(groupBox_, SIGNAL(clicked(bool)), this, SLOT(widgetsToData()));

  layout->addWidget(groupBox_);

  //---

  QGridLayout *groupLayout = new QGridLayout(groupBox_);
  groupLayout->setMargin(2); groupLayout->setSpacing(2);

  int row = 0;

  //--

  // location
  locationEdit_ = CQUtil::makeWidget<CQChartsTitleLocationEdit>("location");

  locationEdit_->setTitleLocation(data_.location);

  CQChartsWidgetUtil::addGridLabelWidget(groupLayout, "Location", locationEdit_, row);

  connect(locationEdit_, SIGNAL(titleLocationChanged()), this, SLOT(widgetsToData()));

  //--

  // position
  positionEdit_ = CQUtil::makeWidget<CQChartsPositionEdit>("positionEdit");

  positionEdit_->setPosition(data_.position);

  connect(positionEdit_, SIGNAL(positionChanged()), this, SLOT(widgetsToData()));

  CQChartsWidgetUtil::addGridLabelWidget(groupLayout, "Position", positionEdit_, row);

  //--

  // rect
  rectEdit_ = CQUtil::makeWidget<CQChartsRectEdit>("rectEdit");

  rectEdit_->setRect(data_.rect);

  connect(rectEdit_, SIGNAL(rectChanged()), this, SLOT(widgetsToData()));

  CQChartsWidgetUtil::addGridLabelWidget(groupLayout, "Rect", rectEdit_, row);

  //--

  // inside plot
  insideEdit_ = CQUtil::makeWidget<CQCheckBox>("insideEdit");

  insideEdit_->setChecked(data_.insidePlot);

  connect(insideEdit_, SIGNAL(toggled(bool)), this, SLOT(widgetsToData()));

  CQChartsWidgetUtil::addGridLabelWidget(groupLayout, "Inside Plot", insideEdit_, row);

  //--

  // box (margin, passing, fill, border, text)
  textEdit_ = CQUtil::makeWidget<CQChartsTextDataEdit>("textEdit");

  textEdit_->setTitle("Box");
  textEdit_->setPreview(false);
  textEdit_->setPlot(title_->plot());
  textEdit_->setView(title_->view());
  textEdit_->setData(data_.textData);

  connect(textEdit_, SIGNAL(textDataChanged()), this, SLOT(widgetsToData()));

  groupLayout->addWidget(textEdit_, row, 0, 1, 2); ++row;

  //---

  groupLayout->setRowStretch(row, 1);

  //---

  widgetsToData();
}

void
CQChartsTitleEdit::
dataToWidgets()
{
  disconnect(groupBox_, SIGNAL(clicked(bool)), this, SLOT(widgetsToData()));
  disconnect(locationEdit_, SIGNAL(titleLocationChanged()), this, SLOT(widgetsToData()));
  disconnect(positionEdit_, SIGNAL(positionChanged()), this, SLOT(widgetsToData()));
  disconnect(rectEdit_, SIGNAL(rectChanged()), this, SLOT(widgetsToData()));
  disconnect(insideEdit_, SIGNAL(toggled(bool)), this, SLOT(widgetsToData()));
  disconnect(textEdit_, SIGNAL(boxDataChanged()), this, SLOT(widgetsToData()));

  groupBox_    ->setChecked      (data_.visible);
  locationEdit_->setTitleLocation(data_.location);
  positionEdit_->setPosition     (data_.position);
  rectEdit_    ->setRect         (data_.rect);
  insideEdit_  ->setChecked      (data_.insidePlot);
  textEdit_    ->setData         (data_.textData);

  connect(groupBox_, SIGNAL(clicked(bool)), this, SLOT(widgetsToData()));
  connect(locationEdit_, SIGNAL(titleLocationChanged()), this, SLOT(widgetsToData()));
  connect(positionEdit_, SIGNAL(positionChanged()), this, SLOT(widgetsToData()));
  connect(rectEdit_, SIGNAL(rectChanged()), this, SLOT(widgetsToData()));
  connect(insideEdit_, SIGNAL(toggled(bool)), this, SLOT(widgetsToData()));
  connect(textEdit_, SIGNAL(boxDataChanged()), this, SLOT(widgetsToData()));
}

void
CQChartsTitleEdit::
widgetsToData()
{
  data_.visible    = groupBox_->isChecked();
  data_.location   = locationEdit_->titleLocation();
  data_.position   = positionEdit_->position();
  data_.rect       = rectEdit_->rect();
  data_.insidePlot = insideEdit_->isChecked();
  data_.textData   = textEdit_->data();

  emit titleChanged();
}

void
CQChartsTitleEdit::
applyData()
{
  title_->setVisible    (data_.visible);
  title_->setLocation   (data_.location);
  title_->setAbsPosition(data_.position);
  title_->setAbsRect    (data_.rect);
  title_->setInsidePlot (data_.insidePlot);
  title_->setTextData   (data_.textData);
}
