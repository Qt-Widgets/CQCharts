#include <CQChartsModelList.h>
#include <CQChartsModelControl.h>
#include <CQChartsTable.h>
#include <CQChartsTree.h>
#include <CQChartsModelData.h>
#include <CQChartsModelDetails.h>
#include <CQCharts.h>
#include <CQTableWidget.h>
#include <CQUtil.h>

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>

CQChartsModelList::
CQChartsModelList(CQCharts *charts) :
 charts_(charts)
{
  setObjectName("modelList");

  QVBoxLayout *layout = new QVBoxLayout(this);

  //---

  viewTab_ = CQUtil::makeWidget<QTabWidget>("viewTab");

  layout->addWidget(viewTab_);

  connect(viewTab_, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));

  //---

  connect(charts, SIGNAL(modelDataAdded(int)), this, SLOT(addModelData(int)));

  connect(charts, SIGNAL(modelTypeChanged(int)), this, SLOT(updateModelType(int)));
}

CQChartsModelList::
~CQChartsModelList()
{
  for (auto &p : viewWidgetDatas_)
    delete p.second;
}

void
CQChartsModelList::
addModelData(int ind)
{
  CQChartsModelData *modelData = charts_->getModelData(ind);

  if (! modelData)
    return;

  addModelData(modelData);
}

void
CQChartsModelList::
addModelData(CQChartsModelData *modelData)
{
  addModelDataWidgets(modelData);

  updateModel(modelData);

  if (modelControl_)
    modelControl_->setEnabled(numModels() > 0);
}

void
CQChartsModelList::
updateModel(CQChartsModelData *modelData)
{
  reloadModel(modelData);

  setDetails(modelData);
}

void
CQChartsModelList::
updateModelType(int ind)
{
  CQChartsModelData *modelData = charts_->getModelData(ind);

  if (! modelData)
    return;

  setDetails(modelData);
}

void
CQChartsModelList::
addModelDataWidgets(CQChartsModelData *modelData)
{
  CQChartsViewWidgetData *viewWidgetData = new CQChartsViewWidgetData;

  viewWidgetData->ind = modelData->ind();

  viewWidgetDatas_[viewWidgetData->ind] = viewWidgetData;

  //---

  QTabWidget *tableTab = CQUtil::makeWidget<QTabWidget>("tableTab");

  int tabInd = viewTab_->addTab(tableTab, QString("Model %1").arg(viewWidgetData->ind));

  viewTab_->setCurrentIndex(viewTab_->count() - 1);

  viewWidgetData->tabInd = tabInd;

  //---

  QFrame *viewFrame = CQUtil::makeWidget<QFrame>("view");

  QVBoxLayout *viewLayout = new QVBoxLayout(viewFrame);

  tableTab->addTab(viewFrame, "Model");

  //---

  QFrame *detailsFrame = CQUtil::makeWidget<QFrame>("details");

  QVBoxLayout *detailsLayout = new QVBoxLayout(detailsFrame);

  tableTab->addTab(detailsFrame, "Details");

  //---

  QLineEdit *filterEdit = CQUtil::makeWidget<QLineEdit>("filter");

  viewLayout->addWidget(filterEdit);

  connect(filterEdit, SIGNAL(returnPressed()), this, SLOT(filterSlot()));

  viewWidgetData->filterEdit = filterEdit;

  //---

  QStackedWidget *stack = CQUtil::makeWidget<QStackedWidget>("stack");

  viewLayout->addWidget(stack);

  viewWidgetData->stack = stack;

  //---

  CQChartsTree *tree = new CQChartsTree(charts_);

  stack->addWidget(tree);

  connect(tree, SIGNAL(columnClicked(int)), this, SLOT(treeColumnClicked(int)));
  connect(tree, SIGNAL(selectionChanged()), this, SLOT(treeSelectionChanged()));

  viewWidgetData->tree = tree;

  //---

  CQChartsTable *table = new CQChartsTable(charts_);

  stack->addWidget(table);

  connect(table, SIGNAL(columnClicked(int)), this, SLOT(tableColumnClicked(int)));
  connect(table, SIGNAL(selectionChanged()), this, SLOT(tableSelectionChanged()));

  viewWidgetData->table = table;

  //------

  QTextEdit *detailsText = CQUtil::makeWidget<QTextEdit>("detailsText");

  detailsText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  detailsText->setReadOnly(true);

  detailsLayout->addWidget(detailsText);

  viewWidgetData->detailsText = detailsText;

  CQTableWidget *detailsTable = CQUtil::makeWidget<CQTableWidget>("detailsText");

  detailsLayout->addWidget(detailsTable);

  viewWidgetData->detailsTable = detailsTable;
}

