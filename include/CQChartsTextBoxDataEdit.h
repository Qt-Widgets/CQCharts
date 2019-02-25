#ifndef CQChartsTextBoxDataEdit_H
#define CQChartsTextBoxDataEdit_H

#include <CQChartsData.h>
#include <CQChartsLineEditBase.h>

class CQChartsTextBoxDataEdit;
class CQChartsPlot;
class CQChartsView;

class CQChartsTextBoxDataLineEdit : public CQChartsLineEditBase {
  Q_OBJECT

  Q_PROPERTY(CQChartsTextBoxData textBoxData READ textBoxData WRITE setTextBoxData)

 public:
  CQChartsTextBoxDataLineEdit(QWidget *parent=nullptr);

  const CQChartsTextBoxData &textBoxData() const;
  void setTextBoxData(const CQChartsTextBoxData &c);

  void drawPreview(QPainter *painter, const QRect &rect) override;

 signals:
  void textBoxDataChanged();

 private slots:
  void menuEditChanged();

 private:
  void textChanged() override;

  void updateTextBoxData(const CQChartsTextBoxData &c, bool updateText);

  void textBoxDataToWidgets();

  void connectSlots(bool b) override;

 private:
  CQChartsTextBoxDataEdit* dataEdit_ { nullptr };
};

//---

#include <CQChartsEditBase.h>

class CQChartsTextDataEdit;
class CQChartsBoxDataEdit;
class CQChartsTextBoxDataEditPreview;

class CQChartsTextBoxDataEdit : public CQChartsEditBase {
  Q_OBJECT

 public:
  CQChartsTextBoxDataEdit(QWidget *parent=nullptr);

  const CQChartsTextBoxData &data() const { return data_; }
  void setData(const CQChartsTextBoxData &d);

  void setPlot(CQChartsPlot *plot) override;
  void setView(CQChartsView *view) override;

  void setTitle(const QString &title);

  void setPreview(bool b);

 signals:
  void textBoxDataChanged();

 private:
  void dataToWidgets();

 private slots:
  void widgetsToData();

 private:
  CQChartsPlot*                   plot_     { nullptr };
  CQChartsView*                   view_     { nullptr };
  CQChartsTextBoxData             data_;
  CQChartsTextDataEdit*           textEdit_ { nullptr };
  CQChartsBoxDataEdit*            boxEdit_  { nullptr };
  CQChartsTextBoxDataEditPreview* preview_  { nullptr };
};

//---

class CQChartsTextBoxDataEditPreview : public CQChartsEditPreview {
  Q_OBJECT

 public:
  CQChartsTextBoxDataEditPreview(CQChartsTextBoxDataEdit *edit);

  void draw(QPainter *painter) override;

  static void draw(QPainter *painter, const CQChartsTextBoxData &data, const QRect &rect,
                   CQChartsPlot *plot, CQChartsView *view);

 private:
  CQChartsTextBoxDataEdit *edit_ { nullptr };
};

//------

#include <CQChartsPropertyViewEditor.h>

// type for CQChartsTextBoxData
class CQChartsTextBoxDataPropertyViewType : public CQChartsPropertyViewType {
 public:
  CQPropertyViewEditorFactory *getEditor() const override;

  void drawPreview(QPainter *painter, const QRect &rect, const QVariant &value,
                   CQChartsPlot *plot, CQChartsView *view) override;

  QString tip(const QVariant &value) const override;
};

//---

// editor factory for CQChartsTextBoxData
class CQChartsTextBoxDataPropertyViewEditor : public CQChartsPropertyViewEditorFactory {
 public:
  CQChartsLineEditBase *createPropertyEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QVariant &var);
};

//------

#endif
