#ifndef CQChartsModelView_H
#define CQChartsModelView_H

#include <QFrame>
#include <QAbstractItemModel>
#include <QSharedPointer>

class CQCharts;
class CQChartsTable;
class CQChartsTree;

class QStackedWidget;
class QItemSelectionModel;

/*!
 * \brief Model View Widget
 */
class CQChartsModelView : public QFrame {
  Q_OBJECT

 public:
  using ModelP = QSharedPointer<QAbstractItemModel>;

 public:
  CQChartsModelView(CQCharts *charts, QWidget *parent=nullptr);
 ~CQChartsModelView();

  void setFilterAnd(bool b);

  void setFilter(const QString &text);
  void addFilter(const QString &text);

  QString filterDetails() const;

  void setSearch(const QString &text);
  void addSearch(const QString &text);

  void setModel(ModelP model, bool hierarchical);

  QItemSelectionModel *selectionModel();

 signals:
  void filterChanged();

 private:
  CQCharts*       charts_       { nullptr };
  QStackedWidget* stack_        { nullptr };
  CQChartsTable*  table_        { nullptr };
  CQChartsTree*   tree_         { nullptr };
  bool            hierarchical_ { false };
};

#endif
