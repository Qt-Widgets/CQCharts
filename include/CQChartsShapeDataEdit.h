#ifndef CQChartsShapeDataEdit_H
#define CQChartsShapeDataEdit_H

#include <CQChartsData.h>
#include <CQChartsLineEditBase.h>

class CQChartsShapeDataEdit;
class CQChartsPlot;
class CQChartsView;

class CQChartsShapeDataLineEdit : public CQChartsLineEditBase {
  Q_OBJECT

  Q_PROPERTY(CQChartsShapeData shapeData READ shapeData WRITE setShapeData)

 public:
  CQChartsShapeDataLineEdit(QWidget *parent=nullptr);

  const CQChartsShapeData &shapeData() const;
  void setShapeData(const CQChartsShapeData &c);

  void drawPreview(QPainter *painter, const QRect &rect) override;

 signals:
  void shapeDataChanged();

 private slots:
  void menuEditChanged();

 private:
  void textChanged() override;

  void updateShapeData(const CQChartsShapeData &c, bool updateText);

  void shapeDataToWidgets();

  void connectSlots(bool b) override;

 private:
  CQChartsShapeDataEdit* dataEdit_ { nullptr };
};

//---

#include <CQChartsEditBase.h>

class CQChartsFillDataEdit;
class CQChartsStrokeDataEdit;
class CQChartsShapeDataEditPreview;

class CQChartsShapeDataEdit : public CQChartsEditBase {
  Q_OBJECT

 public:
  CQChartsShapeDataEdit(QWidget *parent=nullptr);

  const CQChartsShapeData &data() const { return data_; }
  void setData(const CQChartsShapeData &d);

  void setPlot(CQChartsPlot *plot) override;
  void setView(CQChartsView *view) override;

  void setTitle(const QString &title);

  void setPreview(bool b);

 signals:
  void shapeDataChanged();

 private:
  void dataToWidgets();

 private slots:
  void widgetsToData();

 private:
  CQChartsPlot*                 plot_       { nullptr };
  CQChartsView*                 view_       { nullptr };
  CQChartsShapeData             data_;
  CQChartsFillDataEdit*         fillEdit_   { nullptr };
  CQChartsStrokeDataEdit*       strokeEdit_ { nullptr };
  CQChartsShapeDataEditPreview* preview_    { nullptr };
};

//---

class CQChartsShapeDataEditPreview : public CQChartsEditPreview {
  Q_OBJECT

 public:
  CQChartsShapeDataEditPreview(CQChartsShapeDataEdit *edit);

  void draw(QPainter *painter) override;

  static void draw(QPainter *painter, const CQChartsShapeData &data, const QRect &rect,
                   CQChartsPlot *plot, CQChartsView *view);

 private:
  CQChartsShapeDataEdit *edit_ { nullptr };
};

//------

#include <CQChartsPropertyViewEditor.h>

// type for CQChartsShapeData
class CQChartsShapeDataPropertyViewType : public CQChartsPropertyViewType {
 public:
  CQPropertyViewEditorFactory *getEditor() const override;

  void drawPreview(QPainter *painter, const QRect &rect, const QVariant &value,
                   CQChartsPlot *plot, CQChartsView *view) override;

  QString tip(const QVariant &value) const override;
};

//---

// editor factory for CQChartsShapeData
class CQChartsShapeDataPropertyViewEditor : public CQChartsPropertyViewEditorFactory {
 public:
  CQChartsLineEditBase *createPropertyEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QVariant &var);
};

#endif
