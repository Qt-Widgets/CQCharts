#ifndef CQTclUtil_H
#define CQTclUtil_H

#include <CTclUtil.h>
#include <cassert>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QPointF>
#include <QRectF>
#include <QPolygonF>

namespace CQTclUtil {

using Vars = std::vector<QVariant>;

inline int eval(Tcl_Interp *interp, const QString &str) {
  return Tcl_EvalEx(interp, str.toLatin1().constData(), -1, 0);
}

inline QString modelIndexToString(const QModelIndex &ind) {
  int row = ind.row   ();
  int col = ind.column();

  return QString("%1:%2").arg(row).arg(col);
}

inline bool stringToModelIndex(const QString &str, int &row, int &col) {
  int pos = str.indexOf(':');

  if (pos < 0)
    return false;

  QString lhs = str.mid(0, pos).simplified();
  QString rhs = str.mid(pos + 1).simplified();

  bool ok1, ok2;

  row = lhs.toInt(&ok1);
  col = rhs.toInt(&ok2);

  return (ok1 && ok2);
}

inline Tcl_Obj *variantToObj(Tcl_Interp *interp, const QVariant &var) {
  if      (var.type() == QVariant::Double) {
    return Tcl_NewDoubleObj(var.value<double>());
  }
  else if (var.type() == QVariant::Int) {
    return Tcl_NewIntObj(var.value<int>());
  }
  else if (var.type() == QVariant::Bool) {
    return Tcl_NewBooleanObj(var.value<bool>());
  }
  else if (var.type() == QVariant::String) {
    QString str = var.toString();

    return Tcl_NewStringObj(str.toLatin1().constData(), -1);
  }
  else if (var.type() == QVariant::PointF) {
    const QPointF &p = var.value<QPointF>();

    QString str = QString("{%1 %2}").arg(p.x()).arg(p.y());

    return Tcl_NewStringObj(str.toLatin1().constData(), -1);
  }
  else if (var.type() == QVariant::RectF) {
    const QRectF &r = var.value<QRectF>();

    QString str = QString("{%1 %2 %3 %4}").
     arg(r.left()).arg(r.bottom()).arg(r.right()).arg(r.top());

    return Tcl_NewStringObj(str.toLatin1().constData(), -1);
  }
  else if (var.type() == QVariant::PolygonF) {
    const QPolygonF &p = var.value<QPolygonF>();

    QString str = "{";

    for (int i = 0; i < p.length(); ++i) {
      if (i > 0) str += " ";

      str + QString("{%1 %2}").arg(p[i].x()).arg(p[i].y());
    }

    str += "}";

    return Tcl_NewStringObj(str.toLatin1().constData(), -1);
  }
  else if (var.type() == QVariant::ModelIndex) {
    QModelIndex ind = var.value<QModelIndex>();

    QString str = modelIndexToString(ind);

    return Tcl_NewStringObj(str.toLatin1().constData(), -1);
  }
  else if (var.type() == QVariant::List) {
    QVariantList vars = var.value<QVariantList>();

    Tcl_Obj *obj = Tcl_NewListObj(0, nullptr);

    int nv = vars.length();

    for (int i = 0; i < nv; ++i) {
      if (! vars[i].isValid())
        continue;

      Tcl_Obj *sobj = variantToObj(interp, vars[i]);

      Tcl_ListObjAppendElement(interp, obj, sobj);
    }

    return obj;
  }
  else {
    QString str = var.toString();
    assert(str.length());

    return Tcl_NewStringObj(str.toLatin1().constData(), -1);
  }
}

inline QVariant variantFromObj(Tcl_Interp *interp, Tcl_Obj *obj) {
  static const Tcl_ObjType *itype;
  static const Tcl_ObjType *rtype;
  static const Tcl_ObjType *stype;
  static const Tcl_ObjType *ltype;

  if (! itype) {
    itype = Tcl_GetObjType("int");
    rtype = Tcl_GetObjType("double");
    stype = Tcl_GetObjType("string");
    ltype = Tcl_GetObjType("list");
  }

  double real    = 0.0;
  int    integer = 0;

  Tcl_IncrRefCount(obj);

  QVariant var;

  const Tcl_ObjType *type = obj->typePtr;

  if (type) {
    if      (type == itype) {
      if (Tcl_GetIntFromObj(interp, obj, &integer) == TCL_OK)
        var = QVariant(integer);
    }
    else if (type == rtype) {
      if (Tcl_GetDoubleFromObj(interp, obj, &real) == TCL_OK)
        var = QVariant(real);
    }
    else if (type == ltype) {
      QList<QVariant> lvars;

      int len = 0;

      if (Tcl_ListObjLength(interp, obj, &len) == TCL_OK) {
        for (int i = 0; i < len; ++i) {
          Tcl_Obj *lobj;

          if (Tcl_ListObjIndex(interp, obj, i, &lobj) == TCL_OK) {
            QVariant lvar = variantFromObj(interp, lobj);

            lvars.push_back(lvar);
          }
        }

        var = lvars;
      }
    }
    else if (type == stype) {
    }
    else {
      //assert(false);
    }
  }

  if (! var.isValid()) {
    int len = 0;

    char *str = Tcl_GetStringFromObj(obj, &len);

    std::string cstr(str, len);

    var = QVariant(QString(cstr.c_str()));
  }

  Tcl_DecrRefCount(obj);

  return var;
}

inline void createVar(Tcl_Interp *interp, const QString &name, const QVariant &var) {
  if (var.isValid()) {
    Tcl_Obj *nameObj  = variantToObj(interp, name);
    Tcl_Obj *valueObj = variantToObj(interp, var );

    Tcl_ObjSetVar2(interp, nameObj, nullptr, valueObj, TCL_GLOBAL_ONLY);

    Tcl_IncrRefCount(nameObj); Tcl_DecrRefCount(nameObj);
  }
}

inline QVariant getVar(Tcl_Interp *interp, const QString &name) {
  Tcl_Obj *nameObj = variantToObj(interp, name);

  Tcl_Obj *obj = Tcl_ObjGetVar2(interp, nameObj, nullptr, TCL_GLOBAL_ONLY);

  Tcl_IncrRefCount(nameObj); Tcl_DecrRefCount(nameObj);

  if (! obj)
    return QVariant();

  return variantFromObj(interp, obj);
}

inline Vars getListVar(Tcl_Interp *interp, const QString &name) {
  Tcl_Obj *obj = Tcl_ObjGetVar2(interp, variantToObj(interp, name), nullptr, TCL_GLOBAL_ONLY);

  if (! obj)
    return Vars();

  int       n = 0;
  Tcl_Obj **objs = nullptr;

  int rc = Tcl_ListObjGetElements(interp, obj, &n, &objs);

  if (rc != TCL_OK)
    return Vars();

  Vars vars;

  for (int i = 0; i < n; ++i) {
    vars.push_back(variantFromObj(interp, objs[i]));
  }

  return vars;
}

inline void setResult(Tcl_Interp *interp, const QVariant &var) {
  if (var.isValid())
    Tcl_SetObjResult(interp, variantToObj(interp, var));
}

inline void setResult(Tcl_Interp *interp, const QVariantList &vars) {
  Tcl_Obj *obj = Tcl_NewListObj(0, nullptr);

  int nv = vars.length();

  for (int i = 0; i < nv; ++i) {
    if (! vars[i].isValid())
      continue;

    Tcl_Obj *sobj = variantToObj(interp, vars[i]);

    Tcl_ListObjAppendElement(interp, obj, sobj);
  }

  Tcl_SetObjResult(interp, obj);
}

inline QVariant getResult(Tcl_Interp *interp) {
  Tcl_Obj *res = Tcl_GetObjResult(interp);

  Tcl_Obj *res1 = Tcl_DuplicateObj(res);

  QVariant var = variantFromObj(interp, res);

  Tcl_SetObjResult(interp, res1);

  return var;
}

}

