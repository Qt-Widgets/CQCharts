#include <CQCollapseModel.h>
#include <set>
#include <cassert>

CQCollapseModel::
CQCollapseModel(QAbstractItemModel *model)
{
  setObjectName("collapseModel");

  setSourceModel(model);
}

CQCollapseModel::
~CQCollapseModel()
{
}

QAbstractItemModel *
CQCollapseModel::
sourceModel() const
{
  return QAbstractProxyModel::sourceModel();
}

void
CQCollapseModel::
setSourceModel(QAbstractItemModel *sourceModel)
{
  QAbstractProxyModel::setSourceModel(sourceModel);
}

//------

void
CQCollapseModel::
setColumnType(int column, CQBaseModelType type)
{
  ColumnConfig &columnConfig = getColumnConfig(column);

  columnConfig.type = type;

  int nr = rowCount();

  for (int r = 0; r < nr; ++r) {
    VariantData &variantData = getVariantData(r, column, Qt::DisplayRole);

    variantData.resetDisplayValue();
  }
}

void
CQCollapseModel::
setColumnCollapseOp(int column, CollapseOp op)
{
  ColumnConfig &columnConfig = getColumnConfig(column);

  columnConfig.collapseOp = op;

  int nr = rowCount();

  for (int r = 0; r < nr; ++r) {
    VariantData &variantData = getVariantData(r, column, Qt::DisplayRole);

    variantData.resetDisplayValue();
  }
}

//------

// get number of columns
int
CQCollapseModel::
columnCount(const QModelIndex &) const
{
  // extra count column
  QAbstractItemModel *model = this->sourceModel();

  return model->columnCount() + 1;
}

// get number of child rows for parent
int
CQCollapseModel::
rowCount(const QModelIndex &parent) const
{
  // only top level rows
  // TODO: collapse by one level ?
  if (parent.isValid())
    return 0;

  QAbstractItemModel *model = this->sourceModel();

  return model->rowCount(parent);
}

// get child node for row/column of parent
QModelIndex
CQCollapseModel::
index(int row, int column, const QModelIndex &parent) const
{
  int nr = rowCount(parent);

  if (row < 0 || row >= nr)
    return QModelIndex();

  return createIndex(row, column);
}

// get parent for child
QModelIndex
CQCollapseModel::
parent(const QModelIndex &) const
{
  // flat - no parent
  return QModelIndex();
}

bool
CQCollapseModel::
hasChildren(const QModelIndex &parent) const
{
  // flat - only root (no parent) has children
  if (! parent.isValid())
    return (rowCount() > 0);

  return false;
}

QVariant
CQCollapseModel::
data(const QModelIndex &index, int role) const
{
  // flat - only data at root
  if (index.parent().isValid())
    return QVariant();

  //---

  QAbstractItemModel *model = this->sourceModel();

  int c  = index.column();
  int nc = model->columnCount();

  if (c < 0 || c > nc)
    return QVariant();

  //---

  // count in last column
  if (c == nc) {
    QModelIndex ind1 = model->index(index.row(), 0, QModelIndex());

    if (role == Qt::DisplayRole || role == Qt::EditRole)
      return model->rowCount(ind1);
    else
      return QVariant();
  }

  //---

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    // collapsed data in other columns
    int r = index.row();

    VariantData &variantData = getVariantData(r, c, role);

    ColumnConfig &columnConfig = const_cast<CQCollapseModel *>(this)->getColumnConfig(c);

    return variantData.displayValue(columnConfig);
  }
  else {
    return QVariant();
  }
}

bool
CQCollapseModel::
setData(const QModelIndex &, const QVariant &, int)
{
  // not allowed
  return false;
}

QVariant
CQCollapseModel::
headerData(int section, Qt::Orientation orientation, int role) const
{
  QAbstractItemModel *model = this->sourceModel();

  int nc = model->columnCount();

  if (section < 0 || section > nc)
    return QVariant();

  //----

  // count in last column
  if (section == nc) {
    if      (orientation == Qt::Horizontal) {
      if      (role == Qt::DisplayRole || role == Qt::EditRole)
        return "Count";
      else if (role == Qt::ToolTipRole)
        return "Count";
      else if (role == static_cast<int>(CQBaseModelRole::Type)) {
        return static_cast<int>(CQBaseModelType::INTEGER);
      }
      else if (role == static_cast<int>(CQBaseModelRole::BaseType)) {
        return static_cast<int>(CQBaseModelType::INTEGER);
      }
    }
    else
      return QVariant();
  }

  //---

  // handle column types
  if (orientation == Qt::Horizontal) {
    if (role == static_cast<int>(CQBaseModelRole::Type) ||
        role == static_cast<int>(CQBaseModelRole::BaseType)) {
      ColumnConfig &columnConfig = const_cast<CQCollapseModel *>(this)->getColumnConfig(section);

      if      (role == static_cast<int>(CQBaseModelRole::Type)) {
        return static_cast<int>(columnConfig.type);
      }
      else if (role == static_cast<int>(CQBaseModelRole::BaseType)) {
        return static_cast<int>(columnConfig.type);
      }
    }
  }

  // pass to source model
  return model->headerData(section, orientation, role);
}

