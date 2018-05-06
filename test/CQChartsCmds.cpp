#include <CQChartsCmds.h>
#include <CQChartsModelData.h>
#include <CQChartsExpr.h>
#include <CQCharts.h>
#include <CQChartsWindow.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQChartsAxis.h>
#include <CQChartsAnnotation.h>
#include <CQChartsLength.h>
#include <CQChartsPosition.h>
#include <CQChartsColor.h>
#include <CQChartsLineDash.h>
#include <CQChartsPaletteColorData.h>
#include <CQChartsCsv.h>
#include <CQChartsTsv.h>
#include <CQChartsJson.h>
#include <CQChartsGnuData.h>
#include <CQChartsExprModel.h>
#include <CQChartsDataModel.h>
#include <CQChartsUtil.h>

#include <CQChartsTree.h>
#include <CQChartsTable.h>
#include <CQDataModel.h>
#include <CQFoldedModel.h>
#include <CQSortModel.h>

#include <CQUtil.h>
#include <CUnixFile.h>
#include <CHRTimer.h>

#ifdef CQ_CHARTS_CEIL
#include <CCeil.h>
#endif

#include <QStackedWidget>
#include <QSortFilterProxyModel>
#include <QFont>

//----

namespace {

bool stringToBool(const QString &str, bool *ok) {
  QString lstr = str.toLower();

  if (lstr == "0" || lstr == "false" || lstr == "no") {
    *ok = true;
    return false;
  }

  if (lstr == "1" || lstr == "true" || lstr == "yes") {
    *ok = true;
    return true;
  }

  *ok = false;

  return false;
}

void
errorMsg(const QString &msg)
{
  std::cerr << msg.toStdString() << "\n";
}

}

//----

CQChartsCmds::
CQChartsCmds(CQCharts *charts) :
 charts_(charts)
{
  expr_ = new CExpr;
}

CQChartsCmds::
~CQChartsCmds()
{
  for (auto &modelData : modelDatas_)
    delete modelData;

  delete expr_;
}

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
setCeil(bool b)
{
  if (b) {
    //ClParserInst->setDollarPrefix(true);

    ClLanguageMgrInst->init(nullptr, nullptr);

    ClLanguageMgrInst->defineCommand("load"   , CQChartsCmds::loadLCmd   , this);
    ClLanguageMgrInst->defineCommand("process", CQChartsCmds::processLCmd, this);
    ClLanguageMgrInst->defineCommand("sort"   , CQChartsCmds::sortLCmd   , this);
    ClLanguageMgrInst->defineCommand("group"  , CQChartsCmds::groupLCmd  , this);
    ClLanguageMgrInst->defineCommand("place"  , CQChartsCmds::placeLCmd  , this);

    // add/remove plot
    ClLanguageMgrInst->defineCommand("add_plot"   , CQChartsCmds::addPlotLCmd   , this);
    ClLanguageMgrInst->defineCommand("remove_plot", CQChartsCmds::removePlotLCmd, this);

    // get/set model
    ClLanguageMgrInst->defineCommand("set_model", CQChartsCmds::setModelLCmd, this);
    ClLanguageMgrInst->defineCommand("get_model", CQChartsCmds::getModelLCmd, this);

    // get/set view
    ClLanguageMgrInst->defineCommand("get_view", CQChartsCmds::getViewLCmd, this);
    ClLanguageMgrInst->defineCommand("set_view", CQChartsCmds::getViewLCmd, this);

    // get/set property
    ClLanguageMgrInst->defineCommand("set_property", CQChartsCmds::setPropertyLCmd, this);
    ClLanguageMgrInst->defineCommand("get_property", CQChartsCmds::getPropertyLCmd, this);

    // get/set data
    ClLanguageMgrInst->defineCommand("set_data", CQChartsCmds::setDataLCmd, this);
    ClLanguageMgrInst->defineCommand("get_data", CQChartsCmds::getDataLCmd, this);

    // annotations
    ClLanguageMgrInst->defineCommand("text_shape"    , CQChartsCmds::textShapeLCmd    , this);
    ClLanguageMgrInst->defineCommand("arrow_shape"   , CQChartsCmds::arrowShapeLCmd   , this);
    ClLanguageMgrInst->defineCommand("rect_shape"    , CQChartsCmds::rectShapeLCmd    , this);
    ClLanguageMgrInst->defineCommand("ellipse_shape" , CQChartsCmds::ellipseShapeLCmd , this);
    ClLanguageMgrInst->defineCommand("polygon_shape" , CQChartsCmds::polygonShapeLCmd , this);
    ClLanguageMgrInst->defineCommand("polyline_shape", CQChartsCmds::polylineShapeLCmd, this);
    ClLanguageMgrInst->defineCommand("point_shape"   , CQChartsCmds::pointShapeLCmd   , this);

    // theme/palette
    ClLanguageMgrInst->defineCommand("theme"  , CQChartsCmds::themeLCmd  , this);
    ClLanguageMgrInst->defineCommand("palette", CQChartsCmds::paletteLCmd, this);

    // connect
    ClLanguageMgrInst->defineCommand("connect", CQChartsCmds::connectLCmd, this);

    ceil_ = b;
  }
}
#endif

bool
CQChartsCmds::
processCmd(const QString &cmd, const Args &args)
{
  if      (cmd == "load"   ) { loadCmd   (args); }
  else if (cmd == "process") { processCmd(args); }
  else if (cmd == "sort"   ) { sortCmd   (args); }
  else if (cmd == "group"  ) { groupCmd  (args); }
  else if (cmd == "place"  ) { placeCmd  (args); }

  // get/set model
  else if (cmd == "set_model") { setModelCmd(args); }
  else if (cmd == "get_model") { getModelCmd(args); }

  // get/set view
  else if (cmd == "set_view") { setViewCmd(args); }
  else if (cmd == "get_view") { getViewCmd(args); }

  else if (cmd == "add_plot"   ) { addPlotCmd   (args); }
  else if (cmd == "remove_plot") { removePlotCmd(args); }

  // get/set property
  else if (cmd == "set_property") { setPropertyCmd(args); }
  else if (cmd == "get_property") { getPropertyCmd(args); }

  // get/set data
  else if (cmd == "set_data") { setDataCmd(args); }
  else if (cmd == "get_data") { getDataCmd(args); }

  // annotations
  else if (cmd == "text_shape"    ) { textShapeCmd    (args); }
  else if (cmd == "arrow_shape"   ) { arrowShapeCmd   (args); }
  else if (cmd == "rect_shape"    ) { rectShapeCmd    (args); }
  else if (cmd == "ellipse_shape" ) { ellipseShapeCmd (args); }
  else if (cmd == "polygon_shape" ) { polygonShapeCmd (args); }
  else if (cmd == "polyline_shape") { polylineShapeCmd(args); }
  else if (cmd == "point_shape"   ) { pointShapeCmd   (args); }

  // them/palette
  else if (cmd == "theme"  ) { themeCmd  (args); }
  else if (cmd == "palette") { paletteCmd(args); }

  // connect
  else if (cmd == "connect") { connectCmd(args); }

  // control
  else if (cmd == "@let"     ) { letCmd     (args); }
  else if (cmd == "@if"      ) { ifCmd      (args); }
  else if (cmd == "@while"   ) { whileCmd   (args); }
  else if (cmd == "@continue") { continueCmd(args); }
  else if (cmd == "@print"   ) { printCmd   (args); }

  else if (cmd == "source") { sourceCmd(args); }
  else if (cmd == "exit"  ) { exit(0); }

  else return false;

  return true;
}

//------

class CQChartsCmdsArgs {
 public:
  using Args = std::vector<QString>;

  class Arg {
   public:
    Arg(const QString &arg="") :
     arg_(arg) {
      isOpt_ = (arg_.length() && arg_[0] == '-');
    }

    QString str() const { assert(! isOpt_); return arg_; }

    bool isOpt() const { return isOpt_; }

    QString opt() const { assert(isOpt_); return arg_.mid(1); }

   private:
    QString arg_;
    bool    isOpt_;
  };

 public:
  CQChartsCmdsArgs(const Args &argv) :
   argv_(argv), argc_(argv_.size()) {
  }

  bool eof() const { return (i_ >= argc_); }

  const Arg &getArg() {
    assert(i_ < argc_);

    lastArg_ = Arg(argv_[i_++]);

    return lastArg_;
  }

  bool getOptValue(QString &str) {
    if (eof()) return false;

    str = argv_[i_++];

    return true;
  }

  bool getOptValue(int &i) {
    QString str;

    if (! getOptValue(str))
      return false;

    bool ok;

    i = str.toInt(&ok);

    return ok;
  }

  bool getOptValue(double &r) {
    QString str;

    if (! getOptValue(str))
      return false;

    bool ok;

    r = str.toDouble(&ok);

    return ok;
  }

  bool getOptValue(bool &b) {
    QString str;

    if (! getOptValue(str))
      return false;

    str = str.toLower();

    if      (str == "0" || str == "no"  || str == "false" || str == "off") {
      b = false; return true;
    }
    else if (str == "1" || str == "yes" || str == "true"  || str == "on" ) {
      b = true; return true;
    }

    return false;
  }

  bool getOptValue(CQChartsLength &l) {
    QString str;

    if (! getOptValue(str))
      return false;

    l = CQChartsLength(str);

    return true;
  }

  bool getOptValue(CQChartsPosition &p) {
    QString str;

    if (! getOptValue(str))
      return false;

    p = CQChartsPosition(str);

    return true;
  }

  bool getOptValue(QFont &f) {
    QString str;

    if (! getOptValue(str))
      return false;

    f = QFont(str);

    return true;
  }

  bool getOptValue(CQChartsColor &c) {
    QString str;

    if (! getOptValue(str))
      return false;

    c = CQChartsColor(str);

    return true;
  }

  bool getOptValue(CQChartsLineDash &d) {
    QString str;

    if (! getOptValue(str))
      return false;

    d = CQChartsLineDash(str);

    return true;
  }

  bool getOptValue(QPolygonF &poly) {
    QString str;

    if (! getOptValue(str))
      return false;

    CQStrParse parse(str);

    while (! parse.eof()) {
      parse.skipSpace();

      QString xstr;

      if (! parse.readNonSpace(xstr))
        break;

      parse.skipSpace();

      QString ystr;

      if (! parse.readNonSpace(ystr))
        break;

      parse.skipSpace();

      double x, y;

      if (! CQChartsUtil::toReal(xstr, x))
        break;

      if (! CQChartsUtil::toReal(ystr, y))
        break;

      QPointF p(x, y);

      poly << p;
    }

    return poly.length();
  }

  void error() {
    if (lastArg_.isOpt())
      errorMsg("Invalid option '" + lastArg_.opt() + "'");
    else
      errorMsg("Invalid arg '" + lastArg_.str() + "'");
  }

 private:
  Args argv_;
  int  i_    { 0 };
  int  argc_ { 0 };
  Arg  lastArg_;
};

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
loadLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->loadCmd(args);
}
#endif