//---

class CQTcl : public CTcl {
 public:
  using Vars       = std::vector<QVariant>;
  using ObjCmdProc = Tcl_ObjCmdProc *;
  using ObjCmdData = ClientData;

 public:
  CQTcl() { }

  virtual ~CQTcl() { }

  void createVar(const QString &name, const QVariant &var) {
    CQTclUtil::createVar(interp(), name, var);
  }

  QVariant getVar(const QString &name) const {
    return CQTclUtil::getVar(interp(), name);
  }

  Vars getListVar(const QString &name) const {
    return CQTclUtil::getListVar(interp(), name);
  }

  Tcl_Command createExprCommand(const QString &name, ObjCmdProc proc, ObjCmdData data) {
    QString mathName = "tcl::mathfunc::" + name;

    return createObjCommand(mathName, proc, data);
  }

  Tcl_Command createObjCommand(const QString &name, ObjCmdProc proc, ObjCmdData data) {
    return Tcl_CreateObjCommand(interp(), (char *) name.toLatin1().constData(),
                                proc, data, nullptr);
  }

  int createAlias(const QString &newName, const QString &oldName) {
    return Tcl_CreateAlias(interp(), newName.toLatin1().constData(),
                           interp(), oldName.toLatin1().constData(),
                           0, nullptr);
  }

