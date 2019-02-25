#ifndef CQChartsBoxDataEdit_H
#define CQChartsBoxDataEdit_H

#include <CQChartsData.h>
#include <CQChartsLineEditBase.h>

class CQChartsBoxDataEdit;
class CQChartsPlot;
class CQChartsView;

class CQChartsBoxDataLineEdit : public CQChartsLineEditBase {
  Q_OBJECT

  Q_PROPERTY(CQChartsBoxData boxData READ boxData WRITE setBoxData)

 public:
  CQChartsBoxDataLineEdit(QWidget *parent=nullptr);

  const CQChartsBoxData &boxData() const;
  void setBoxData(const CQChartsBoxData &c);

  void drawPreview(QPainter *painter, const QRect &rect) override;

 signals:
  void boxDataChanged();

 private slots:
  void menuEditChanged();

 private:
  void textChanged() override;

  void updateBoxData(const CQChartsBoxData &c, bool updateText);

  void boxDataToWidgets();

  void connectSlots(bool b) override;

 private:
  CQChartsBoxDataEdit* dataEdit_ { nullptr };
};

//---

#include <CQChartsEditBase.h>

class CQChartsShapeDataEdit;
class CQChartsSidesEdit;
class CQChartsBoxDataEditPreview;
class CQRealSpin;
class CQGroupBox;

class CQChartsBoxDataEdit : public CQChartsEditBase {
  Q_OBJECT

 public:
  CQChartsBoxDataEdit(QWidget *parent=nullptr);

  const CQChartsBoxData &data() const { return data_; }
  void setData(const CQChartsBoxData &d);

  void setPlot(CQChartsPlot *plot) override;
  void setView(CQChartsView *view) override;

  void setTitle(const QString &title);

  void setPreview(bool b);

 signals:
  void boxDataChanged();

 private:
  void dataToWidgets();

 private slots:
  void widgetsToData();

 private:
  CQChartsBoxData             data_;
  CQGroupBox*                 groupBox_    { nullptr };
  CQRealSpin*                 marginEdit_  { nullptr };
  CQRealSpin*                 paddingEdit_ { nullptr };
  CQChartsShapeDataEdit*      shapeEdit_   { nullptr };
  CQChartsSidesEdit*          sidesEdit_   { nullptr };
  CQChartsBoxDataEditPreview* preview_     { nullptr };
};

//---

class CQChartsBoxDataEditPreview : public CQChartsEditPreview {
  Q_OBJECT

 public:
  CQChartsBoxDataEditPreview(CQChartsBoxDataEdit *edit);

  void draw(QPainter *painter) override;

  static void draw(QPainter *painter, const CQChartsBoxData &data, const QRect &rect,
                   CQChartsPlot *plot, CQChartsView *view);

 private:
  CQChartsBoxDataEdit *edit_ { nullptr };
};

//------

#include <CQChartsPropertyViewEditor.h>

// type for CQChartsBoxData
class CQChartsBoxDataPropertyViewType : public CQChartsPropertyViewType {
 public:
  CQPropertyViewEditorFactory *getEditor() const override;

  void drawPreview(QPainter *painter, const QRect &rect, const QVariant &value,
                   CQChartsPlot *plot, CQChartsView *view) override;

  QString tip(const QVariant &value) const override;
};

//---

#include <CQPropertyViewEditor.h>

// editor factory for CQChartsBoxData
class CQChartsBoxDataPropertyViewEditor : public CQChartsPropertyViewEditorFactory {
 public:
  CQChartsLineEditBase *createPropertyEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QVariant &var);
};

#endif