bool
CQChartsCmds::
loadCmd(const Args &args)
{
  QString           filename;
  CQChartsFileType  fileType { CQChartsFileType::NONE };
  CQChartsInputData inputData;
  QString           title;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      // input data type
      if      (opt == "csv" ) fileType = CQChartsFileType::CSV;
      else if (opt == "tsv" ) fileType = CQChartsFileType::TSV;
      else if (opt == "json") fileType = CQChartsFileType::JSON;
      else if (opt == "data") fileType = CQChartsFileType::DATA;
      else if (opt == "expr") fileType = CQChartsFileType::EXPR;
      else if (opt == "var" ) {
        QString str;

        if (argv.getOptValue(str))
          inputData.vars.push_back(str);

        fileType = CQChartsFileType::VARS;
      }

      // input data control
      else if (opt == "comment_header"     ) { inputData.commentHeader = true; }
      else if (opt == "first_line_header"  ) { inputData.firstLineHeader = true; }
      else if (opt == "first_column_header") { inputData.firstColumnHeader = true; }
      else if (opt == "num_rows") {
        int i;

        if (argv.getOptValue(i))
          inputData.numRows = std::max(i, 1);
      }
      else if (opt == "filter") { (void) argv.getOptValue(inputData.filter); }
      else if (opt == "title" ) { (void) argv.getOptValue(title); }
      else                      { argv.error(); }
    }
    else {
      if (filename == "")
        filename = arg.str();
    }
  }

  if (fileType == CQChartsFileType::NONE) {
    errorMsg("No file type");
    return false;
  }

  if (fileType != CQChartsFileType::EXPR && fileType != CQChartsFileType::VARS) {
    if (filename == "") {
      errorMsg("No filename");
      return false;
    }
  }
  else {
    if (filename != "") {
      errorMsg("Extra filename");
      return false;
    }
  }

  if (! loadFileModel(filename, fileType, inputData))
    return false;

  CQChartsModelData *modelData = currentModelData();

  if (! modelData)
    return false;

  if (title.length())
    emit titleChanged(modelData->ind, title);

  setCmdRc(modelData->ind);

  return true;
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
setModelLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->setModelCmd(args);
}
#endif

void
CQChartsCmds::
setModelCmd(const Args &args)
{
  int     ind = -1;
  QString columnType;
  QString processExpr;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "ind"        ) { (void) argv.getOptValue(ind); }
      else if (opt == "column_type") { (void) argv.getOptValue(columnType); }
      else if (opt == "process"    ) { (void) argv.getOptValue(processExpr); }
      else                           { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsModelData *modelData = getModelDataOrCurrent(ind);

  if (! modelData) {
    errorMsg("No model");
    return;
  }

  ModelP model = modelData->model;

  if (columnType != "") {
    setColumnFormats(model, columnType);

    emit updateModelDetails(modelData->ind);

    //test_->updateModelDetails(modelData);
  }

  if (processExpr != "")
    processExpression(model, processExpr);
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
getModelLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->getModelCmd(args);
}
#endif

void
CQChartsCmds::
getModelCmd(const Args &args)
{
  int     ind    = -1;
  int     column = -1;
  QString name;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "ind"   ) { (void) argv.getOptValue(ind); }
      else if (opt == "column") { (void) argv.getOptValue(column); }
      else if (opt == "name"  ) { (void) argv.getOptValue(name); }
      else                      { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsModelData *modelData = getModelDataOrCurrent(ind);

  if (! modelData) {
    errorMsg("No model");
    return;
  }

  //---

  CQChartsModelDetails details;

  details.update(charts_, modelData->model.data());

  if      (name == "num_rows")
    setCmdRc(details.numRows());
  else if (name == "num_columns")
    setCmdRc(details.numColumns());
  else if (name == "min") {
    if (column >= 0 && column < details.numColumns()) {
      CQChartsModelColumnDetails &columnDetails = details.columnDetails(column);

      setCmdRc(columnDetails.minValue());
    }
  }
  else if (name == "max") {
    if (column >= 0 && column < details.numColumns()) {
      CQChartsModelColumnDetails &columnDetails = details.columnDetails(column);

      setCmdRc(columnDetails.maxValue());
    }
  }
  else
    setCmdRc(QString());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
processLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->processCmd(args);
}
#endif

void
CQChartsCmds::
processCmd(const Args &args)
{
  CQExprModel::Function function = CQExprModel::Function::EVAL;
  QString               header;
  int                   column   = -1;
  QString               expr;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "add"   ) { function = CQExprModel::Function::ADD; }
      else if (opt == "delete") { function = CQExprModel::Function::DELETE; }
      else if (opt == "modify") { function = CQExprModel::Function::ASSIGN; }
      else if (opt == "header") { (void) argv.getOptValue(header); }
      else if (opt == "column") { (void) argv.getOptValue(column); }
      else                        argv.error();
    }
    else {
      if (expr == "")
        expr = arg.str();
      else
        argv.error();
    }
  }

  //---

  CQChartsModelData *modelData = currentModelData();

  if (! modelData)
    return;

  ModelP model = modelData->model;

  CQExprModel *exprModel = getExprModel(model);

  if (! exprModel) {
    errorMsg("Expression not supported for model");
    return;
  }

  if      (function  == CQExprModel::Function::ADD) {
    if (! exprModel->addExtraColumn(header, expr)) {
      errorMsg("Failed to add column");
      return;
    }
  }
  else if (function  == CQExprModel::Function::DELETE) {
    if (! exprModel->removeExtraColumn(column)) {
      errorMsg("Failed to delete column");
      return;
    }
  }
  else if (function  == CQExprModel::Function::ASSIGN) {
    if (! exprModel->assignExtraColumn(header, column, expr)) {
      errorMsg("Failed to modify column");
      return;
    }
  }
  else {
    processExpression(model, expr);
  }
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
setViewLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->setViewCmd(args);
}
#endif

void
CQChartsCmds::
setViewCmd(const Args &args)
{
  QString viewName;
  QString title;
  QString properties;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view"      ) { (void) argv.getOptValue(viewName); }
      else if (opt == "title"     ) { (void) argv.getOptValue(title); }
      else if (opt == "properties") { (void) argv.getOptValue(properties); }
      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  //---

  if (title.length())
    view->setTitle(title);

  if (properties.length())
    setViewProperties(view, properties);

  //---

  setCmdRc(view->id());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
getViewLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->getViewCmd(args);
}
#endif

void
CQChartsCmds::
getViewCmd(const Args &args)
{
  QString viewName;
  bool    current = false;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view"   ) { (void) argv.getOptValue(viewName); }
      else if (opt == "current") { current = true; }
      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  if (current) {
    if (view_)
      setCmdRc(view_->id());
    else
      setCmdRc(QString());

    return;
  }
  else {
    setCmdRc(QString());
  }
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
addPlotLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->addPlotCmd(args);
}
#endif

void
CQChartsCmds::
addPlotCmd(const Args &args)
{
  QString               typeName;
  QString               filterStr;
  CQChartsNameValueData nameValueData;
  QString               columnType;
  bool                  xintegral { false };
  bool                  yintegral { false };
  bool                  xlog { false };
  bool                  ylog { false };
  QString               title;
  QString               properties;
  QString               positionStr;
  OptReal               xmin, ymin, xmax, ymax;

  int argc = args.size();

  for (int i = 0; i < argc; ++i) {
    QString arg = args[i];

    if (arg[0] == '-') {
      QString opt = arg.mid(1);

      // plot type
      if      (opt == "type") {
        ++i;

        if (i < argc)
          typeName = args[i];
      }
      // plot filter
      else if (opt == "where") {
        ++i;

        if (i < argc)
          filterStr = args[i];
      }
      // plot columns
      else if (opt == "column" || opt == "columns") {
        ++i;

        if (i < argc) {
          QString columnsStr = args[i];

          QStringList strs = columnsStr.split(",", QString::SkipEmptyParts);

          for (int j = 0; j < strs.size(); ++j) {
            const QString &nameValue = strs[j];

            auto pos = nameValue.indexOf('=');

            if (pos >= 0) {
              auto name  = nameValue.mid(0, pos).simplified();
              auto value = nameValue.mid(pos + 1).simplified();

              nameValueData.values[name] = value;
            }
            else {
              errorMsg("Invalid " + opt + " option '" + args[i] + "'");
            }
          }
        }
      }
      // plot bool parameters
      else if (opt == "bool") {
        ++i;

        if (i < argc) {
          QString nameValue = args[i];

          auto pos = nameValue.indexOf('=');

          QString name, value;

          if (pos >= 0) {
            name  = nameValue.mid(0, pos).simplified();
            value = nameValue.mid(pos + 1).simplified();
          }
          else {
            name  = nameValue;
            value = "true";
          }

          bool ok;

          bool b = stringToBool(value, &ok);

          if (ok)
            nameValueData.bools[name] = b;
          else {
            errorMsg("Invalid -bool option '" + args[i] + "'");
          }
        }
      }
      // plot string parameters
      else if (opt == "string") {
        ++i;

        if (i < argc) {
          QString nameValue = args[i];

          auto pos = nameValue.indexOf('=');

          QString name, value;

          if (pos >= 0) {
            name  = nameValue.mid(0, pos).simplified();
            value = nameValue.mid(pos + 1).simplified();
          }
          else {
            name  = nameValue;
            value = "";
          }

          nameValueData.strings[name] = value;
        }
      }
      // plot real parameters
      else if (opt == "real") {
        ++i;

        if (i < argc) {
          QString nameValue = args[i];

          auto pos = nameValue.indexOf('=');

          QString name;
          double  value = 0.0;

          if (pos >= 0) {
            bool ok;

            name  = nameValue.mid(0, pos).simplified();
            value = nameValue.mid(pos + 1).simplified().toDouble(&ok);
          }
          else {
            name  = nameValue;
            value = 0.0;
          }

          nameValueData.reals[name] = value;
        }
      }

      // column types
      else if (opt == "column_type") {
        ++i;

        if (i < argc)
          columnType = args[i];
      }

      // axis type
      else if (opt == "xintegral") {
        xintegral = true;
      }
      else if (opt == "yintegral") {
        yintegral = true;
      }

      // log scale
      else if (opt == "xlog") {
        xlog = true;
      }
      else if (opt == "ylog") {
        ylog = true;
      }

      // title
      else if (opt == "title") {
        ++i;

        if (i < argc)
          title = args[i];
      }

      // plot properties
      else if (opt == "properties") {
        ++i;

        if (i < argc)
          properties = args[i];
      }

      // position
      else if (opt == "position") {
        ++i;

        if (i < argc)
          positionStr = args[i];
      }

      // data range
      else if (opt == "xmin") {
        ++i;

        if (i < argc)
          xmin = args[i].toDouble();
      }
      else if (opt == "xmax") {
        ++i;

        if (i < argc)
          xmax = args[i].toDouble();
      }
      else if (opt == "ymin") {
        ++i;

        if (i < argc)
          ymin = args[i].toDouble();
      }
      else if (opt == "ymax") {
        ++i;

        if (i < argc)
          ymax = args[i].toDouble();
      }

      else {
        errorMsg("Invalid option '" + opt + "'");
      }
    }
    else {
      errorMsg("Invalid arg '" + arg + "'");
    }
  }

  //------

  CQChartsModelData *modelData = currentModelData();

  if (! modelData) {
    errorMsg("No model data");
    return;
  }

  ModelP model = modelData->model;

  if (columnType != "") {
    setColumnFormats(model, columnType);

    emit updateModelDetails(modelData->ind);

    //test_->updateModelDetails(modelData);
  }

  //------

  typeName = fixTypeName(typeName);

  if (typeName == "")
    return;

  // ignore if bad type
  CQChartsPlotType *type = charts_->plotType(typeName);

  if (! type) {
    errorMsg("Invalid type '" + typeName + "' for plot");
    return;
  }

  //------

  double vr = CQChartsView::viewportRange();

  CQChartsGeom::BBox bbox(0, 0, vr, vr);

  if (positionStr != "") {
    QStringList positionStrs = positionStr.split(" ", QString::SkipEmptyParts);

    if (positionStrs.length() == 4) {
      bool ok1, ok2, ok3, ok4;

      double pxmin = positionStrs[0].toDouble(&ok1);
      double pymin = positionStrs[1].toDouble(&ok2);
      double pxmax = positionStrs[2].toDouble(&ok3);
      double pymax = positionStrs[3].toDouble(&ok4);

      if (ok1 && ok2 && ok3 && ok4) {
        bbox = CQChartsGeom::BBox(pxmin, pymin, pxmax, pymax);
      }
      else
        errorMsg("Invalid position '" + positionStr + "'");
    }
    else {
      errorMsg("Invalid position '" + positionStr + "'");
    }
  }

  //------

  // create plot from init (argument) data
  CQChartsPlot *plot = createPlot(model, modelData->sm, type, nameValueData, true, bbox);
  assert(plot);

  //------

  // init plot
  if (title != "")
    plot->setTitleStr(title);

  if (xlog)
    plot->setLogX(true);

  if (ylog)
    plot->setLogY(true);

  if (xintegral)
    plot->xAxis()->setIntegral(true);

  if (yintegral)
    plot->yAxis()->setIntegral(true);

  if (xmin) plot->setXMin(xmin);
  if (ymin) plot->setYMin(ymin);
  if (xmax) plot->setXMax(xmax);
  if (ymax) plot->setYMax(ymax);

  //---

  if (properties != "")
    setPlotProperties(plot, properties);

  //---

  setCmdRc(plot->id());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
removePlotLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->removePlotCmd(args);
}
#endif

