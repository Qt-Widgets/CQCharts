#ifndef CQBucketer_H
#define CQBucketer_H

#include <QVariant>
#include <cmath>

class CQBucketer {
 public:
  enum class Type {
    STRING,
    INTEGER_RANGE,
    REAL_RANGE,
    REAL_AUTO
  };

 public:
  CQBucketer();

  //---

  // get/set value type
  const Type &type() const { return type_; }
  void setType(const Type &t);

  //---

  // fixed bucket delta

  double rstart() const { return rstart_; }
  void setRStart(double r) { rstart_ = r; }

  double rdelta() const { return rdelta_; }
  void setRDelta(double r) { rdelta_ = r; }

  double istart() const { return int(rstart_); }
  void setIStart(int i) { rstart_ = i; }

  double idelta() const { return int(rdelta_); }
  void setIDelta(int i) { rdelta_ = i; }

  //---

  bool isIntegral() const { return integral_; }
  void setIntegral(bool b) { if (b != integral_) { integral_ = b; needsCalc_ = true; } }

  //---

  // auto bucket delta

  double rmin() const { return rmin_; }
  void setRMin(double r) { if (r != rmin_) { rmin_ = r; needsCalc_ = true; } }

  double rmax() const { return rmax_; }
  void setRMax(double r) { if (r != rmax_) { rmax_ = r; needsCalc_ = true; } }

  double imin() const { return int(rmin_); }
  void setIMin(int i) { if (i != rmin_) { rmin_ = i; needsCalc_ = true; } }

  double imax() const { return int(rmax_); }
  void setIMax(int i) { if (i != rmax_) { rmax_ = i; needsCalc_ = true; } }

  int numAuto() const { return numAuto_; }
  void setNumAuto(int i) { if (i != numAuto_) { numAuto_ = i; needsCalc_ = true; } }

  //---

  double calcDelta() const { return calcDelta_; }

  //---

  // get bucket for generic value
  int bucket(const QVariant &var) const;

  bool bucketValues(int bucket, double &min, double &max) const;

  bool bucketIValues(int bucket, int &min, int &max) const;
  bool bucketRValues(int bucket, double &min, double &max) const;

  bool autoBucketValues(int bucket, double &min, double &max) const;

  //----

  static int    varInt (const QVariant &var, bool &ok);
  static double varReal(const QVariant &var, bool &ok);
  //----

  QString bucketName(int bucket, bool utfArrow=false) const;

  QString bucketName(int    imin, int    imax, bool utfArrow=false) const;
  QString bucketName(double rmin, double rmax, bool utfArrow=false) const;

  //----

  void autoCalc() const;

  //----

  int stringBucket(const QString &str) const;

  int intBucket(int i) const;

  int realBucket(double r) const;

  int autoRealBucket(double r) const;

  //----

 private:
  double calcRStart() const;
  int    calcIStart() const;

  int roundNearest(double x) const;

  int roundDown(double x) const;

  double roundNearestF(double x) const;

  double roundDownF(double x) const;

 private:
  using StringInd = std::map<QString,int>;
  using IndString = std::map<int,QString>;

  // data
  Type   type_     { Type::STRING }; // data type
  double rmin_     { 0.0 };          // actual min value
  double rmax_     { 1.0 };          // actual max value
  bool   integral_ { false };        // prefer integral buckets

  // manual
  double rstart_  { 0.0 }; // manual bucktet start value
  double rdelta_  { 1.0 }; // manual bucktet delta value

  // auto bucket number of values
  int numAuto_ { 10 }; // num auto

  // cached data
  mutable bool      needsCalc_ { true }; // needs auto calc
  mutable double    calcMin_   { 0.0 };  // calculated min value
  mutable double    calcMax_   { 1.0 };  // calculated max value
  mutable double    calcDelta_ { 1.0 };  // calculated delta value
  mutable StringInd stringInd_;          // string to ind map
  mutable IndString indString_;          // ind to string map
};

#endif
