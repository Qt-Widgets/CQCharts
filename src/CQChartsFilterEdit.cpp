#include <CQChartsFilterEdit.h>
#include <CQIconCombo.h>
#include <CQSwitch.h>

#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QButtonGroup>

#include <svg/filter_light_svg.h>
#include <svg/filter_dark_svg.h>
#include <svg/search_light_svg.h>
#include <svg/search_dark_svg.h>

CQChartsFilterEdit::
CQChartsFilterEdit(QWidget *parent) :
 QFrame(parent)
{
  setObjectName("filterEdit");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  edit_ = new QLineEdit;

  edit_->setObjectName("edit");
  edit_->setClearButtonEnabled(true);

  connect(edit_, SIGNAL(returnPressed()), this, SLOT(acceptSlot()));

  layout->addWidget(edit_);

  combo_ = new CQIconCombo;

  combo_->setObjectName("combo");

  combo_->addItem(CQPixmapCacheInst->getIcon("FILTER_LIGHT", "FILTER_DARK"), "Filter");
  combo_->addItem(CQPixmapCacheInst->getIcon("SEARCH_LIGHT", "SEARCH_DARK"), "Search");

  connect(combo_, SIGNAL(currentIndexChanged(int)), this, SLOT(comboSlot(int)));

  layout->addWidget(combo_);

  QFrame *opFrame = new QFrame;

  opFrame->setObjectName("opFrame");

  QHBoxLayout *opLayout = new QHBoxLayout(opFrame);
  opLayout->setMargin(0); opLayout->setSpacing(2);

  addReplaceSwitch_ = new CQSwitch("Replace", "Add");

  addReplaceSwitch_->setObjectName("add_replace");
  addReplaceSwitch_->setChecked(true);
  addReplaceSwitch_->setHighlightOn(false);

  opLayout->addWidget(addReplaceSwitch_);

  andOrSwitch_ = new CQSwitch("And", "Or");

  andOrSwitch_->setObjectName("and");
  andOrSwitch_->setChecked(true);
  andOrSwitch_->setHighlightOn(false);

  connect(andOrSwitch_, SIGNAL(toggled(bool)), this, SLOT(andSlot()));

  opLayout->addWidget(andOrSwitch_);

  layout->addWidget(opFrame);
}

void
CQChartsFilterEdit::
setFilterDetails(const QString &str)
{
  filterDetails_ = str;

  edit_->setToolTip(filterDetails_);
}

void
CQChartsFilterEdit::
setSearchDetails(const QString &str)
{
  searchDetails_ = str;

  edit_->setToolTip(searchDetails_);
}

void
CQChartsFilterEdit::
comboSlot(int /*ind*/)
{
  if (combo_->currentIndex() == 0) {
    edit_->setText(filterText_);

    edit_->setToolTip(filterDetails_);
  }
  else {
    edit_->setText(searchText_);

    edit_->setToolTip(searchDetails_);
  }
}

void
CQChartsFilterEdit::
andSlot()
{
  bool b = andOrSwitch_->isChecked();

  emit filterAnd(b);
}

void
CQChartsFilterEdit::
acceptSlot()
{
  QString text = edit_->text();

  if (combo_->currentIndex() == 0) {
    if (text != filterText_) {
      filterText_    = text;
      filterDetails_ = "";

      if (addReplaceSwitch_->isChecked())
        emit replaceFilter(filterText_);
      else
        emit addFilter(filterText_);
    }
  }
  else {
    if (text != searchText_) {
      searchText_ = text;

      if (addReplaceSwitch_->isChecked())
        emit replaceSearch(searchText_);
      else
        emit addSearch(searchText_);
    }
  }
}