void
CQChartsCmds::
removePlotCmd(const Args &args)
{
  QString viewName;
  QString plotName;
  bool    all = false;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }
      else if (opt == "all" ) { all = true; }
      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  if (all) {
    view_->removeAllPlots();
  }
  else {
    CQChartsPlot *plot = getPlotByName(view, plotName);
    if (! plot) return;

    view_->removePlot(plot);
  }
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
getPropertyLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->getPropertyCmd(args);
}
#endif

void
CQChartsCmds::
getPropertyCmd(const Args &args)
{
  QString modelId;
  QString viewName;
  QString plotName;
  QString annotationName;
  QString name;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "model"     ) { (void) argv.getOptValue(modelId); }
      else if (opt == "view"      ) { (void) argv.getOptValue(viewName); }
      else if (opt == "plot"      ) { (void) argv.getOptValue(plotName); }
      else if (opt == "annotation") { (void) argv.getOptValue(annotationName); }
      else if (opt == "name"      ) { (void) argv.getOptValue(name); }
      else                          { argv.error(); }
    }
    else { argv.error(); }
  }

  if (modelId != "") {
    bool ok;

    int ind = modelId.toInt(&ok);

    CQChartsModelData *modelData = getModelData(ind);

    if (modelData) {
      CQChartsModelDetails details;

      details.update(charts_, modelData->model.data());

      if      (name == "num_rows")
        setCmdRc(details.numRows());
      else if (name == "num_columns")
        setCmdRc(details.numColumns());
    }
  }
  else {
    CQChartsView *view = getViewByName(viewName);
    if (! view) return;

    //---

    QVariant value;

    if (plotName != "") {
      CQChartsPlot *plot = getPlotByName(view, plotName);
      if (! plot) return;

      if (annotationName != "") {
        CQChartsAnnotation *annotation = plot->getAnnotationByName(annotationName);
        if (! annotation) return;

        if (! annotation->getProperty(name, value)) {
          errorMsg("Failed to get annotation parameter '" + name + "'");
          return;
        }
      }
      else {
        if (! plot->getProperty(name, value)) {
          errorMsg("Failed to get plot parameter '" + name + "'");
          return;
        }
      }
    }
    else {
      if (annotationName != "") {
        CQChartsAnnotation *annotation = view->getAnnotationByName(annotationName);
        if (! annotation) return;

        if (! annotation->getProperty(name, value)) {
          errorMsg("Failed to get annotation parameter '" + name + "'");
          return;
        }
      }
      else {
        if (! view->getProperty(name, value)) {
          errorMsg("Failed to get view parameter '" + name + "'");
          return;
        }
      }
    }

    bool rc;

    setCmdRc(CQChartsUtil::toString(value, rc));
  }
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
setPropertyLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->setPropertyCmd(args);
}
#endif

void
CQChartsCmds::
setPropertyCmd(const Args &args)
{
  QString viewName;
  QString plotName;
  QString annotationName;
  QString name;
  QString value;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view"      ) { (void) argv.getOptValue(viewName); }
      else if (opt == "plot"      ) { (void) argv.getOptValue(plotName); }
      else if (opt == "annotation") { (void) argv.getOptValue(annotationName); }
      else if (opt == "name"      ) { (void) argv.getOptValue(name); }
      else if (opt == "value"     ) { (void) argv.getOptValue(value); }
      else                          { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  //---

  if (plotName != "") {
    CQChartsPlot *plot = getPlotByName(view, plotName);
    if (! plot) return;

    if (annotationName != "") {
      CQChartsAnnotation *annotation = plot->getAnnotationByName(annotationName);
      if (! annotation) return;

      if (! annotation->setProperty(name, value)) {
        errorMsg("Failed to set annotation parameter '" + name + "'");
        return;
      }
    }
    else {
      if (! plot->setProperty(name, value)) {
        errorMsg("Failed to set plot parameter '" + name + "'");
        return;
      }
    }
  }
  else {
    if (! view->setProperty(name, value)) {
      errorMsg("Failed to set view parameter '" + name + "'");
      return;
    }
  }
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
themeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->themeCmd(args);
}
#endif

void
CQChartsCmds::
themeCmd(const Args &args)
{
  QString                  viewName;
  CQChartsPaletteColorData paletteData;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") {
        (void) argv.getOptValue(viewName);
      }
      else if (opt == "color_type") {
        (void) argv.getOptValue(paletteData.colorTypeStr);
      }
      else if (opt == "color_model") {
        (void) argv.getOptValue(paletteData.colorModelStr);
      }
      else if (opt == "redModel" || opt == "greenModel" || opt == "blueModel") {
        int model;

        if (argv.getOptValue(model)) {
          if      (opt == "redModel"  ) paletteData.redModel   = model;
          else if (opt == "greenModel") paletteData.greenModel = model;
          else if (opt == "blueModel" ) paletteData.blueModel  = model;
        }
      }
      else if (opt == "negateRed" || opt == "negateGreen" || opt == "negateBlue") {
        bool b;

        if (argv.getOptValue(b)) {
          if      (opt == "negateRed"  ) paletteData.negateRed   = b;
          else if (opt == "negateGreen") paletteData.negateGreen = b;
          else if (opt == "negateBlue" ) paletteData.negateBlue  = b;
        }
      }
      else if (opt == "redMin"   || opt == "redMax"   ||
               opt == "greenMin" || opt == "greenMax" ||
               opt == "blueMin"  || opt == "blueMax") {
        double r;

        if (argv.getOptValue(r)) {
          if      (opt == "redMin"  ) paletteData.redMin   = r;
          else if (opt == "greenMin") paletteData.greenMin = r;
          else if (opt == "blueMin" ) paletteData.blueMin  = r;
          else if (opt == "redMax"  ) paletteData.redMax   = r;
          else if (opt == "greenMax") paletteData.greenMax = r;
          else if (opt == "blueMax" ) paletteData.blueMax  = r;
        }
      }
      else if (opt == "defined") {
        QString str;

        if (argv.getOptValue(str)) {
          QStringList strs = str.split(" ", QString::SkipEmptyParts);

          if (! strs.length())
            continue;

          double dv = (strs.length() > 1 ? 1.0/(strs.length() - 1) : 0.0);

          paletteData.definedColors.clear();

          for (int j = 0; j < strs.length(); ++j) {
            int pos = strs[j].indexOf('=');

            double v = j*dv;
            QColor c;

            if (pos > 0) {
              QString lhs = strs[j].mid(0, pos).simplified();
              QString rhs = strs[j].mid(pos + 1).simplified();

              bool ok;

              v = lhs.toDouble(&ok);
              c = QColor(rhs);
            }
            else
              c = QColor(strs[j]);

            paletteData.definedColors.push_back(CQChartsDefinedColor(v, c));
          }
        }
      }
      else if (opt == "get_color") {
        double r;

        if (argv.getOptValue(r)) {
          paletteData.getColorFlag  = true;
          paletteData.getColorValue = r;
        }
      }
      else if (opt == "get_color_scale") {
        paletteData.getColorScale = true;
      }
      else {
        argv.error();
      }
    }
    else {
      argv.error();
    }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  //---

  CQChartsGradientPalette *theme = view->theme()->theme();

  setPaleteData(theme, paletteData);

  //---

  view->updatePlots();

  CQChartsWindow *window = CQChartsWindowMgrInst->getWindowForView(view);

  if (window)
    window->updatePalette();
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
paletteLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->paletteCmd(args);
}
#endif

