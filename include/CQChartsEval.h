#ifndef CQChartsEval_H
#define CQChartsEval_H

#include <CQTclUtil.h>

#define CQChartsEvalInst CQChartsEval::instance()

//! \brief class to evaluate tcl expression
class CQChartsEval {
 public:
  static CQChartsEval *instance();

 ~CQChartsEval();

  bool evalExpr(int row, const QString &exprStr, QVariant &var);

  CQTcl* qtcl() const { return qtcl_; }

 private:
  CQChartsEval();

  void addFunc(const QString &name, CQTcl::ObjCmdProc proc);

 private:
  static int colorCmd(ClientData clientData, Tcl_Interp *, int objc, const Tcl_Obj **objv);

 private:
  CQTcl* qtcl_ { nullptr };
};

#endif
