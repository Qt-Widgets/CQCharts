#ifndef CQChartsModelData_H
#define CQChartsModelData_H

#include <QObject>
#include <QSharedPointer>

class CQChartsModelDetails;
class CQCharts;
class CQSummaryModel;
class QAbstractItemModel;
class QItemSelectionModel;

class CQChartsModelData : public QObject {
  Q_OBJECT

 public:
  using ModelP = QSharedPointer<QAbstractItemModel>;

#ifdef CQCHARTS_FOLDED_MODEL
  using FoldedModels = std::vector<ModelP>;
#endif

 public:
  CQChartsModelData(CQCharts *charts, ModelP &model);

 ~CQChartsModelData();

  CQCharts *charts() const { return charts_; }

  // get model
  ModelP &model() { return model_; }
  const ModelP &model() const { return model_; }

  // get (unique) index
  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  QString id() const;

  // get/set selection model
  QItemSelectionModel *selectionModel() const { return selectionModel_; }
  void setSelectionModel(QItemSelectionModel *p) { selectionModel_ = p; }

  // get/set name
  const QString &name() const { return name_; }
  void setName(const QString &s) { name_ = s; }

#ifdef CQCHARTS_FOLDED_MODEL
  // get associated fold models
  ModelP foldProxyModel() const { return foldProxyModel_; }
  void setFoldProxyModel(ModelP &model) { foldProxyModel_ = model; }
  void resetFoldProxyModel() { foldProxyModel_ = ModelP(); }

  const FoldedModels &foldedModels() const { return foldedModels_; }

  void addFoldedModel(ModelP &model) { foldedModels_.push_back(model); }
  void clearFoldedModels() { foldedModels_.clear(); }

  // fold model
  void foldModel(const QString &str);
  void foldClear();
#endif

  // get details
  CQChartsModelDetails *details();
  const CQChartsModelDetails *details() const;

  ModelP currentModel() const;

  // add/get summary model
  CQSummaryModel *addSummaryModel();

  bool isSummaryEnabled() const { return summaryEnabled_; }
  void setSummaryEnabled(bool b) { summaryEnabled_ = b; }

  CQSummaryModel *summaryModel() const { return summaryModel_; }

  CQChartsModelData *summaryModelData() const { return summaryModelData_; }

  const ModelP &summaryModelP() const { return summaryModelP_; }

 private:
  void connectModel();
  void disconnectModel();

 private slots:
  void modelDataChangedSlot(const QModelIndex &, const QModelIndex &);

  void modelLayoutChangedSlot();
  void modelResetSlot();

  void modelRowsInsertedSlot();
  void modelRowsRemovedSlot();
  void modelColumnsInsertedSlot();
  void modelColumnsRemovedSlot();

 signals:
  void modelChanged();

 private:
  CQCharts*             charts_           { nullptr }; // parent charts
  ModelP                model_;                        // model
  int                   ind_              { -1 };      // model ind
  QItemSelectionModel*  selectionModel_   { nullptr }; // selection model
  QString               name_;                         // model name
  CQChartsModelDetails* details_          { nullptr }; // model details
#ifdef CQCHARTS_FOLDED_MODEL
  ModelP                foldProxyModel_;               // folded proxy model
  FoldedModels          foldedModels_;                 // folded models
#endif
  bool                  summaryEnabled_   { false };   // summary model enabled
  CQSummaryModel*       summaryModel_     { nullptr }; // summary model
  ModelP                summaryModelP_;                // summary model (shared pointer)
  CQChartsModelData*    summaryModelData_ { nullptr }; // summary model data
};

#endif