void
CQChartsCmds::
paletteCmd(const Args &args)
{
  QString                  viewName;
  CQChartsPaletteColorData paletteData;

  int argc = args.size();

  for (int i = 0; i < argc; ++i) {
    QString arg = args[i];

    if (arg[0] == '-') {
      QString opt = arg.mid(1);

      if      (opt == "view") {
        ++i;

        if (i < argc)
          viewName = args[i];
      }
      else if (opt == "color_type") {
        ++i;

        if (i >= argc) {
          errorMsg("Missing color type");
          continue;
        }

        paletteData.colorTypeStr = args[i];
      }
      else if (opt == "color_model") {
        ++i;

        if (i >= argc) {
          errorMsg("Missing color model");
          continue;
        }

        paletteData.colorModelStr = args[i];
      }
      else if (opt == "redModel" || opt == "greenModel" || opt == "blueModel") {
        ++i;

        if (i >= argc) {
          errorMsg("Missing model number");
          continue;
        }

        bool ok;

        int model = args[i].toInt(&ok);

        if (! ok) {
          errorMsg("Invalid model number");
          continue;
        }

        if      (opt == "redModel"  ) paletteData.redModel   = model;
        else if (opt == "greenModel") paletteData.greenModel = model;
        else if (opt == "blueModel" ) paletteData.blueModel  = model;
      }
      else if (opt == "negateRed" || opt == "negateGreen" || opt == "negateBlue") {
        ++i;

        if (i >= argc) {
          errorMsg("Missing model number");
          continue;
        }

        bool ok;

        bool b = stringToBool(args[i], &ok);

        if (! ok) {
          errorMsg("Invalid negate bool");
          continue;
        }

        if      (opt == "negateRed"  ) paletteData.negateRed   = b;
        else if (opt == "negateGreen") paletteData.negateGreen = b;
        else if (opt == "negateBlue" ) paletteData.negateBlue  = b;
      }
      else if (opt == "redMin"   || opt == "redMax"   ||
               opt == "greenMin" || opt == "greenMax" ||
               opt == "blueMin"  || opt == "blueMax") {
        ++i;

        if (i >= argc) {
          errorMsg("Missing min/max value");
          continue;
        }

        bool ok;

        double r = args[i].toDouble(&ok);

        if (! ok) {
          errorMsg("Invalid min/max value");
          continue;
        }

        if      (opt == "redMin"  ) paletteData.redMin   = r;
        else if (opt == "greenMin") paletteData.greenMin = r;
        else if (opt == "blueMin" ) paletteData.blueMin  = r;
        else if (opt == "redMax"  ) paletteData.redMax   = r;
        else if (opt == "greenMax") paletteData.greenMax = r;
        else if (opt == "blueMax" ) paletteData.blueMax  = r;
      }
      else if (opt == "defined") {
        ++i;

        if (i >= argc) {
          errorMsg("Missing defined colors");
          continue;
        }

        QStringList strs = args[i].split(" ", QString::SkipEmptyParts);

        if (! strs.length())
          continue;

        double dv = (strs.length() > 1 ? 1.0/(strs.length() - 1) : 0.0);

        paletteData.definedColors.clear();

        for (int j = 0; j < strs.length(); ++j) {
          int pos = strs[j].indexOf('=');

          double v = j*dv;
          QColor c;

          if (pos > 0) {
            QString lhs = strs[j].mid(0, pos).simplified();
            QString rhs = strs[j].mid(pos + 1).simplified();

            bool ok;

            v = lhs.toDouble(&ok);
            c = QColor(rhs);
          }
          else
            c = QColor(strs[j]);

          paletteData.definedColors.push_back(CQChartsDefinedColor(v, c));
        }
      }
      else if (opt == "get_color") {
        ++i;

        if (i >= argc) {
          errorMsg("Missing color value");
          continue;
        }

        paletteData.getColorFlag  = true;
        paletteData.getColorValue = args[i].toDouble(&paletteData.getColorFlag);
      }
      else if (opt == "get_color_scale") {
        paletteData.getColorScale = true;
      }
      else {
        errorMsg("Invalid option '" + opt + "'");
      }
    }
    else {
      errorMsg("Invalid arg '" + arg + "'");
    }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  //---

  CQChartsGradientPalette *palette = view->theme()->palette();

  setPaleteData(palette, paletteData);

  //---

  view->updatePlots();

  CQChartsWindow *window = CQChartsWindowMgrInst->getWindowForView(view);

  if (window)
    window->updatePalette();
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
groupLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->groupCmd(args);
}
#endif

void
CQChartsCmds::
groupCmd(const Args &args)
{
  QString     viewName;
  QStringList plotNames;

  bool x1x2    = false;
  bool y1y2    = false;
  bool overlay = false;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view"   ) { (void) argv.getOptValue(viewName); }
      else if (opt == "x1x2"   ) { x1x2 = true; }
      else if (opt == "y1y2"   ) { y1y2 = true; }
      else if (opt == "overlay") { overlay = true; }
      else                       { argv.error(); }
    }
    else {
      QString str = arg.str();

      plotNames.push_back(str);
    }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  //---

  Plots plots;

  getPlotsByName(view, plotNames, plots);

  //---

  if      (x1x2) {
    if (plots.size() != 2) {
      errorMsg("Need 2 plots for x1x2");
      return;
    }

    view->initX1X2(plots[0], plots[1], overlay);
  }
  else if (y1y2) {
    if (plots.size() != 2) {
      errorMsg("Need 2 plots for y1y2");
      return;
    }

    view->initY1Y2(plots[0], plots[1], overlay);
  }
  else if (overlay) {
    if (plots.size() < 2) {
      errorMsg("Need 2 or more plots for overlay");
      return;
    }

    view->initOverlay(plots);
  }
  else {
    errorMsg("No grouping specified");
    return;
  }
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
placeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->placeCmd(args);
}
#endif

void
CQChartsCmds::
placeCmd(const Args &args)
{
  QString     viewName;
  QStringList plotNames;

  bool horizontal = false;
  bool vertical   = false;
  int  rows       = -1;
  int  columns    = -1;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view"      ) { (void) argv.getOptValue(viewName); }
      else if (opt == "vertical"  ) { vertical = true; }
      else if (opt == "horizontal") { horizontal = true; }
      else if (opt == "rows"      ) { (void) argv.getOptValue(rows); }
      else if (opt == "columns"   ) { (void) argv.getOptValue(columns); }
      else                          { argv.error(); }
    }
    else {
      QString str = arg.str();

      plotNames.push_back(str);
    }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  //---

  Plots plots;

  getPlotsByName(view, plotNames, plots);

  //---

  int np = plots.size();

  if (np <= 0)
    return;

  int nr = 1, nc = 1;

  if     (horizontal)
    nc = np;
  else if (vertical)
    nr = np;
  else if (rows > 0) {
    nr = rows;
    nc = (np + nr - 1)/nr;
  }
  else if (columns > 0) {
    nc = columns;
    nr = (np + nc - 1)/nc;
  }
  else {
    nr = std::max(int(sqrt(np)), 1);
    nc = (np + nr - 1)/nr;
  }

  double vr = CQChartsView::viewportRange();

  double dx = vr/nc;
  double dy = vr/nr;

  int    i = 0;
  double y = vr;

  for (int r = 0; r < nr; ++r) {
    double x = 0.0;

    for (int c = 0; c < nc; ++c, ++i) {
      CQChartsPlot *plot = plots[i];

      CQChartsGeom::BBox bbox(x, y - dy, x + dx, y);

      plot->setBBox(bbox);

      x += dx;
    }

    y -= dy;
  }
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
sortLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->sortCmd(args);
}
#endif

void
CQChartsCmds::
sortCmd(const Args &args)
{
  QString sort;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      //QString opt = arg.opt();

      argv.error();
    }
    else {
      if (sort == "")
        sort = arg.str();
      else
        argv.error();
    }
  }

  CQChartsModelData *modelData = currentModelData();

  if (! modelData)
    return;

  ModelP model = modelData->model;

  sortModel(model, sort);
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
getDataLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->getDataCmd(args);
}
#endif

void
CQChartsCmds::
getDataCmd(const Args &args)
{
  QString viewName;
  QString plotName;
  QString idName;
  QString columnName;
  QString roleName;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view"  ) { (void) argv.getOptValue(viewName); }
      else if (opt == "plot"  ) { (void) argv.getOptValue(plotName); }
      else if (opt == "id"    ) { (void) argv.getOptValue(idName); }
      else if (opt == "column") { (void) argv.getOptValue(columnName); }
      else if (opt == "role"  ) { (void) argv.getOptValue(roleName); }
      else                      { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  CQChartsPlot *plot = getPlotByName(view, plotName);
  if (! plot) return;

  //---

  CQChartsColumn column(columnName);

  bool ok;

  QVariant var = plot->getData(idName, column, roleName, ok);

  bool rc;

  setCmdRc(CQChartsUtil::toString(var, rc));
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
setDataLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->setDataCmd(args);
}
#endif

void
CQChartsCmds::
setDataCmd(const Args &args)
{
  QString plotName;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "plot") {
        (void) argv.getOptValue(plotName);
      }
      else {
        argv.error();
      }
    }
    else {
      argv.error();
    }
  }
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
rectShapeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->rectShapeCmd(args);
}
#endif

void
CQChartsCmds::
rectShapeCmd(const Args &args)
{
  QString viewName, plotName, id;

  double x1 { 0.0 }, y1 { 0.0 }, x2 { 0.0 }, y2 { 0.0 };

  CQChartsBoxData boxData;

  CQChartsFillData   &background = boxData.shape.background;
  CQChartsStrokeData &border     = boxData.shape.border;

  border.visible = true;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }
      else if (opt == "id"  ) { (void) argv.getOptValue(id); }

      else if (opt == "x1") { (void) argv.getOptValue(x1); }
      else if (opt == "y1") { (void) argv.getOptValue(y1); }
      else if (opt == "x2") { (void) argv.getOptValue(x2); }
      else if (opt == "y2") { (void) argv.getOptValue(y2); }

      else if (opt == "margin" ) { (void) argv.getOptValue(boxData.margin); }
      else if (opt == "padding") { (void) argv.getOptValue(boxData.padding); }

      else if (opt == "background"        ) { (void) argv.getOptValue(background.visible); }
      else if (opt == "background_color"  ) { (void) argv.getOptValue(background.color  ); }
      else if (opt == "background_alpha"  ) { (void) argv.getOptValue(background.alpha  ); }
//    else if (opt == "background_pattern") { (void) argv.getOptValue(background.pattern); }

      else if (opt == "border"      ) { (void) argv.getOptValue(border.visible); }
      else if (opt == "border_color") { (void) argv.getOptValue(border.color  ); }
      else if (opt == "border_alpha") { (void) argv.getOptValue(border.alpha  ); }
      else if (opt == "border_width") { (void) argv.getOptValue(border.width  ); }
      else if (opt == "border_dash" ) { (void) argv.getOptValue(border.dash   ); }

      else if (opt == "corner_size" ) { (void) argv.getOptValue(boxData.cornerSize ); }
      else if (opt == "border_sides") { (void) argv.getOptValue(boxData.borderSides); }

      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  CQChartsPlot *plot = getOptPlotByName(view, plotName);
  if (! plot) return;

  //---

  QPointF start(x1, y1);
  QPointF end  (x2, y2);

  CQChartsRectAnnotation *annotation = plot->addRectAnnotation(start, end);

  annotation->setId(id);

  annotation->setBoxData(boxData);

  setCmdRc(annotation->ind());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
ellipseShapeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->ellipseShapeCmd(args);
}
#endif

void
CQChartsCmds::
ellipseShapeCmd(const Args &args)
{
  QString viewName, plotName, id;

  double xc { 0.0 }, yc { 0.0 }, rx { 0.0 }, ry { 0.0 };

  CQChartsBoxData boxData;

  CQChartsFillData   &background = boxData.shape.background;
  CQChartsStrokeData &border     = boxData.shape.border;

  border.visible = true;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }
      else if (opt == "id"  ) { (void) argv.getOptValue(id); }

      else if (opt == "xc") { (void) argv.getOptValue(xc); }
      else if (opt == "yc") { (void) argv.getOptValue(yc); }
      else if (opt == "rx") { (void) argv.getOptValue(rx); }
      else if (opt == "ry") { (void) argv.getOptValue(ry); }

      else if (opt == "background"        ) { (void) argv.getOptValue(background.visible); }
      else if (opt == "background_color"  ) { (void) argv.getOptValue(background.color  ); }
      else if (opt == "background_alpha"  ) { (void) argv.getOptValue(background.alpha  ); }
//    else if (opt == "background_pattern") { (void) argv.getOptValue(background.pattern); }

      else if (opt == "border"      ) { (void) argv.getOptValue(border.visible); }
      else if (opt == "border_color") { (void) argv.getOptValue(border.color  ); }
      else if (opt == "border_alpha") { (void) argv.getOptValue(border.alpha  ); }
      else if (opt == "border_width") { (void) argv.getOptValue(border.width  ); }
      else if (opt == "border_dash" ) { (void) argv.getOptValue(border.dash   ); }

      else if (opt == "corner_size" ) { (void) argv.getOptValue(boxData.cornerSize ); }
      else if (opt == "border_sides") { (void) argv.getOptValue(boxData.borderSides); }

      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  CQChartsPlot *plot = getOptPlotByName(view, plotName);
  if (! plot) return;

  //---

  QPointF center(xc, yc);

  CQChartsEllipseAnnotation *annotation = plot->addEllipseAnnotation(center, rx, ry);

  annotation->setId(id);

  annotation->setBoxData(boxData);

  setCmdRc(annotation->ind());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
polygonShapeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->polygonShapeCmd(args);
}
#endif

void
CQChartsCmds::
polygonShapeCmd(const Args &args)
{
  QString viewName, plotName, id;

  QPolygonF points;

  CQChartsShapeData shapeData;

  CQChartsFillData   &background = shapeData.background;
  CQChartsStrokeData &border     = shapeData.border;

  border.visible = true;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }
      else if (opt == "id"  ) { (void) argv.getOptValue(id); }

      else if (opt == "points") { (void) argv.getOptValue(points); }

      else if (opt == "background"        ) { (void) argv.getOptValue(background.visible); }
      else if (opt == "background_color"  ) { (void) argv.getOptValue(background.color  ); }
      else if (opt == "background_alpha"  ) { (void) argv.getOptValue(background.alpha  ); }
//    else if (opt == "background_pattern") { (void) argv.getOptValue(background.pattern); }

      else if (opt == "border"      ) { (void) argv.getOptValue(border.visible); }
      else if (opt == "border_color") { (void) argv.getOptValue(border.color  ); }
      else if (opt == "border_alpha") { (void) argv.getOptValue(border.alpha  ); }
      else if (opt == "border_width") { (void) argv.getOptValue(border.width  ); }
      else if (opt == "border_dash" ) { (void) argv.getOptValue(border.dash   ); }

      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  CQChartsPlot *plot = getOptPlotByName(view, plotName);
  if (! plot) return;

  //---

  CQChartsPolygonAnnotation *annotation = plot->addPolygonAnnotation(points);

  annotation->setId(id);

  annotation->setShapeData(shapeData);

  setCmdRc(annotation->ind());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
polylineShapeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->polylineShapeCmd(args);
}
#endif

void
CQChartsCmds::
polylineShapeCmd(const Args &args)
{
  QString viewName, plotName, id;

  QPolygonF points;

  CQChartsShapeData shapeData;

  CQChartsFillData   &background = shapeData.background;
  CQChartsStrokeData &border     = shapeData.border;

  border.visible = true;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }
      else if (opt == "id"  ) { (void) argv.getOptValue(id); }

      else if (opt == "points") { (void) argv.getOptValue(points); }

      else if (opt == "background"        ) { (void) argv.getOptValue(background.visible); }
      else if (opt == "background_color"  ) { (void) argv.getOptValue(background.color  ); }
      else if (opt == "background_alpha"  ) { (void) argv.getOptValue(background.alpha  ); }
//    else if (opt == "background_pattern") { (void) argv.getOptValue(background.pattern); }

      else if (opt == "border"      ) { (void) argv.getOptValue(border.visible); }
      else if (opt == "border_color") { (void) argv.getOptValue(border.color  ); }
      else if (opt == "border_alpha") { (void) argv.getOptValue(border.alpha  ); }
      else if (opt == "border_width") { (void) argv.getOptValue(border.width  ); }
      else if (opt == "border_dash" ) { (void) argv.getOptValue(border.dash   ); }

      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  CQChartsPlot *plot = getOptPlotByName(view, plotName);
  if (! plot) return;

  //---

  CQChartsPolylineAnnotation *annotation = plot->addPolylineAnnotation(points);

  annotation->setId(id);

  annotation->setShapeData(shapeData);

  setCmdRc(annotation->ind());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
textShapeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->textShapeCmd(args);
}
#endif

void
CQChartsCmds::
textShapeCmd(const Args &args)
{
  QString viewName, plotName, id;

  double  x = 0.0, y = 0.0;
  QString text = "Annotation";

  CQChartsTextData textData;
  CQChartsBoxData  boxData;

  CQChartsFillData   &background = boxData.shape.background;
  CQChartsStrokeData &border     = boxData.shape.border;

  background.visible = false;
  border    .visible = false;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }
      else if (opt == "id"  ) { (void) argv.getOptValue(id); }

      else if (opt == "x") { (void) argv.getOptValue(x); }
      else if (opt == "y") { (void) argv.getOptValue(y); }

      else if (opt == "text") { (void) argv.getOptValue(text); }

      else if (opt == "font"    ) { (void) argv.getOptValue(textData.font ); }
      else if (opt == "color"   ) { (void) argv.getOptValue(textData.color); }
      else if (opt == "alpha"   ) { (void) argv.getOptValue(textData.alpha); }
      else if (opt == "angle"   ) { (void) argv.getOptValue(textData.angle); }
      else if (opt == "contrast") { (void) argv.getOptValue(textData.contrast); }
//    else if (opt == "align"   ) { (void) argv.getOptValue(textData.align); }

      else if (opt == "background"        ) { (void) argv.getOptValue(background.visible); }
      else if (opt == "background_color"  ) { (void) argv.getOptValue(background.color  ); }
      else if (opt == "background_alpha"  ) { (void) argv.getOptValue(background.alpha  ); }
//    else if (opt == "background_pattern") { (void) argv.getOptValue(background.pattern); }

      else if (opt == "border"      ) { (void) argv.getOptValue(border.visible); }
      else if (opt == "border_color") { (void) argv.getOptValue(border.color  ); }
      else if (opt == "border_alpha") { (void) argv.getOptValue(border.alpha  ); }
      else if (opt == "border_width") { (void) argv.getOptValue(border.width  ); }
      else if (opt == "border_dash" ) { (void) argv.getOptValue(border.dash   ); }

      else if (opt == "corner_size" ) { (void) argv.getOptValue(boxData.cornerSize ); }
      else if (opt == "border_sides") { (void) argv.getOptValue(boxData.borderSides); }

      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  //---

  QPointF pos(x, y);

  CQChartsTextAnnotation *annotation;

  if (plotName != "") {
    CQChartsPlot *plot = getOptPlotByName(view, plotName);
    if (! plot) return;

    annotation = plot->addTextAnnotation(pos, text);
  }
  else {
    annotation = view->addTextAnnotation(pos, text);
  }

  annotation->setId(id);

  annotation->setTextData(textData);
  annotation->setBoxData(boxData);

  setCmdRc(annotation->ind());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
arrowShapeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->arrowShapeCmd(args);
}
#endif

void
CQChartsCmds::
arrowShapeCmd(const Args &args)
{
  QString viewName, plotName, id;

  double x1 { 0.0 }; double y1 { 0.0 };
  double x2 { 0.0 }; double y2 { 0.0 };

  CQChartsArrowData arrowData;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }
      else if (opt == "id"  ) { (void) argv.getOptValue(id); }

      else if (opt == "x1") { (void) argv.getOptValue(x1); }
      else if (opt == "y1") { (void) argv.getOptValue(y1); }
      else if (opt == "x2") { (void) argv.getOptValue(x2); }
      else if (opt == "y2") { (void) argv.getOptValue(y2); }

      else if (opt == "length"      ) { (void) argv.getOptValue(arrowData.length      ); }
      else if (opt == "angle"       ) { (void) argv.getOptValue(arrowData.angle       ); }
      else if (opt == "back_angle"  ) { (void) argv.getOptValue(arrowData.backAngle   ); }
      else if (opt == "fhead"       ) { (void) argv.getOptValue(arrowData.fhead       ); }
      else if (opt == "thead"       ) { (void) argv.getOptValue(arrowData.thead       ); }
      else if (opt == "empty"       ) { (void) argv.getOptValue(arrowData.empty       ); }
      else if (opt == "line_width"  ) { (void) argv.getOptValue(arrowData.stroke.width); }
      else if (opt == "stroke_color") { (void) argv.getOptValue(arrowData.stroke.color); }
      else if (opt == "filled"      ) { (void) argv.getOptValue(arrowData.fill.visible); }
      else if (opt == "fill_color"  ) { (void) argv.getOptValue(arrowData.fill.color  ); }
      else if (opt == "labels"      ) { (void) argv.getOptValue(arrowData.labels      ); }

      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  //---

  QPointF start(x1, y1);
  QPointF end  (x2, y2);

  CQChartsArrowAnnotation *annotation;

  if (plotName != "") {
    CQChartsPlot *plot = getOptPlotByName(view, plotName);
    if (! plot) return;

    annotation = plot->addArrowAnnotation(start, end);
  }
  else {
    annotation = view->addArrowAnnotation(start, end);
  }

  annotation->setId(id);

  annotation->setData(arrowData);

  setCmdRc(annotation->ind());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
pointShapeLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->pointShapeCmd(args);
}
#endif

void
CQChartsCmds::
pointShapeCmd(const Args &args)
{
  QString viewName, plotName, id;

  CQChartsSymbolData pointData;
  double             x = 0.0, y = 0.0;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }
      else if (opt == "id"  ) { (void) argv.getOptValue(id); }

      else if (opt == "x") { (void) argv.getOptValue(x); }
      else if (opt == "y") { (void) argv.getOptValue(y); }

      else if (opt == "size") { (void) argv.getOptValue(pointData.size); }

      else if (opt == "type") {
        QString typeStr;

        if (argv.getOptValue(typeStr))
          pointData.type = CQChartsPlotSymbolMgr::nameToType(typeStr);
      }

      else if (opt == "stroked") { (void) argv.getOptValue(pointData.stroke.visible); }
      else if (opt == "filled" ) { (void) argv.getOptValue(pointData.fill  .visible); }

      else if (opt == "line_width") { (void) argv.getOptValue(pointData.stroke.width); }
      else if (opt == "line_color") { (void) argv.getOptValue(pointData.stroke.color); }
      else if (opt == "line_alpha") { (void) argv.getOptValue(pointData.stroke.alpha); }

      else if (opt == "fill_color") { (void) argv.getOptValue(pointData.fill.color); }
      else if (opt == "fill_alpha") { (void) argv.getOptValue(pointData.fill.alpha); }

      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  CQChartsPlot *plot = getOptPlotByName(view, plotName);
  if (! plot) return;

  //---

  QPointF pos(x, y);

  CQChartsPointAnnotation *annotation = plot->addPointAnnotation(pos, pointData.type);

  annotation->setId(id);

  annotation->setPointData(pointData);

  setCmdRc(annotation->ind());
}

//------

#ifdef CQ_CHARTS_CEIL
void
CQChartsCmds::
connectLCmd(ClLanguageCommand *command, ClLanguageArgs *largs, void *data)
{
  CQChartsCmds *cmds = static_cast<CQChartsCmds *>(data);

  Args args = cmds->parseCommandArgs(command, largs);

  cmds->connectCmd(args);
}
#endif