void
CQChartsModelList::
currentTabChanged(int)
{
  CQChartsViewWidgetData *viewWidgetData = currentViewWidgetData();

  if (viewWidgetData)
    charts_->setCurrentModelInd(viewWidgetData->ind);
}

CQChartsModelData *
CQChartsModelList::
currentModelData() const
{
  CQChartsViewWidgetData *viewWidgetData = currentViewWidgetData();

  if (viewWidgetData)
    return charts_->getModelData(viewWidgetData->ind);

  return nullptr;
}

CQChartsViewWidgetData *
CQChartsModelList::
currentViewWidgetData() const
{
  int ind = viewTab_->currentIndex();

  for (auto &p : viewWidgetDatas_) {
    CQChartsViewWidgetData *viewWidgetData = p.second;

    if (viewWidgetData->tabInd == ind)
      return viewWidgetData;
  }

  return nullptr;
}

void
CQChartsModelList::
filterSlot()
{
  QLineEdit *filterEdit = qobject_cast<QLineEdit *>(sender());

  for (auto &p : viewWidgetDatas_) {
    CQChartsViewWidgetData *viewWidgetData = p.second;

    if (viewWidgetData->filterEdit == filterEdit) {
      if (viewWidgetData->stack->currentIndex() == 0) {
        if (viewWidgetData->tree)
          viewWidgetData->tree->setFilter(filterEdit->text());
      }
      else {
        if (viewWidgetData->table)
          viewWidgetData->table->setFilter(filterEdit->text());
      }
    }
  }
}

void
CQChartsModelList::
treeColumnClicked(int column)
{
  if (modelControl_)
    modelControl_->setColumnData(column);
}

void
CQChartsModelList::
treeSelectionChanged()
{
}

void
CQChartsModelList::
tableColumnClicked(int column)
{
  if (modelControl_)
    modelControl_->setColumnData(column);
}

void
CQChartsModelList::
tableSelectionChanged()
{
  CQChartsViewWidgetData *viewWidgetData = currentViewWidgetData();

  if (viewWidgetData) {
    QItemSelectionModel *sm = viewWidgetData->table->selectionModel();

    QModelIndexList inds = sm->selectedColumns();

    if (inds.size() == 1) {
      int column = inds[0].column();

      if (modelControl_)
        modelControl_->setColumnData(column);
    }
  }
}

void
CQChartsModelList::
setTabTitle(int ind, const QString &title)
{
  CQChartsViewWidgetData *viewWidgetData = this->viewWidgetData(ind);
  assert(viewWidgetData);

  if (viewWidgetData->stack->currentIndex() == 0) {
    if (viewWidgetData->tree)
      viewWidgetData->tree->setWindowTitle(title);
  }
  else {
    if (viewWidgetData->table)
      viewWidgetData->table->setWindowTitle(title);
  }

  viewTab()->setTabText(viewWidgetData->tabInd, title);
}

void
CQChartsModelList::
redrawView(const CQChartsModelData *modelData)
{
  CQChartsViewWidgetData *viewWidgetData = this->viewWidgetData(modelData->ind());
  assert(viewWidgetData);

  if (viewWidgetData->stack->currentIndex() == 0) {
    if (viewWidgetData->tree)
      viewWidgetData->tree->update();
  }
  else {
    if (viewWidgetData->table)
      viewWidgetData->table->update();
  }
}

void
CQChartsModelList::
reloadModel(CQChartsModelData *modelData)
{
  CQChartsViewWidgetData *viewWidgetData = this->viewWidgetData(modelData->ind());
  assert(viewWidgetData);

  if (modelData->details()->isHierarchical()) {
    if (viewWidgetData->tree) {
      viewWidgetData->tree->setModelP(modelData->currentModel());

      modelData->setSelectionModel(viewWidgetData->tree->selectionModel());
    }
    else
      modelData->setSelectionModel(nullptr);

    viewWidgetData->stack->setCurrentIndex(0);
  }
  else {
    if (viewWidgetData->table) {
      viewWidgetData->table->setModelP(modelData->currentModel());

      modelData->setSelectionModel(viewWidgetData->table->selectionModel());
    }
    else
      modelData->setSelectionModel(nullptr);

    viewWidgetData->stack->setCurrentIndex(1);
  }
}

