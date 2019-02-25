#ifndef CQChartsColumnEdit_H
#define CQChartsColumnEdit_H

#include <CQChartsColumn.h>
#include <CQChartsLineEditBase.h>

class CQChartsColumnEdit;
class QAbstractItemModel;

class CQChartsColumnLineEdit : public CQChartsLineEditBase {
  Q_OBJECT

  Q_PROPERTY(CQChartsColumn column READ column WRITE setColumn)

 public:
  CQChartsColumnLineEdit(QWidget *parent=nullptr);

  void setPlot(CQChartsPlot *plot) override;

  QAbstractItemModel *model() const;
  void setModel(QAbstractItemModel *model);

  const CQChartsColumn &column() const;
  void setColumn(const CQChartsColumn &c);

  void drawPreview(QPainter *painter, const QRect &rect);

 signals:
  void columnChanged();

 private slots:
  void menuEditChanged();

 private:
  void textChanged() override;

  void updateColumn(const CQChartsColumn &c, bool updateText);

  void columnToWidgets();

  void connectSlots(bool b) override;

 private:
  CQChartsColumnEdit* dataEdit_ { nullptr };
};

//---

#include <CQChartsEditBase.h>

class CQGroupBox;
class QLineEdit;
class QComboBox;
class QCheckBox;

class CQChartsColumnEdit : public CQChartsEditBase {
  Q_OBJECT

  Q_PROPERTY(CQChartsColumn column READ column WRITE setColumn)

 public:
  CQChartsColumnEdit(QWidget *parent=nullptr);

  QAbstractItemModel *model() const { return model_; }
  void setModel(QAbstractItemModel *model);

  const CQChartsColumn &column() const;
  void setColumn(const CQChartsColumn &c);

 signals:
  void columnChanged();

 private slots:
  void widgetsToColumn();

  void updateState();

  void menuColumnGroupClicked(bool b);
  void menuExprGroupClicked  (bool b);
  void vheaderCheckClicked   (bool b);

  void menuColumnChanged(int i);
  void roleTextChanged(const QString &str);

  void expressionTextChanged(const QString &str);

 private:
  void columnToWidgets();

  void updateColumnsFromModel();

  void connectSlots(bool b);

 private:
  QAbstractItemModel* model_          { nullptr };
  CQChartsColumn      column_;
  CQGroupBox*         columnGroup_    { nullptr };
  QComboBox*          columnCombo_    { nullptr };
  QLineEdit*          roleEdit_       { nullptr };
  CQGroupBox*         menuExprGroup_  { nullptr };
  QLineEdit*          expressionEdit_ { nullptr };
  QCheckBox*          vheaderCheck_   { nullptr };
};

//------

#include <CQPropertyViewType.h>

// type for CQChartsColumn
class CQChartsColumnPropertyViewType : public CQPropertyViewType {
 public:
  CQChartsColumnPropertyViewType();

  CQPropertyViewEditorFactory *getEditor() const override;

  bool setEditorData(CQPropertyViewItem *item, const QVariant &value) override;

  void draw(const CQPropertyViewDelegate *delegate, QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index,
            const QVariant &value, bool inside) override;

  QString tip(const QVariant &value) const override;
};

//---

#include <CQPropertyViewEditor.h>

// editor factory for CQChartsColumn
class CQChartsColumnPropertyViewEditor : public CQPropertyViewEditorFactory {
 public:
  CQChartsColumnPropertyViewEditor();

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QVariant &var);
};

#endif