void
CQChartsCmds::
connectCmd(const Args &args)
{
  QString viewName, plotName;
  QString fromName, toName;

  CQChartsCmdsArgs argv(args);

  while (! argv.eof()) {
    const CQChartsCmdsArgs::Arg &arg = argv.getArg();

    if (arg.isOpt()) {
      QString opt = arg.opt();

      if      (opt == "view") { (void) argv.getOptValue(viewName); }
      else if (opt == "plot") { (void) argv.getOptValue(plotName); }

      else if (opt == "from") { (void) argv.getOptValue(fromName); }
      else if (opt == "to"  ) { (void) argv.getOptValue(toName); }

      else { argv.error(); }
    }
    else { argv.error(); }
  }

  //---

  CQChartsView *view = getViewByName(viewName);
  if (! view) return;

  CQChartsPlot *plot = nullptr;

  if (plotName != "") {
    plot = getOptPlotByName(view, plotName);
    if (! plot) return;
  }

  //---

  CQChartsCmdsSlot *cmdsSlot = new CQChartsCmdsSlot(this, view, plot, toName);

  if      (fromName == "objIdPressed") {
    if (plot)
      connect(plot, SIGNAL(objIdPressed(const QString &)),
              cmdsSlot, SLOT(objIdPressed(const QString &)));
    else
      connect(view, SIGNAL(objIdPressed(const QString &)),
              cmdsSlot, SLOT(objIdPressed(const QString &)));
  }
  else if (fromName == "annotationIdPressed") {
    if (plot)
      connect(plot, SIGNAL(annotationIdPressed(const QString &)),
              cmdsSlot, SLOT(annotationIdPressed(const QString &)));
    else
      connect(view, SIGNAL(annotationIdPressed(const QString &)),
              cmdsSlot, SLOT(annotationIdPressed(const QString &)));
  }
  else {
    errorMsg("unknown slot");
    return;
  }

  return;
}

//------

void
CQChartsCmds::
sourceCmd(const Args &args)
{
  QString filename;

  int argc = args.size();

  for (int i = 0; i < argc; ++i) {
    QString arg = args[i];

    if (arg[0] == '-') {
      QString opt = arg.mid(1);

      errorMsg("Invalid option '" + opt + "'");
    }
    else {
      if (filename == "")
        filename = arg;
    }
  }

  if (filename == "") {
    errorMsg("No filename");
    return;
  }

  CUnixFile file(filename.toStdString());

  if (! file.open()) {
    errorMsg("Failed to open file '" + filename + "'");
    return;
  }

  // read lines
  std::string line;

  while (file.readLine(line)) {
    QString qline = line.c_str();

    while (! isCompleteLine(qline)) {
      std::string line1;

      if (! file.readLine(line1))
        break;

      QString qline1 = line1.c_str();

      qline += "\n" + qline1;
    }

    parseLine(qline);
  }
}

void
CQChartsCmds::
letCmd(const Args &args)
{
  int argc = args.size();

  if (argc != 1) {
    errorMsg("let requires 1 args");
    return;
  }

  CExprValuePtr value;

  CQChartsExpr::processAssignExpression(expr(), args[0], value); //init
}

void
CQChartsCmds::
ifCmd(const Args &args)
{
  int argc = args.size();

  if (argc != 2) {
    errorMsg("syntax error : @if {expr} {statement}");
    return;
  }

  QStringList lines = stringToCmds(args[1]);

  bool b;

  if (CQChartsExpr::processBoolExpression(expr(), args[0], b) && b) { // test
    for (int i = 0; i < lines.length(); ++i) {
      parseLine(lines[i]); // body
    }
  }
}

void
CQChartsCmds::
whileCmd(const Args &args)
{
  int argc = args.size();

  if (argc != 2) {
    errorMsg("syntax error : @while {expr} {statement}");
    return;
  }

  QStringList lines = stringToCmds(args[1]);

  bool b;

  while (CQChartsExpr::processBoolExpression(expr(), args[0], b) && b) { // test
    for (int i = 0; i < lines.length(); ++i) {
      continueFlag_ = false;

      parseLine(lines[i]); // body

      if (continueFlag_)
        break;
    }
  }
}

void
CQChartsCmds::
continueCmd(const Args &)
{
  continueFlag_ = true;
}

void
CQChartsCmds::
printCmd(const Args &args)
{
  int argc = args.size();

  for (int i = 0; i < argc; ++i) {
    CExprValuePtr value;

    CQChartsExpr::processExpression(expr(), args[i], value);

    if (value.isValid()) {
      value->print(std::cout);

      std::cout << "\n";
    }
  }
}

//------

void
CQChartsCmds::
setPaleteData(CQChartsGradientPalette *palette, const CQChartsPaletteColorData &paletteData)
{
  if (paletteData.colorTypeStr != "") {
    CQChartsGradientPalette::ColorType colorType = CQChartsGradientPalette::ColorType::MODEL;

    if      (paletteData.colorTypeStr == "model"    )
      colorType = CQChartsGradientPalette::ColorType::MODEL;
    else if (paletteData.colorTypeStr == "defined"  )
      colorType = CQChartsGradientPalette::ColorType::DEFINED;
    else if (paletteData.colorTypeStr == "functions")
      colorType = CQChartsGradientPalette::ColorType::FUNCTIONS;
    else if (paletteData.colorTypeStr == "cubehelix")
      colorType = CQChartsGradientPalette::ColorType::CUBEHELIX;

    palette->setColorType(colorType);
  }

  //---

  if (paletteData.colorModelStr != "") {
    CQChartsGradientPalette::ColorModel colorModel = CQChartsGradientPalette::ColorModel::RGB;

    if      (paletteData.colorModelStr == "rgb")
      colorModel = CQChartsGradientPalette::ColorModel::RGB;
    else if (paletteData.colorModelStr == "hsv")
      colorModel = CQChartsGradientPalette::ColorModel::HSV;
    else if (paletteData.colorModelStr == "cmy")
      colorModel = CQChartsGradientPalette::ColorModel::CMY;
    else if (paletteData.colorModelStr == "yiq")
      colorModel = CQChartsGradientPalette::ColorModel::YIQ;
    else if (paletteData.colorModelStr == "xyz")
      colorModel = CQChartsGradientPalette::ColorModel::XYZ;

    palette->setColorModel(colorModel);
  }

  //---

  if (paletteData.redModel  ) palette->setRedModel  (*paletteData.redModel  );
  if (paletteData.greenModel) palette->setGreenModel(*paletteData.greenModel);
  if (paletteData.blueModel ) palette->setBlueModel (*paletteData.blueModel );

  if (paletteData.negateRed  ) palette->setRedNegative  (*paletteData.negateRed  );
  if (paletteData.negateGreen) palette->setGreenNegative(*paletteData.negateGreen);
  if (paletteData.negateBlue ) palette->setBlueNegative (*paletteData.negateBlue );

  if (paletteData.redMin  ) palette->setRedMin  (*paletteData.redMin  );
  if (paletteData.redMax  ) palette->setRedMax  (*paletteData.redMax  );
  if (paletteData.greenMin) palette->setGreenMin(*paletteData.greenMin);
  if (paletteData.greenMax) palette->setGreenMax(*paletteData.greenMax);
  if (paletteData.blueMin ) palette->setBlueMin (*paletteData.blueMin );
  if (paletteData.blueMax ) palette->setBlueMax (*paletteData.blueMax );

  //---

  if (! paletteData.definedColors.empty()) {
    palette->resetDefinedColors();

    for (const auto &definedColor : paletteData.definedColors)
      palette->addDefinedColor(definedColor.v, definedColor.c);
  }

  //---

  if (paletteData.getColorFlag) {
    QColor c = palette->getColor(paletteData.getColorValue, paletteData.getColorScale);

    setCmdRc(c.name());
  }
}

//------

QStringList
CQChartsCmds::
stringToCmds(const QString &str) const
{
  QStringList lines = str.split('\n', QString::SkipEmptyParts);

  QStringList lines1;

  int i = 0;

  for ( ; i < lines.size(); ++i) {
    QString line = lines[i];

    while (! isCompleteLine(line)) {
      ++i;

      if (i >= lines.size())
        break;

      const QString &line1 = lines[i];

      line += "\n" + line1;
    }

    lines1.push_back(line);
  }

  return lines1;
}

//------

#ifdef CQ_CHARTS_CEIL
CQChartsCmds::Args
CQChartsCmds::
parseCommandArgs(ClLanguageCommand *command, ClLanguageArgs *largs)
{
  largs->setSpaceSeparated(true);
  largs->setStripQuotes   (true);

  largs->setArgs(command);

  uint num_args = largs->getNumArgs();

  Args args;

  for (uint i = 0; i < num_args; ++i) {
    int error_code;

    std::string arg = largs->getArg(i + 1, &error_code);
    assert(! arg.empty());

    if (arg[0] == '$') {
      std::string var = arg.substr(1);

      ClParserValuePtr value = ClParserInst->getVariableValue(var);

      std::string str;

      if (value.isValid()) {
        if (value->isString())
          str = value->getString()->getText();
        else
          str = value->asString();
      }

      args.push_back(str.c_str());
    }
    else
      args.push_back(arg.c_str());
  }

  return args;
}
#endif

void
CQChartsCmds::
setCmdRc(int rc)
{
#ifdef CQ_CHARTS_CEIL
  if (isCeil()) {
    ClParserInst->setVariableValue("_rc", ClParserValueMgrInst->createValue((long) rc));
    return;
  }
#endif

  CExprValuePtr ivalue = expr()->createIntegerValue(rc);

  expr()->createVariable("_rc", ivalue);
}

void
CQChartsCmds::
setCmdRc(const QString &rc)
{
#ifdef CQ_CHARTS_CEIL
  if (isCeil()) {
    ClParserInst->setVariableValue("_rc", ClParserValueMgrInst->createValue(rc.toStdString()));
    return;
  }
#endif

  CExprValuePtr svalue = expr()->createStringValue(rc.toStdString());

  expr()->createVariable("_rc", svalue);
}

void
CQChartsCmds::
setCmdRc(const QVariant &rc)
{
#ifdef CQ_CHARTS_CEIL
  if (isCeil()) {
    if      (rc.type() == QVariant::Int)
      ClParserInst->setVariableValue("_rc",
        ClParserValueMgrInst->createValue((long) rc.value<int>()));
    else if (rc.type() == QVariant::Double)
      ClParserInst->setVariableValue("_rc",
        ClParserValueMgrInst->createValue(rc.value<double>()));
    else {
      bool ok;

      ClParserInst->setVariableValue("_rc",
        ClParserValueMgrInst->createValue(CQChartsUtil::toString(rc, ok).toStdString()));
    }

    return;
  }
#endif

  CExprValuePtr svalue = expr()->createStringValue(rc.toString().toStdString());

  expr()->createVariable("_rc", svalue);
}

//------