void
CQChartsModelList::
setDetails(const CQChartsModelData *modelData)
{
  currentViewWidgetData_ = this->viewWidgetData(modelData->ind());
  assert(currentViewWidgetData_);

  //---

  CQChartsModelData *modelData1 = nullptr;

  if (currentViewWidgetData_->stack->currentIndex() == 0) {
    if (currentViewWidgetData_->tree)
      modelData1 = charts_->getModelData(currentViewWidgetData_->tree->modelP().data());
  }
  else {
    if (currentViewWidgetData_->table)
      modelData1 = charts_->getModelData(currentViewWidgetData_->table->modelP().data());
  }

  if (! modelData1)
    modelData1 = const_cast<CQChartsModelData *>(modelData);

  //---

  if (currentDetails_)
    disconnect(currentDetails_, SIGNAL(detailsReset()), this, SLOT(updateDetails()));

  currentDetails_ = nullptr;

  if (modelData1)
    currentDetails_ = modelData1->details();

  if (! currentDetails_)
    return;

  connect(currentDetails_, SIGNAL(detailsReset()), this, SLOT(updateDetails()));

  //---

  updateDetails();
}

void
CQChartsModelList::
updateDetails()
{
  assert(currentDetails_);

  //---

  int nc = currentDetails_->numColumns();
  int nr = currentDetails_->numRows   ();

  //---

  currentViewWidgetData_->detailsTable->clear();

  currentViewWidgetData_->detailsTable->setColumnCount(7);

  currentViewWidgetData_->detailsTable->setHorizontalHeaderLabels(QStringList() <<
    "Column" << "Type" << "Min" << "Max" << "Mean" << "StdDev" << "Monotonic");

  currentViewWidgetData_->detailsTable->setRowCount(nc);

  auto columnDetails = [&](int c, QString &nameStr, QString &typeStr, QString &minStr,
                           QString &maxStr, QString &meanStr, QString &stdDevStr,
                           QString &monoStr) {
    const CQChartsModelColumnDetails *columnDetails = currentDetails_->columnDetails(c);

    nameStr   = columnDetails->headerName();
    typeStr   = columnDetails->typeName();
    minStr    = columnDetails->dataName(columnDetails->minValue   ()).toString();
    maxStr    = columnDetails->dataName(columnDetails->maxValue   ()).toString();
    meanStr   = columnDetails->dataName(columnDetails->meanValue  ()).toString();
    stdDevStr = columnDetails->dataName(columnDetails->stdDevValue()).toString();

    if (columnDetails->isMonotonic())
      monoStr = (columnDetails->isIncreasing() ? "Increasing" : "Decreasing");
    else
      monoStr = "";
  };

  auto setTableRow = [&](int c) {
    QString nameStr, typeStr, minStr, maxStr, meanStr, stdDevStr, monoStr;

    columnDetails(c, nameStr, typeStr, minStr, maxStr, meanStr, stdDevStr, monoStr);

    QTableWidgetItem *item1 = new QTableWidgetItem(nameStr);
    QTableWidgetItem *item2 = new QTableWidgetItem(typeStr);
    QTableWidgetItem *item3 = new QTableWidgetItem(minStr);
    QTableWidgetItem *item4 = new QTableWidgetItem(maxStr);
    QTableWidgetItem *item5 = new QTableWidgetItem(meanStr);
    QTableWidgetItem *item6 = new QTableWidgetItem(stdDevStr);
    QTableWidgetItem *item7 = new QTableWidgetItem(monoStr);

    currentViewWidgetData_->detailsTable->setItem(c, 0, item1);
    currentViewWidgetData_->detailsTable->setItem(c, 1, item2);
    currentViewWidgetData_->detailsTable->setItem(c, 2, item3);
    currentViewWidgetData_->detailsTable->setItem(c, 3, item4);
    currentViewWidgetData_->detailsTable->setItem(c, 4, item5);
    currentViewWidgetData_->detailsTable->setItem(c, 5, item6);
    currentViewWidgetData_->detailsTable->setItem(c, 6, item7);
  };

  //---

  QString text = "<b></b>";

  text += "<table padding=\"4\">";
  text += QString("<tr><td>Columns</td><td>%1</td></tr>").arg(nc);
  text += QString("<tr><td>Rows</td><td>%1</td></tr>").arg(nr);
  text += "</table>";

  currentViewWidgetData_->detailsText->setHtml(text);

  //---

  for (int c = 0; c < nc; ++c)
    setTableRow(c);
}

CQChartsViewWidgetData *
CQChartsModelList::
viewWidgetData(int ind) const
{
  auto p = viewWidgetDatas_.find(ind);

  if (p == viewWidgetDatas_.end())
    return nullptr;

  return (*p).second;
}
