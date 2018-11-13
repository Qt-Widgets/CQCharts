#include <CQChartsTableDelegate.h>
#include <CQChartsTable.h>
#include <CQChartsVariant.h>
#include <CQChartsPlotSymbol.h>

#include <QCheckBox>
#include <QPainter>

CQChartsTableDelegate::
CQChartsTableDelegate(CQChartsTable *table) :
 table_(table) {
}

void
CQChartsTableDelegate::
paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  CQBaseModel::Type columnType = getColumnType(index);

  if      (columnType == CQBaseModel::Type::BOOLEAN) {
    QVariant var = table_->modelP()->data(index);

    drawCheckInside(painter, option, var.toBool(), index);
  }
  else if (columnType == CQBaseModel::Type::COLOR) {
    QVariant var = table_->modelP()->data(index, Qt::EditRole);

    if (! var.isValid())
      var = table_->modelP()->data(index, Qt::DisplayRole);

    bool ok;

    CQChartsColor c = CQChartsVariant::toColor(var, ok);

    if (ok)
      drawColor(painter, option, c, index);
    else
      QItemDelegate::paint(painter, option, index);
  }
  else if (columnType == CQBaseModel::Type::SYMBOL) {
    QVariant var = table_->modelP()->data(index, Qt::EditRole);

    if (! var.isValid())
      var = table_->modelP()->data(index, Qt::DisplayRole);

    bool ok;

    CQChartsSymbol symbol = CQChartsVariant::toSymbol(var, ok);

    if (ok)
      drawSymbol(painter, option, symbol, index);
    else
      QItemDelegate::paint(painter, option, index);
  }
  else {
    QVariant var = table_->modelP()->data(index);

    if (var.type() == QVariant::List) {
      QString str;

      CQChartsVariant::toString(var, str);

      drawString(painter, option, str, index);
    }
    else
      QItemDelegate::paint(painter, option, index);
  }
}

QWidget *
CQChartsTableDelegate::
createEditor(QWidget *parent, const QStyleOptionViewItem &item, const QModelIndex &index) const
{
  CQBaseModel::Type columnType = getColumnType(index);

  if (columnType == CQBaseModel::Type::BOOLEAN) {
    QVariant var = table_->modelP()->data(index);

    QCheckBox *check = new QCheckBox(parent);

    check->setObjectName("check");

    check->setChecked(var.toBool());

    check->setText(check->isChecked() ? "true" : "false");

    check->setAutoFillBackground(true);
    //check->setLayoutDirection(Qt::RightToLeft);

    connect(check, SIGNAL(stateChanged(int)), this, SLOT(updateBoolean()));

    currentIndex_ = index;

    return check;
  }
  else {
    return QItemDelegate::createEditor(parent, item, index);
  }
}

void
CQChartsTableDelegate::
click(const QModelIndex &index) const
{
  CQBaseModel::Type columnType = getColumnType(index);

  if (columnType == CQBaseModel::Type::BOOLEAN) {
    QVariant var = table_->modelP()->data(index);

    table_->modelP()->setData(index, ! var.toBool());
  }
}

CQBaseModel::Type
CQChartsTableDelegate::
getColumnType(const QModelIndex &index) const
{
  QAbstractItemModel *model = table_->modelP().data();

  CQChartsTableDelegate *th = const_cast<CQChartsTableDelegate *>(this);

  auto p = th->columnTypeMap_.find(index.column());

  if (p == th->columnTypeMap_.end()) {
    CQBaseModel::Type  columnType;
    CQBaseModel::Type  columnBaseType;
    CQChartsNameValues nameValues;

    (void) CQChartsUtil::columnValueType(table_->charts(), model, index.column(),
                                         columnType, columnBaseType, nameValues);

    p = th->columnTypeMap_.insert(p, ColumnTypeMap::value_type(index.column(), columnType));
  }

  return (*p).second;
}

void
CQChartsTableDelegate::
drawCheckInside(QPainter *painter, const QStyleOptionViewItem &option,
                bool checked, const QModelIndex &index) const
{
  QItemDelegate::drawBackground(painter, option, index);

  Qt::CheckState checkState = (checked ? Qt::Checked : Qt::Unchecked);

  QRect rect = option.rect;

  rect.setWidth(option.rect.height());

  rect.adjust(0, 1, -3, -2);

  QItemDelegate::drawCheck(painter, option, rect, checkState);

  QFontMetrics fm(painter->font());

  int x = rect.right() + 4;
//int y = rect.top() + fm.ascent();

  QRect rect1;

  rect1.setCoords(x, option.rect.top(), option.rect.right(), option.rect.bottom());

  //painter->drawText(x, y, (checked ? "true" : "false"));
  QItemDelegate::drawDisplay(painter, option, rect1, checked ? "true" : "false");
}

void
CQChartsTableDelegate::
drawColor(QPainter *painter, const QStyleOptionViewItem &option,
          const CQChartsColor &color, const QModelIndex &index) const
{
  QItemDelegate::drawBackground(painter, option, index);

  QRect rect = option.rect;

  rect.setWidth(option.rect.height());

  rect.adjust(0, 1, -3, -2);

  QColor c = color.interpColor(table_->charts(), 0, 1);

  painter->fillRect(rect, QBrush(c));

  painter->setPen(QColor(0,0,0)); // TODO: contrast border

  painter->drawRect(rect);

//QFontMetrics fm(painter->font());

  int x = rect.right() + 2;
//int y = rect.top() + fm.ascent();

  QRect rect1;

  rect1.setCoords(x, option.rect.top(), option.rect.right(), option.rect.bottom());

  QItemDelegate::drawDisplay(painter, option, rect1, c.name());
}

void
CQChartsTableDelegate::
drawSymbol(QPainter *painter, const QStyleOptionViewItem &option,
           const CQChartsSymbol &symbol, const QModelIndex &index) const
{
  QItemDelegate::drawBackground(painter, option, index);

  QRect rect = option.rect;

  rect.setWidth(option.rect.height());

  rect.adjust(4, 4, -4, -4);

  painter->setPen(QColor(0,0,0)); // TODO: contrast border

  CQChartsGeom::Point c = CQChartsUtil::fromQPoint(rect.center());

  if (symbol.isValid()) {
    CQChartsSymbol2DRenderer srenderer(painter, c, rect.height()/2.0);

    bool filled = false;

    if (filled) {
      CQChartsPlotSymbolMgr::fillSymbol(symbol, &srenderer);

      CQChartsPlotSymbolMgr::strokeSymbol(symbol, &srenderer);
    }
    else
      CQChartsPlotSymbolMgr::drawSymbol(symbol, &srenderer);
  }

  QFontMetrics fm(painter->font());

  int x = rect.right() + 2;
//int y = rect.top() + fm.ascent();

  QRect rect1;

  rect1.setCoords(x, option.rect.top(), option.rect.right(), option.rect.bottom());

  QItemDelegate::drawDisplay(painter, option, rect1, symbol.toString());
}

void
CQChartsTableDelegate::
drawString(QPainter *painter, const QStyleOptionViewItem &option, const QString &str,
           const QModelIndex &index) const
{
  QItemDelegate::drawBackground(painter, option, index);

  QItemDelegate::drawDisplay(painter, option, option.rect, str);
}

void
CQChartsTableDelegate::
clearColumnTypes()
{
  columnTypeMap_.clear();
}

void
CQChartsTableDelegate::
updateBoolean()
{
  QCheckBox *check = qobject_cast<QCheckBox *>(sender());
  assert(check);

  check->setText(check->isChecked() ? "true" : "false");

  table_->modelP()->setData(currentIndex_, check->isChecked());
}