CQChartsPlot *
CQChartsCmds::
createPlot(const ModelP &model, QItemSelectionModel *sm, CQChartsPlotType *type,
           const CQChartsNameValueData &nameValueData, bool reuse,
           const CQChartsGeom::BBox &bbox)
{
  CQChartsView *view = getView(reuse);

  //---

  CQChartsPlot *plot = type->create(view, model);

  if (sm)
    plot->setSelectionModel(sm);

  //---

  // set plot property for widgets for plot parameters
  for (const auto &parameter : type->parameters()) {
    if      (parameter.type() == "column") {
      auto p = nameValueData.values.find(parameter.name());

      if (p == nameValueData.values.end())
        continue;

      CQChartsColumn column;

      if (! stringToColumn(model, (*p).second, column)) {
        errorMsg("Bad column name '" + (*p).second + "'");
        column = -1;
      }

      QString scol = column.toString();

      if (! CQUtil::setProperty(plot, parameter.propName(), scol))
        errorMsg("Failed to set parameter " + parameter.propName());
    }
    else if (parameter.type() == "columns") {
      auto p = nameValueData.values.find(parameter.name());

      if (p == nameValueData.values.end())
        continue;

      QStringList strs = (*p).second.split(" ", QString::SkipEmptyParts);

      std::vector<CQChartsColumn> columns;

      for (int j = 0; j < strs.size(); ++j) {
        CQChartsColumn column;

        if (! stringToColumn(model, strs[j], column)) {
          errorMsg("Bad column name '" + strs[j] + "'");
          continue;
        }

        columns.push_back(column);
      }

      QString s = CQChartsColumn::columnsToString(columns);

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(s)))
        errorMsg("Failed to set parameter " + parameter.propName());
    }
    else if (parameter.type() == "string") {
      auto p = nameValueData.strings.find(parameter.name());

      if (p == nameValueData.strings.end())
        continue;

      QString str = (*p).second;

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(str)))
        errorMsg("Failed to set parameter " + parameter.propName());
    }
    else if (parameter.type() == "real") {
      auto p = nameValueData.reals.find(parameter.name());

      if (p == nameValueData.reals.end())
        continue;

      double r = (*p).second;

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(r)))
        errorMsg("Failed to set parameter " + parameter.propName());
    }
    else if (parameter.type() == "bool") {
      auto p = nameValueData.bools.find(parameter.name());

      if (p == nameValueData.bools.end())
        continue;

      bool b = (*p).second;

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(b)))
        errorMsg("Failed to set parameter " + parameter.propName());
    }
    else
      assert(false);
  }

  //---

  // add plot to view and show
  view->addPlot(plot, bbox);

  //---

  emit plotCreated(plot);

  return plot;
}

QString
CQChartsCmds::
fixTypeName(const QString &typeName) const
{
  QString typeName1 = typeName;

  // adjust typename for alias (TODO: add to typeData)
  if      (typeName1 == "piechart"     ) typeName1 = "pie";
  else if (typeName1 == "xyplot"       ) typeName1 = "xy";
  else if (typeName1 == "scatterplot"  ) typeName1 = "scatter";
  else if (typeName1 == "bar"          ) typeName1 = "barchart";
  else if (typeName1 == "boxplot"      ) typeName1 = "box";
  else if (typeName1 == "parallelplot" ) typeName1 = "parallel";
  else if (typeName1 == "geometryplot" ) typeName1 = "geometry";
  else if (typeName1 == "delaunayplot" ) typeName1 = "delaunay";
  else if (typeName1 == "adjacencyplot") typeName1 = "adjacency";

  return typeName1;
}

//------

void
CQChartsCmds::
setViewProperties(CQChartsView *view, const QString &properties)
{
  if (! view->setProperties(properties))
    errorMsg("Failed to set view properties '" + properties + "'");
}

void
CQChartsCmds::
setPlotProperties(CQChartsPlot *plot, const QString &properties)
{
  if (! plot->setProperties(properties))
    errorMsg("Failed to set plot properties '" + properties + "'");
}

//------

void
CQChartsCmds::
processExpression(const QString &expr)
{
  CQChartsModelData *modelData = currentModelData();

  if (! modelData)
    return;

  ModelP model = modelData->model;

  processExpression(model, expr);
}

void
CQChartsCmds::
processAddExpression(ModelP &model, const QString &exprStr)
{
  CQExprModel *exprModel = getExprModel(model);

  if (! exprModel) {
    errorMsg("Expression not supported for model");
    return;
  }

  exprModel->addExtraColumn(exprStr);
}

void
CQChartsCmds::
processExpression(ModelP &model, const QString &exprStr)
{
  CQExprModel *exprModel = getExprModel(model);

  if (! exprModel) {
    errorMsg("Expression not supported for model");
    return;
  }

  CQExprModel::Function function { CQExprModel::Function::EVAL };
  int                   column   { -1 };
  QString               expr;

  if (! exprModel->decodeExpressionFn(exprStr, function, column, expr)) {
    errorMsg("Invalid expression '" + exprStr + "'");
    return;
  }

  processExpression(model, function, column, expr);
}

void
CQChartsCmds::
processExpression(ModelP &model, CQExprModel::Function function, int column, const QString &expr)
{
  CQExprModel *exprModel = getExprModel(model);

  if (! exprModel) {
    errorMsg("Expression not supported for model");
    return;
  }

  // add column <expr>
  if      (function == CQExprModel::Function::ADD) {
    exprModel->addExtraColumn(expr);
  }
  // delete column <n>
  else if (function == CQExprModel::Function::DELETE) {
    bool rc = exprModel->removeExtraColumn(column);

    if (! rc) {
      errorMsg(QString("Failed to delete column '%1'").arg(column));
      return;
    }
  }
  // modify column <n>:<expr>
  else if (function == CQExprModel::Function::ASSIGN) {
    exprModel->assignExtraColumn(column, expr);
  }
  else {
    exprModel->processExpr(expr);
  }
}

//------

void
CQChartsCmds::
setColumnFormats(const ModelP &model, const QString &columnType)
{
  // split into multiple column type definitions
  QStringList fstrs = columnType.split(";", QString::KeepEmptyParts);

  for (int i = 0; i < fstrs.length(); ++i) {
    QString typeStr = fstrs[i].simplified();

    if (! typeStr.length())
      continue;

    // default column to index
    CQChartsColumn column(i);

    // if #<col> then use that for column index
    int pos = typeStr.indexOf("#");

    if (pos >= 0) {
      QString columnStr = typeStr.mid(0, pos).simplified();

      CQChartsColumn column1;

      if (stringToColumn(model, columnStr, column1))
        column = column1;
      else
        errorMsg("Bad column name '" + columnStr + "'");

      typeStr = typeStr.mid(pos + 1).simplified();
    }

    //---

    if (! CQChartsUtil::setColumnTypeStr(charts_, model.data(), column, typeStr)) {
      errorMsg(QString("Invalid type '" + typeStr + "' for section '%1'").
                 arg(column.toString()));
      continue;
    }
  }
}

//------

CQChartsView *
CQChartsCmds::
getViewByName(const QString &viewName) const
{
  CQChartsView *view = nullptr;

  if (viewName != "") {
    view = charts_->getView(viewName);

    if (! view) {
      errorMsg("No view '" + viewName + "'");
      return nullptr;
    }
  }
  else {
    view = currentView();

    if (! view) {
      CQChartsCmds *th = const_cast<CQChartsCmds *>(this);

      view = th->getView(/*reuse*/true);
    }

    if (! view) {
      errorMsg("No view");
      return nullptr;
    }
  }

  return view;
}

//------

bool
CQChartsCmds::
getPlotsByName(CQChartsView *view, QStringList &plotNames, Plots &plots) const
{
  bool rc = true;

  for (int i = 0; i < plotNames.length(); ++i) {
    QString plotName = plotNames[i];

    CQChartsPlot *plot = getPlotByName(view, plotName);

    if (plot)
      plots.push_back(plot);
    else
      rc = false;
  }

  return rc;
}

CQChartsPlot *
CQChartsCmds::
getOptPlotByName(CQChartsView *view, const QString &plotName) const
{
  CQChartsPlot *plot = nullptr;

  if (plotName != "")
    plot = getPlotByName(view, plotName);
  else
    plot = view->currentPlot();

  return plot;
}

CQChartsPlot *
CQChartsCmds::
getPlotByName(CQChartsView *view, const QString &plotName) const
{
  assert(view);

  CQChartsPlot *plot = view->getPlot(plotName);

  if (! plot) {
    errorMsg("No plot '" + plotName + "'");
    return nullptr;
  }

  return plot;
}

//------

CQExprModel *
CQChartsCmds::
getExprModel(ModelP &model) const
{
  CQExprModel *exprModel = qobject_cast<CQExprModel *>(model.data());

  if (! exprModel) {
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(model.data());

    if (proxyModel)
      exprModel = qobject_cast<CQExprModel *>(proxyModel->sourceModel());
  }

  return exprModel;
}

bool
CQChartsCmds::
loadFileModel(const QString &filename, CQChartsFileType type, const CQChartsInputData &inputData)
{
  CScopeTimer timer("loadFileModel");

  bool hierarchical;

  QAbstractItemModel *model = loadFile(filename, type, inputData, hierarchical);

  if (! model)
    return false;

  ModelP modelp(model);

  CQChartsModelData *modelData = addModelData(modelp, hierarchical);

  //---

  if (inputData.fold.length())
    foldModel(modelData, inputData.fold);

  //---

  sortModel(modelData->model, inputData.sort);

  //---

  emit updateModel(modelData->ind);

  //test_->updateModel(modelData);

  return true;
}

QAbstractItemModel *
CQChartsCmds::
loadFile(const QString &filename, CQChartsFileType type, const CQChartsInputData &inputData,
         bool &hierarchical)
{
  hierarchical = false;

  QAbstractItemModel *model = nullptr;

  if      (type == CQChartsFileType::CSV) {
    model = loadCsv(filename, inputData);
  }
  else if (type == CQChartsFileType::TSV) {
    model = loadTsv(filename, inputData);
  }
  else if (type == CQChartsFileType::JSON) {
    model = loadJson(filename, hierarchical);
  }
  else if (type == CQChartsFileType::DATA) {
    model = loadData(filename, inputData);
  }
  else if (type == CQChartsFileType::EXPR) {
    model = createExprModel(inputData.numRows);
  }
  else if (type == CQChartsFileType::VARS) {
    model = createVarsModel(inputData.vars);
  }
  else {
    errorMsg("Bad file type specified '" + fileTypeToString(type) + "'");
    return nullptr;
  }

  return model;
}

QAbstractItemModel *
CQChartsCmds::
loadCsv(const QString &filename, const CQChartsInputData &inputData)
{
  CQChartsCsv *csv = new CQChartsCsv(charts_);

  csv->setCommentHeader    (inputData.commentHeader);
  csv->setFirstLineHeader  (inputData.firstLineHeader);
  csv->setFirstColumnHeader(inputData.firstColumnHeader);

  if (! csv->load(filename))
    errorMsg("Failed to load '" + filename + "'");

  if (inputData.filter.length())
    csv->setSimpleFilter(inputData.filter);

  return csv;
}

QAbstractItemModel *
CQChartsCmds::
loadTsv(const QString &filename, const CQChartsInputData &inputData)
{
  CQChartsTsv *tsv = new CQChartsTsv(charts_);

  tsv->setCommentHeader    (inputData.commentHeader);
  tsv->setFirstLineHeader  (inputData.firstLineHeader);
  tsv->setFirstColumnHeader(inputData.firstColumnHeader);

  if (! tsv->load(filename))
    errorMsg("Failed to load '" + filename + "'");

  if (inputData.filter.length())
    tsv->setSimpleFilter(inputData.filter);

  return tsv;
}

QAbstractItemModel *
CQChartsCmds::
loadJson(const QString &filename, bool &hierarchical)
{
  CQChartsJson *json = new CQChartsJson(charts_);

  if (! json->load(filename))
    errorMsg("Failed to load '" + filename + "'");

  hierarchical = json->isHierarchical();

  return json;
}

QAbstractItemModel *
CQChartsCmds::
loadData(const QString &filename, const CQChartsInputData &inputData)
{
  CQChartsGnuData *data = new CQChartsGnuData(charts_);

  data->setCommentHeader    (inputData.commentHeader);
  data->setFirstLineHeader  (inputData.firstLineHeader);
//data->setFirstColumnHeader(inputData.firstColumnHeader);

  if (! data->load(filename))
    errorMsg("Failed to load '" + filename + "'");

  return data;
}