bool
CQCollapseModel::
setHeaderData(int section, Qt::Orientation orientation, const QVariant &var, int role)
{
  QAbstractItemModel *model = this->sourceModel();

  int nc = model->columnCount();

  if (section < 0 || section > nc)
    return false;

  //----

  // not allowed for last column
  // TODO: allow change name ?
  if (section == nc) {
    return false;
  }

  //---

  // pass to source model
  return model->setHeaderData(section, orientation, var, role);
}

Qt::ItemFlags
CQCollapseModel::
flags(const QModelIndex & /*index*/) const
{
  //QAbstractItemModel *model = this->sourceModel();

  // pass to source model
  // TODO: remove editable, others ?
  //return model->flags(mapToSource(index));

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

// map index in source model to collapse model
QModelIndex
CQCollapseModel::
mapFromSource(const QModelIndex &sourceIndex) const
{
  if (! sourceIndex.isValid())
    return QModelIndex();

  // must be root
  if (sourceIndex.parent().isValid())
    return QModelIndex();

  int r = sourceIndex.row   ();
  int c = sourceIndex.column();

  if (c < 0 || c >= columnCount() - 1)
    return QModelIndex();

  //---

  return this->index(r, c);
}

// map index in collapse model to source model
QModelIndex
CQCollapseModel::
mapToSource(const QModelIndex &proxyIndex) const
{
  if (! proxyIndex.isValid())
    return QModelIndex();

  // must be root
  if (proxyIndex.parent().isValid())
    return QModelIndex();

  int r = proxyIndex.row   ();
  int c = proxyIndex.column();

  if (c < 0 || c >= columnCount())
    return QModelIndex();

  QAbstractItemModel *model = this->sourceModel();

  return model->index(r, c);
}

//------

CQCollapseModel::ColumnConfig &
CQCollapseModel::
getColumnConfig(int c)
{
  auto p = columnConfigMap_.find(c);

  if (p == columnConfigMap_.end())
    p = columnConfigMap_.insert(p, ColumnConfigMap::value_type(c, ColumnConfig()));

  return (*p).second;
}

CQCollapseModel::VariantData &
CQCollapseModel::
getVariantData(int r, int c, int role) const
{
  CQCollapseModel *th = const_cast<CQCollapseModel *>(this);

  auto pr = th->rowDataMap_.find(r);

  if (pr == th->rowDataMap_.end())
    pr = th->rowDataMap_.insert(pr, RowDataMap::value_type(r, ColumnDataMap()));

  ColumnDataMap &columnDataMap = (*pr).second;

  auto pc = columnDataMap.find(c);

  if (pc == columnDataMap.end())
    pc = columnDataMap.insert(pc, ColumnDataMap::value_type(c, ColumnData()));

  ColumnData &columnData = (*pc).second;

  auto pd = columnData.roleDataMap.find(role);

  if (pd == columnData.roleDataMap.end())
    pd = columnData.roleDataMap.insert(pd, RoleDataMap::value_type(role, VariantData()));

  VariantData &variantData = (*pd).second;

  if (! variantData.isSet())
    collapseRowColumn(r, c, role, variantData);

  return variantData;
}

void
CQCollapseModel::
collapseRowColumn(int row, int column, int role, VariantData &variantData) const
{
  QAbstractItemModel *model = this->sourceModel();

  //---

  QModelIndex ind = model->index(row, column, QModelIndex());

  QVariant var = model->data(ind, role);

  variantData.setParent(var);

  //---

  QModelIndex parent = model->index(row, 0, QModelIndex());

  int nr = model->rowCount(parent);

  for (int r = 0; r < nr; ++r) {
    QModelIndex index = model->index(r, column, parent);

    QVariant var = model->data(index, role);

    variantData.addChild(var);
  }
}

//------

QVariant
CQCollapseModel::VariantData::
displayValue(const ColumnConfig &config) const
{
  if (! displayValue_.isValid()) {
    CQCollapseModel::VariantData *th = const_cast<CQCollapseModel::VariantData *>(this);

    if (config.collapseOp == CollapseOp::UNIQUE) {
      QString pstr = parent_.toString();

      if (pstr != "") {
        th->displayValue_ = pstr;
      }
      else {
        QString str;
        bool    valid = true;

        int nc = children_.length();

        for (int i = 0; i < nc; ++i) {
          const QVariant &var = children_[i];

          QString str1 = var.toString();

          if (str1 == "")
            continue;

          if      (str == "")
            str = str1;
          else if (str != str1) {
            valid = false;
            break;
          }
        }

        if (valid)
          th->displayValue_ = str;
        else
          th->displayValue_ = "";
      }
    }
    else if (config.collapseOp == CollapseOp::SUM ||
             config.collapseOp == CollapseOp::MEAN) {
      if (config.type == CQBaseModelType::REAL || config.type == CQBaseModelType::INTEGER) {
        bool ok;

        double pr = parent_.toDouble(&ok);

        if (ok) {
          th->displayValue_ = pr;
        }
        else {
          int nc = children_.length();

          double sum = 0.0;
          int    nv  = 0;

          for (int i = 0; i < nc; ++i) {
            const QVariant &var = children_[i];

            bool ok;

            double r = var.toDouble(&ok);

            if (! ok)
              continue;

            sum += r;

            ++nv;
          }

          if (config.collapseOp == CollapseOp::MEAN && nv > 0)
            sum /= nv;

          th->displayValue_ = sum;
        }
      }
      else {
        th->displayValue_ = "";
      }
    }
    else {
      th->displayValue_ = "";
    }
  }

  return displayValue_;
}