  bool evalExpr(const QString &expr, QVariant &res, bool showError=false) {
    return eval("expr {" + expr + "}", res, showError);
  }

  int evalExpr(const QString &expr, bool showError=false) {
    return eval("expr {" + expr + "}", showError);
  }

  bool eval(const QString &cmd, QVariant &res, bool showError=false) {
    int rc = eval(cmd, showError);

    if (rc != TCL_OK)
      return false;

    res = getResult();

    return true;
  }

  int eval(const QString &cmd, bool showError=false, bool showResult=false) {
    int rc = CQTclUtil::eval(interp(), cmd);

    if (rc != TCL_OK) {
      if (showError)
        std::cerr << errorInfo(rc).toStdString() << std::endl;
    }

    if (showResult) {
      QVariant res = getResult();

      if (res.isValid()) {
        QString resStr = resToString(res);

        if (resStr.length())
          std::cout << resStr.toStdString() << "\n";
      }
    }

    return rc;
  }

  QString resToString(const QVariant &res) {
    if (res.type() == QVariant::List) {
      QList<QVariant> vars = res.toList();

      QStringList strs;

      for (int i = 0; i < vars.length(); ++i) {
        QString str = resToString(vars[i]);

        strs.push_back(str);
      }

      return "{" + strs.join(" ") + "}";
    }
    else
      return res.toString();
  }

  QVariant variantFromObj(Tcl_Obj *obj) {
    return CQTclUtil::variantFromObj(interp(), obj);
  }

  void traceVar(const QString &name) {
    int flags = TCL_TRACE_READS | TCL_TRACE_WRITES | TCL_TRACE_UNSETS | TCL_GLOBAL_ONLY;

    ClientData data =
      Tcl_VarTraceInfo(interp(), name.toLatin1().constData(), flags, &CQTcl::traceProc, 0);

    if (! data)
      Tcl_TraceVar(interp(), name.toLatin1().constData(), flags,
        &CQTcl::traceProc, (ClientData) this);
  }

  virtual void handleTrace(const char *name, int flags) {
    // ignore unset called on trace destruction
    if (flags & TCL_TRACE_UNSETS) return;

    std::cerr << "CQTcl::handleTrace (";
    if (flags & TCL_TRACE_READS ) std::cerr << " read" ;
    if (flags & TCL_TRACE_WRITES) std::cerr << " write";
  //if (flags & TCL_TRACE_UNSETS) std::cerr << " unset";
    std::cerr << ") " << name << "\n";
  }

  void setResult(const QVariant &rc) {
    CQTclUtil::setResult(interp(), rc);
  }

  void setResult(const QVariantList &rc) {
    CQTclUtil::setResult(interp(), rc);
  }

  QVariant getResult() const {
    return CQTclUtil::getResult(interp());
  }

  QString errorInfo(int rc) const {
    return CTclUtil::errorInfo(interp(), rc).c_str();
  }

 private:
  static char *traceProc(ClientData data, Tcl_Interp *, const char *name1,
                         const char *, int flags) {
    CQTcl *th = static_cast<CQTcl *>(data);
    assert(th);

    th->handleTrace(name1, flags);

    return 0;
  }
};

#endif