QAbstractItemModel *
CQChartsCmds::
createExprModel(int n)
{
  int nc = 1;
  int nr = n;

  CQChartsExprModel *data = new CQChartsExprModel(charts_, nc, nr);

  return data;
}

QAbstractItemModel *
CQChartsCmds::
createVarsModel(const Vars &vars)
{
#ifdef CQ_CHARTS_CEIL
  using ColumnValues = std::vector<QVariant>;
  using VarColumns   = std::vector<ColumnValues>;

  VarColumns varColumns;

  int nv = vars.size();

  int nr = -1;

  for (int i = 0; i < nv; ++i) {
    const QString &var = vars[i];

    ClParserValuePtr value = ClParserInst->getVariableValue(var.toStdString());
    if (! value.isValid()) continue;

    ClParserValueArray values;

    value->toSubValues(values);

    int nv1 = values.size();

    ColumnValues columnValues;

    columnValues.resize(nv1);

    for (int j = 0; j < nv1; ++j) {
      const ClParserValuePtr &value = values[j];

      if      (value->getType() == CL_PARSER_VALUE_TYPE_INTEGER) {
        long l;

        (void) value->integerValue(&l);

        columnValues[j] = QVariant(int(l));
      }
      else if (value->getType() == CL_PARSER_VALUE_TYPE_REAL) {
        double r;

        (void) value->realValue(&r);

        columnValues[j] = QVariant(r);
      }
      else if (value->getType() == CL_PARSER_VALUE_TYPE_STRING) {
        std::string s;

        (void) value->stringValue(s);

        columnValues[j] = QVariant(s.c_str());
      }
    }

    if (nr < 0)
      nr = nv1;
    else
      nr = std::min(nr, nv1);

    varColumns.push_back(columnValues);
  }

  int nc = varColumns.size();

  CQChartsDataModel *model = new CQChartsDataModel(charts_, nc, nr);

  CQDataModel *dataModel = model->dataModel();

  QModelIndex parent;

  for (int c = 0; c < nc; ++c) {
    const ColumnValues &columnValues = varColumns[c];

    for (int r = 0; r < nr; ++r) {
      QModelIndex ind = dataModel->index(r, c, parent);

      dataModel->setData(ind, columnValues[r]);
    }
  }

  return model;
#else
  int nc = vars.size();
  int nr = 100;

  CQChartsExprModel *model = new CQChartsExprModel(charts_, nc, nr);

  return model;
#endif
}

//------

void
CQChartsCmds::
foldModel(CQChartsModelData *modelData, const QString &str)
{
  foldClear(modelData);

  //---

  using FoldDatas = std::vector<CQFoldData>;

  FoldDatas foldDatas;

  QStringList strs = str.split(",", QString::SkipEmptyParts);

  for (int i = 0; i < strs.length(); ++i) {
    QStringList strs1 = strs[i].split(":", QString::SkipEmptyParts);

    if (strs1.length() == 0)
      continue;

    bool ok;

    int column = strs1[0].toInt(&ok);

    if (! ok)
      continue;

    CQFoldData foldData(column);

    if (strs1.length() > 1) {
      CQFoldData::Type type = CQFoldData::Type::REAL_RANGE;

      int i = 1;

      if (strs1.length() > 2) {
        if (strs1[1] == "i")
          type = CQFoldData::Type::INTEGER_RANGE;

        ++i;
      }

      double delta = strs1[i].toDouble(&ok);

      if (! ok)
        continue;

      foldData.setType (type);
      foldData.setDelta(delta);
    }

    foldDatas.push_back(foldData);
  }

  //---

  ModelP modelp = modelData->model;

  for (const auto &foldData : foldDatas) {
    QAbstractItemModel *model = modelp.data();

    CQFoldedModel *foldedModel = new CQFoldedModel(model, foldData);

    modelp = ModelP(foldedModel);

    modelData->foldedModels.push_back(modelp);
  }

  if (! modelData->foldedModels.empty()) {
    QAbstractItemModel *model = modelp.data();

    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(model);

    if (! proxyModel) {
      QSortFilterProxyModel *foldProxyModel = new QSortFilterProxyModel;

      foldProxyModel->setSourceModel(model);

      modelp = ModelP(foldProxyModel);
    }

    modelData->foldProxyModel = modelp;
  }
}

void
CQChartsCmds::
foldClear(CQChartsModelData *modelData)
{
  modelData->foldedModels.clear();

  modelData->foldProxyModel = ModelP();
}

//------

void
CQChartsCmds::
sortModel(ModelP &model, const QString &args)
{
  if (! args.length())
    return;

  QString columnStr = args.simplified();

  Qt::SortOrder order = Qt::AscendingOrder;

  if (columnStr[0] == '+' || columnStr[0] == '-') {
    order = (columnStr[0] == '+' ? Qt::AscendingOrder : Qt::DescendingOrder);

    columnStr = columnStr.mid(1);
  }

  CQChartsColumn column;

  if (stringToColumn(model, columnStr, column)) {
    if (column.type() == CQChartsColumn::Type::DATA)
      model->sort(column.column(), order);
  }
}

//------

bool
CQChartsCmds::
stringToColumn(const ModelP &model, const QString &str, CQChartsColumn &column) const
{
  CQChartsColumn column1(str);

  if (column1.isValid()) {
    column = column1;

    return true;
  }

  //---

  if (! str.length())
    return false;

  for (int column1 = 0; column1 < model->columnCount(); ++column1) {
    QVariant var = model->headerData(column1, Qt::Horizontal, Qt::DisplayRole);

    if (! var.isValid())
      continue;

    bool rc;

    QString str1 = CQChartsUtil::toString(var, rc);

    if (str1 == str) {
      column = CQChartsColumn(column1);
      return true;
    }
  }

  return false;
}

//------

CQChartsModelData *
CQChartsCmds::
addModelData(ModelP &model, bool hierarchical)
{
  int ind = modelDatas_.size() + 1;

  CQChartsModelData *modelData = new CQChartsModelData;

  modelData->ind          = ind;
  modelData->model        = model;
  modelData->hierarchical = hierarchical;

  modelDatas_.push_back(modelData);

  emit modelDataAdded(modelData->ind);

  setCurrentInd(ind);

  return modelDatas_.back();
}

CQChartsModelData *
CQChartsCmds::
getModelDataOrCurrent(int ind)
{
  if (ind >= 0)
    return getModelData(ind);

  return currentModelData();
}

CQChartsModelData *
CQChartsCmds::
getModelData(int ind)
{
  for (auto &modelData : modelDatas_) {
    if (modelData->ind == ind)
      return modelData;
  }

  return nullptr;
}

CQChartsModelData *
CQChartsCmds::
currentModelData()
{
  if (modelDatas_.empty())
    return nullptr;

  int ind = currentInd();

  if (ind >= 0 && ind < int(modelDatas_.size()))
    return modelDatas_[ind];

  return modelDatas_.back();
}

CQChartsView *
CQChartsCmds::
view() const
{
  return view_;
}

CQChartsView *
CQChartsCmds::
getView(bool reuse)
{
  if (reuse) {
    if (! view_)
      view_ = addView();
  }
  else {
    view_ = addView();
  }

  return view_;
}

CQChartsView *
CQChartsCmds::
addView()
{
  CQChartsView *view = charts_->addView();

  emit viewCreated(view);

  // TODO: handle multiple window
  CQChartsWindow *window = CQChartsWindowMgrInst->createWindow(view);

  emit windowCreated(window);

  return view;
}

CQChartsView *
CQChartsCmds::
currentView() const
{
  QStringList ids;

  charts_->getViewIds(ids);

  if (ids.empty())
    return nullptr;

  return charts_->getView(ids.back());
}

//------

bool
CQChartsCmds::
isCompleteLine(QString &str) const
{
  if (! str.length())
    return true;

  if (str[str.size() - 1] == '\\') {
    str = str.mid(0, str.length() - 1);
    return false;
  }

  //---

  CQStrParse line(str);

  line.skipSpace();

  while (! line.eof()) {
    if      (line.isChar('{')) {
      if (! line.skipBracedString())
        return false;
    }
    else if (line.isChar('\"') || line.isChar('\'')) {
      if (! line.skipString())
        return false;
    }
    else {
      line.skipNonSpace();
    }

    line.skipSpace();
  }

  return true;
}

void
CQChartsCmds::
parseLine(const QString &str)
{
  CQStrParse line(str);

  line.skipSpace();

  if (line.isChar('#'))
    return;

  QString cmd;

  line.readNonSpace(cmd);

  if (cmd == "")
    return;

  //---

  bool hasArgs    = true;
  bool keepQuotes = false;

  if (cmd == "@let")
    hasArgs = false;

  if (cmd == "@let" || cmd == "@print" || cmd == "@if" || cmd == "@while")
    keepQuotes = true;

  //---

  Args args;

  if (hasArgs) {
    while (! line.eof()) {
      line.skipSpace();

      if       (line.isChar('"') || line.isChar('\'')) {
        QString str1;

        if (! line.readString(str1, /*stripQuotes*/true))
          errorMsg("Invalid string '" + str1 + "'");

        str1 = CQChartsExpr::replaceStringVariables(expr(), str1);

        if (keepQuotes)
          str1 = "\"" + str1 + "\"";

        args.push_back(str1);
      }
      else if (line.isChar('{')) {
        QString str1;

        if (! line.readBracedString(str1, /*includeBraces*/false))
          errorMsg("Invalid braced string '" + str1 + "'");

        args.push_back(str1);
      }
      else {
        QString arg;

        if (line.readNonSpace(arg))
          args.push_back(arg);
      }
    }
  }
  else {
    line.skipSpace();

    QString arg = line.getAt();

    args.push_back(arg);
  }

  //---

  if (! processCmd(cmd, args))
    errorMsg("Invalid command '" + cmd + "'");
}

//------

CQChartsCmdsSlot::
CQChartsCmdsSlot(CQChartsCmds *cmds, CQChartsView *view, CQChartsPlot *plot,
                 const QString &procName) :
 cmds_(cmds), view_(view), plot_(plot), procName_(procName)
{
}

void
CQChartsCmdsSlot::
objIdPressed(const QString &id)
{
  std::string cmd = getCmd(id);

#ifdef CQ_CHARTS_CEIL
  ClLanguageMgrInst->runCommand(cmd);
#else
  std::cerr << cmd << "\n";
#endif
}

void
CQChartsCmdsSlot::
annotationIdPressed(const QString &id)
{
  std::string cmd = getCmd(id);

#ifdef CQ_CHARTS_CEIL
  ClLanguageMgrInst->runCommand(cmd);
#else
  std::cerr << cmd << "\n";
#endif
}

std::string
CQChartsCmdsSlot::
getCmd(const QString &id) const
{
  std::string cmd;

  QString viewName = view_->id();

  if (plot_) {
    QString plotName = plot_->id();

    cmd = procName_.toStdString() + "(" +
           "\"" + viewName.toStdString() + "\", " +
           "\"" + plotName.toStdString() + "\", " +
           "\"" + id      .toStdString() + "\")";
  }
  else
    cmd = procName_.toStdString() + "(" +
           "\"" + viewName.toStdString() + "\", " +
           "\"" + id      .toStdString() + "\")";

  return cmd;
}
