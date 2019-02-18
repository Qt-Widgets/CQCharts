#ifndef CQChartsFillPattern_H
#define CQChartsFillPattern_H

#include <QString>
#include <QStringList>

class CQChartsFillPattern {
 public:
  enum class Type {
    NONE,
    SOLID,
    HATCH,
    DENSE,
    HORIZ,
    VERT,
    FDIAG,
    BDIAG
  };

 public:
  static void registerMetaType();

  static int metaTypeId;

  static QStringList typeNames();

 public:
  CQChartsFillPattern(Type type=Type::SOLID) :
   type_(type) {
  }

  explicit CQChartsFillPattern(const QString &str) {
    type_ = stringToType(str);
  }

  bool isValid() const { return type_ != Type::NONE; }

  Type type() const { return type_; }
  void setType(Type type) { type_  = type; }

  //---

  Qt::BrushStyle style() const { return typeToStyle(type_); }

  //---

  QString toString() const { return typeToString(type_); }

  bool fromString(const QString &s) {
    Type type = stringToType(s);

    if (type == Type::NONE)
      return false;

    type_ = type;

    return true;
  }

  //---

  static Type stringToType(const QString &str);
  static QString typeToString(const Type &type);

  static Type styleToType(const Qt::BrushStyle &style);
  static Qt::BrushStyle typeToStyle(const Type &type);

  friend bool operator==(const CQChartsFillPattern &lhs, const CQChartsFillPattern &rhs) {
    return (lhs.type_ == rhs.type_);
  }

  friend bool operator!=(const CQChartsFillPattern &lhs, const CQChartsFillPattern &rhs) {
    return (lhs.type_ != rhs.type_);
  }

 private:
  Type type_ { Type::NONE };
};

//---

#include <CQUtilMeta.h>

CQUTIL_DCL_META_TYPE(CQChartsFillPattern)

#endif
