#ifndef CQChartsNameValues_H
#define CQChartsNameValues_H

#include <QVariant>
#include <QFont>

class CQChartsColor;

class CQChartsNameValues {
 public:
  using NameValues = std::map<QString,QVariant>;

 public:
  CQChartsNameValues() { }

  CQChartsNameValues(const QString &str) { (void) fromString(str); }

  const NameValues &nameValues() const { return nameValues_; }
  void setNameValues(const NameValues &v) { nameValues_ = v; }

  bool empty() const { return nameValues_.empty(); }

  QString toString() const { return toString(nameValues_); }

  static QString toString(const NameValues &nameValues);

  bool fromString(const QString &str);

  static bool fromString(const QString &str, NameValues &nameValues);

  bool nameValue(const QString &name, QVariant &value) const;
  void setNameValue(const QString &name, const QVariant &value);

  template<typename T>
  void setNameValueType(const QString &name, const T &t) {
    setNameValue(name, t.toString());
  }

  bool nameValueString (const QString &name, QString &value) const;
  bool nameValueInteger(const QString &name, int &value) const;
  bool nameValueReal   (const QString &name, double &value) const;
  bool nameValueBool   (const QString &name, bool &value) const;
  bool nameValueColor  (const QString &name, CQChartsColor &color) const;
  bool nameValueFont   (const QString &name, QFont &font) const;
  bool nameValueAlign  (const QString &name, Qt::Alignment &align) const;

  template<typename T>
  bool nameValueType(const QString &name, T &t) const {
    QVariant var;

    if (! nameValue(name, var))
      return false;

    t = T(var.toString());

    return t.isValid();
  }

 private:
  static QStringList splitNameValues(const QString &str);

 private:
  NameValues nameValues_;
};

#endif
