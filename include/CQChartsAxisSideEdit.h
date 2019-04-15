#ifndef CQChartsAxisSideEdit_H
#define CQChartsAxisSideEdit_H

#include <CQChartsAxisSide.h>
#include <CQChartsEnumEdit.h>

//! \brief editor for CQChartsAxisSide
class CQChartsAxisSideEdit : public CQChartsEnumEdit {
  Q_OBJECT

 public:
  CQChartsAxisSideEdit(QWidget *parent=nullptr);

  const CQChartsAxisSide &axisSide() const { return axisSide_; }
  void setAxisSide(const CQChartsAxisSide &pos);

  QStringList enumNames() const override { return axisSide_.enumNames(); }

  void setEnumFromString(const QString &str) override;

  QVariant getVariantFromEnum() const override;

  void setEnumFromVariant(const QVariant &var) override;

  QString variantToString(const QVariant &var) const override;

  void connect(QObject *obj, const char *method) override;

 signals:
  void axisSideChanged();

 private:
  CQChartsAxisSide axisSide_;
  QComboBox*       combo_ { nullptr };
};

//------

//! \brief type for CQChartsAxisSide
class CQChartsAxisSidePropertyViewType : public CQChartsEnumPropertyViewType {
 public:
  CQPropertyViewEditorFactory *getEditor() const override;

  QString variantToString(const QVariant &var) const override;
};

//---

//! \brief editor factory for CQChartsAxisSide
class CQChartsAxisSidePropertyViewEditorFactory :
  public CQChartsEnumPropertyViewEditorFactory {
 public:
  QWidget *createEdit(QWidget *parent) override;
};

#endif
