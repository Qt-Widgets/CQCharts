#include <CQChartsTest.h>
#include <CQCharts.h>
#include <CQChartsTable.h>
#include <CQChartsTree.h>
#include <CQChartsCsv.h>
#include <CQChartsTsv.h>
#include <CQChartsJson.h>
#include <CQChartsGnuData.h>
#include <CQDataModel.h>
#include <CQExprModel.h>
#include <CQChartsWindow.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQChartsPlotObj.h>
#include <CQChartsAxis.h>
#include <CQChartsKey.h>

#include <CQChartsLoader.h>
#include <CQChartsPlotDlg.h>
#include <CQSortModel.h>

//#define CQ_APP_H 1

#ifdef CQ_APP_H
#include <CQApp.h>
#else
#include <QApplication>
#endif

#include <CQUtil.h>

#include <QSortFilterProxyModel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <CReadLine.h>
#include <CStrParse.h>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QByteArray localMsg = msg.toLocal8Bit();

  switch (type) {
    case QtDebugMsg:
      fprintf(stderr, "Debug: %s (%s:%u, %s)\n",
              localMsg.constData(), context.file, context.line, context.function);
      break;
    case QtInfoMsg:
      fprintf(stderr, "Info: %s (%s:%u, %s)\n",
              localMsg.constData(), context.file, context.line, context.function);
      break;
    case QtWarningMsg:
      fprintf(stderr, "Warning: %s (%s:%u, %s)\n",
              localMsg.constData(), context.file, context.line, context.function);
      break;
    case QtCriticalMsg:
      fprintf(stderr, "Critical: %s (%s:%u, %s)\n",
              localMsg.constData(), context.file, context.line, context.function);
      break;
    case QtFatalMsg:
      fprintf(stderr, "Fatal: %s (%s:%u, %s)\n",
              localMsg.constData(), context.file, context.line, context.function);
      abort();
  }
}

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

}

//----

int
main(int argc, char **argv)
{
  qInstallMessageHandler(myMessageOutput);

#ifdef CQ_APP_H
  CQApp app(argc, argv);
#else
  QApplication app(argc, argv);
#endif

  CQUtil::initProperties();

  std::vector<CQChartsTest::InitData> initDatas;

  QString viewTitle;

  bool overlay = false;
  bool y1y2    = false;
  bool loop    = false;

  CQChartsTest::OptReal xmin1 = boost::make_optional(false, 0.0);
  CQChartsTest::OptReal xmax1 = boost::make_optional(false, 0.0);
  CQChartsTest::OptReal xmin2 = boost::make_optional(false, 0.0);
  CQChartsTest::OptReal xmax2 = boost::make_optional(false, 0.0);
  CQChartsTest::OptReal ymin1 = boost::make_optional(false, 0.0);
  CQChartsTest::OptReal ymax1 = boost::make_optional(false, 0.0);
  CQChartsTest::OptReal ymin2 = boost::make_optional(false, 0.0);
  CQChartsTest::OptReal ymax2 = boost::make_optional(false, 0.0);

  CQChartsTest::InitData initData;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      std::string arg = &argv[i][1];

      // input data type
      if      (arg == "csv")
        initData.fileType = CQChartsTest::FileType::CSV;
      else if (arg == "tsv")
        initData.fileType = CQChartsTest::FileType::TSV;
      else if (arg == "json")
        initData.fileType = CQChartsTest::FileType::JSON;
      else if (arg == "data")
        initData.fileType = CQChartsTest::FileType::DATA;
      else if (arg == "expr")
        initData.fileType = CQChartsTest::FileType::EXPR;

      // input data control
      else if (arg == "comment_header")
        initData.inputData.commentHeader = true;
      else if (arg == "first_line_header")
        initData.inputData.firstLineHeader = true;
      else if (arg == "num_rows") {
        ++i;

        if (i < argc)
          initData.inputData.numRows = std::max(atoi(argv[i]), 1);
      }
      else if (arg == "filter") {
        ++i;

        if (i < argc)
          initData.inputData.filter = argv[i];
      }

      // process data
      else if (arg == "process") {
        ++i;

        if (i < argc) {
          if (initData.process.length())
            initData.process += ";";

          initData.process += argv[i];
        }
      }
      // sort data
      else if (arg == "sort") {
        ++i;

        if (i < argc)
          initData.inputData.sort = argv[i];
      }

      // plot type
      else if (arg == "type") {
        ++i;

        if (i < argc)
          initData.typeName = argv[i];
      }

      // plot filter
      else if (arg == "where") {
        ++i;

        if (i < argc)
          initData.filterStr = argv[i];
      }

      // plot columns
      else if (arg == "column" || arg == "columns") {
        ++i;

        if (i < argc) {
          QString columnsStr = argv[i];

          QStringList strs = columnsStr.split(",", QString::SkipEmptyParts);

          for (int j = 0; j < strs.size(); ++j) {
            const QString &nameValue = strs[j];

            auto pos = nameValue.indexOf('=');

            if (pos >= 0) {
              auto name  = nameValue.mid(0, pos).simplified();
              auto value = nameValue.mid(pos + 1).simplified();

              initData.setNameValue(name, value);
            }
            else {
              std::cerr << "Invalid " << arg << " option '" << argv[i] << "'\n";
            }
          }
        }
      }
      else if (arg == "x") {
        ++i;

        if (i < argc)
          initData.setNameValue("x", argv[i]);
      }
      else if (arg == "y") {
        ++i;

        if (i < argc)
          initData.setNameValue("y", argv[i]);
      }
      else if (arg == "z") {
        ++i;

        if (i < argc)
          initData.setNameValue("z", argv[i]);
      }

      // plot bool parameters
      else if (arg == "bool") {
        ++i;

        if (i < argc) {
          QString boolStr = argv[i];

          QStringList strs = boolStr.split(",", QString::SkipEmptyParts);

          for (int j = 0; j < strs.size(); ++j) {
            const QString &nameValue = strs[j];

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
              initData.setNameBool(name, b);
            else {
              std::cerr << "Invalid -bool option '" << argv[i] << "'\n";
            }
          }
        }
      }
      else if (arg == "string") {
        ++i;

        if (i < argc) {
          QString stringStr = argv[i];

          QStringList strs = stringStr.split(",", QString::SkipEmptyParts);

          for (int j = 0; j < strs.size(); ++j) {
            const QString &nameValue = strs[j];

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

            initData.setNameString(name, value);
          }
        }
      }
      else if (arg == "real") {
        ++i;

        if (i < argc) {
          QString realStr = argv[i];

          QStringList strs = realStr.split(",", QString::SkipEmptyParts);

          for (int j = 0; j < strs.size(); ++j) {
            const QString &nameValue = strs[j];

            auto pos = nameValue.indexOf('=');

            QString name, value;

            if (pos >= 0) {
              name  = nameValue.mid(0, pos).simplified();
              value = nameValue.mid(pos + 1).simplified();
            }
            else {
              name  = nameValue;
              value = "0.0";
            }

            bool ok;

            double r = value.toDouble(&ok);

            if (ok)
              initData.setNameReal(name, r);
            else {
              std::cerr << "Invalid -bool option '" << argv[i] << "'\n";
            }
          }
        }
      }
      else if (arg == "bivariate") {
        initData.setNameBool("bivariate", true);
      }
      else if (arg == "stacked") {
        initData.setNameBool("stacked", true);
      }
      else if (arg == "cumulative") {
        initData.setNameBool("cumulative", true);
      }
      else if (arg == "fillunder") {
        initData.setNameBool("fillUnder", true);
      }
      else if (arg == "impulse") {
        initData.setNameBool("impulse", true);
      }

      // column types
      else if (arg == "column_type") {
        ++i;

        if (i < argc)
          initData.columnType = argv[i];
      }

      // axis type
      else if (arg == "xintegral") {
        initData.xintegral = true;
      }
      else if (arg == "yintegral") {
        initData.yintegral = true;
      }

      // title
      else if (arg == "view_title") {
        ++i;

        if (i < argc)
          viewTitle = argv[i];
      }

      else if (arg == "plot_title") {
        ++i;

        if (i < argc)
          initData.title = argv[i];
      }

      // plot properties
      else if (arg == "properties") {
        ++i;

        if (i < argc) {
          if (initData.properties.length())
            initData.properties += ",";

          initData.properties += argv[i];
        }
      }

      // plot chaining (overlay, y1y2, and)
      else if (arg == "overlay") {
        overlay = true;
      }
      else if (arg == "y1y2") {
        y1y2 = true;
      }
      else if (arg == "and") {
        initDatas.push_back(initData);

        if (! overlay) {
          xmin1 = boost::make_optional(false, 0.0);
          xmax1 = boost::make_optional(false, 0.0);
          xmin2 = boost::make_optional(false, 0.0);
          xmax2 = boost::make_optional(false, 0.0);
          ymin1 = boost::make_optional(false, 0.0);
          ymax1 = boost::make_optional(false, 0.0);
          ymin2 = boost::make_optional(false, 0.0);
          ymax2 = boost::make_optional(false, 0.0);
        }

        initData = CQChartsTest::InitData();
      }

      // data range
      else if (arg == "xmin" || arg == "xmin1") {
        ++i;

        if (i < argc)
          xmin1 = std::stod(argv[i]);
      }
      else if (arg == "xmin2") {
        ++i;

        if (i < argc)
          xmin2 = std::stod(argv[i]);
      }
      else if (arg == "xmax" || arg == "xmax1") {
        ++i;

        if (i < argc)
          xmax1 = std::stod(argv[i]);
      }
      else if (arg == "xmax2") {
        ++i;

        if (i < argc)
          xmax2 = std::stod(argv[i]);
      }
      else if (arg == "ymin" || arg == "ymin1") {
        ++i;

        if (i < argc)
          ymin1 = std::stod(argv[i]);
      }
      else if (arg == "ymin2") {
        ++i;

        if (i < argc)
          ymin2 = std::stod(argv[i]);
      }
      else if (arg == "ymax" || arg == "ymax1") {
        ++i;

        if (i < argc)
          ymax1 = std::stod(argv[i]);
      }
      else if (arg == "ymax2") {
        ++i;

        if (i < argc)
          ymax2 = std::stod(argv[i]);
      }

      // prompt loop
      else if (arg == "loop") {
        loop = true;
      }
      else {
        std::cerr << "Invalid option '" << argv[i] << "'\n";
      }
    }
    else {
      initData.filenames.push_back(argv[i]);
    }
  }

  initDatas.push_back(initData);

  //---

  CQChartsTest test;

  int nd = initDatas.size();

  int nr = std::max(int(sqrt(nd)), 1);
  int nc = (nd + nr - 1)/nr;

  double vr = CQChartsView::viewportRange();

  double dx = vr/nc;
  double dy = vr/nr;

  int i = 0;

  for (auto &initData : initDatas) {
    initData.overlay = overlay;
    initData.y1y2    = y1y2;
    initData.nr      = nr;
    initData.nc      = nc;
    initData.dx      = dx;
    initData.dy      = dy;

    //---

    if (xmin1) initData.xmin = xmin1;
    if (xmax1) initData.xmax = xmax1;

    if (initData.y1y2) {
      if      (i == 0) {
        if (ymin1) initData.ymin = ymin1;
        if (ymax1) initData.ymax = ymax1;
      }
      else if (i >= 1) {
        if (ymin2) initData.ymin = ymin2;
        if (ymax2) initData.ymax = ymax2;
      }
    }
    else {
      if (ymin1) initData.ymin = ymin1;
      if (ymax1) initData.ymax = ymax1;
    }

    //---

    if (! test.initPlot(initData))
      continue;

    ++i;
  }

  //---

  if (test.view()) {
    if (viewTitle.length())
      test.view()->setTitle(viewTitle);

    if (overlay)
      test.view()->initOverlay();
  }

  //---

  test.show();

  if (test.window()) {
    test.window()->raise();
  }

  //---

  if (! loop)
    app.exec();
  else
    test.loop();

  return 0;
}

//-----

CQChartsTest::
CQChartsTest() :
 CQAppWindow()
{
  charts_ = new CQCharts;

  charts_->init();

  //---

  addMenus();

  //---

  QVBoxLayout *layout = CQUtil::makeLayout<QVBoxLayout>(centralWidget(), 0, 0);

  //---

  viewTab_ = CQUtil::makeWidget<QTabWidget>("viewTab");

  layout->addWidget(viewTab_);

  //---

  QGroupBox *exprGroup = CQUtil::makeWidget<QGroupBox>("exprGroup");

  exprGroup->setTitle("Expression");

  layout->addWidget(exprGroup);

  QHBoxLayout *exprGroupLayout = new QHBoxLayout(exprGroup);

  exprEdit_ = CQUtil::makeWidget<QLineEdit>("exprEdit");

  exprGroupLayout->addWidget(exprEdit_);

  connect(exprEdit_, SIGNAL(returnPressed()), this, SLOT(exprSlot()));

  //---

  QGroupBox *typeGroup = CQUtil::makeWidget<QGroupBox>("typeGroup");

  typeGroup->setTitle("Column Type");

  layout->addWidget(typeGroup);

  QVBoxLayout *typeGroupLayout = new QVBoxLayout(typeGroup);

  QFrame *columnFrame = CQUtil::makeWidget<QFrame>("columnFrame");

  typeGroupLayout->addWidget(columnFrame);

  QGridLayout *columnLayout = new QGridLayout(columnFrame);

  int row = 0;

  columnNumEdit_  = addLineEdit(columnLayout, row, "Number", "number");
  columnTypeEdit_ = addLineEdit(columnLayout, row, "Type"  , "type"  );

  QPushButton *typeOKButton = new QPushButton("Set");

  typeOKButton->setObjectName("typeOK");

  connect(typeOKButton, SIGNAL(clicked()), this, SLOT(typeOKSlot()));

  columnLayout->addWidget(typeOKButton);
}

CQChartsTest::
~CQChartsTest()
{
  delete charts_;
  delete loader_;
}

//------

void
CQChartsTest::
addMenus()
{
  QMenuBar *menuBar = addMenuBar();

  QMenu *fileMenu = menuBar->addMenu("&File");
  QMenu *plotMenu = menuBar->addMenu("&Plot");
  QMenu *helpMenu = menuBar->addMenu("&Help");

  QAction *loadAction   = new QAction("Load"  , menuBar);
  QAction *createAction = new QAction("Create", menuBar);
  QAction *helpAction   = new QAction("Help"  , menuBar);

  connect(loadAction  , SIGNAL(triggered()), this, SLOT(loadSlot()));
  connect(createAction, SIGNAL(triggered()), this, SLOT(createSlot()));

  fileMenu->addAction(loadAction);
  plotMenu->addAction(createAction);
  helpMenu->addAction(helpAction);
}

//------

CQChartsTest::ViewData &
CQChartsTest::
addViewData(bool hierarchical)
{
  int ind = viewDatas_.size() + 1;

  //---

  QFrame *viewFrame = CQUtil::makeWidget<QFrame>(QString("viewFrame %1").arg(ind));

  QVBoxLayout *viewLayout = new QVBoxLayout(viewFrame);

  viewTab_->addTab(viewFrame, QString("View %1").arg(ind));

  viewTab_->setCurrentIndex(viewTab_->count() - 1);

  //---

  ViewData viewData;

  //---

  viewData.filterEdit = CQUtil::makeWidget<QLineEdit>("filter");

  viewLayout->addWidget(viewData.filterEdit);

  connect(viewData.filterEdit, SIGNAL(returnPressed()), this, SLOT(filterSlot()));

  //---

  QSplitter *splitter = CQUtil::makeWidget<QSplitter>("splitter");

  splitter->setOrientation(Qt::Vertical);

  viewLayout->addWidget(splitter);

  //---

  if (hierarchical) {
    viewData.tree = new CQChartsTree;

    splitter->addWidget(viewData.tree);
  }
  else {
    viewData.table = new CQChartsTable;

    splitter->addWidget(viewData.table);
  }

  connect(viewData.table, SIGNAL(columnClicked(int)), this, SLOT(tableColumnClicked(int)));

  //---

  viewData.detailsText = CQUtil::makeWidget<QTextEdit>("detailsText");

  viewData.detailsText->setReadOnly(true);

  splitter->addWidget(viewData.detailsText);

  //---

  viewDatas_.push_back(viewData);

  return viewDatas_.back();
}

//------

bool
CQChartsTest::
initPlot(const InitData &initData)
{
  int i = plots_.size();

  //---

  setId(QString("%1").arg(i + 1));

  int r = i / initData.nc;
  int c = i % initData.nc;

  double vr = CQChartsView::viewportRange();

  if (initData.overlay || initData.y1y2) {
    setBBox(CQChartsGeom::BBox(0, 0, vr, vr));
  }
  else {
    double x1 =  c     *initData.dx;
    double x2 = (c + 1)*initData.dx;
    double y1 =  r     *initData.dy;
    double y2 = (r + 1)*initData.dy;

    setBBox(CQChartsGeom::BBox(x1, vr - y2, x2, vr - y1));
  }

  //---

  QString filename;

  if (initData.filenames.size() > 0) {
    filename = initData.filenames[0];

    if (initData.fileType != FileType::NONE) {
      if (! loadFileModel(filename, initData.fileType, initData.inputData))
        return false;
    }
    else {
      std::cerr << "No file type specified\n";
    }
  }
  else {
    if (initData.fileType == FileType::EXPR) {
      if (! loadFileModel("", initData.fileType, initData.inputData))
        return false;
    }
  }

  //---

  if (viewDatas_.empty())
    return false;

  ViewData &viewData = currentViewData();

  ModelP model = viewData.model;

  //---

  QStringList strs = initData.process.split(";", QString::SkipEmptyParts);

  for (int i = 0; i < strs.size(); ++i)
    processExpression(strs[i]);

  //---

  CQChartsPlot *plot = initPlotView(viewData, initData, i);

  if (! plot)
    return false;

  if (filename != "")
    plot->setFileName(filename);

  //---

  if (plots_.empty())
    rootPlot_ = plot;

  if      (initData.overlay) {
    plot->setOverlay(true);

    if      (i == 0) {
    }
    else if (i >= 1) {
      CQChartsPlot *prevPlot = plots_.back();

      plot    ->setPrevPlot(prevPlot);
      prevPlot->setNextPlot(plot);

      plot->setDataRange(rootPlot_->dataRange());

      rootPlot_->applyDataRange();
    }
  }
  else if (initData.y1y2) {
    if      (i == 0) {
    }
    else if (i >= 1) {
      CQChartsPlot *prevPlot = plots_.back();

      plot    ->setPrevPlot(prevPlot);
      prevPlot->setNextPlot(plot);

      plot->xAxis()->setVisible(false);
      plot->yAxis()->setSide(CQChartsAxis::Side::TOP_RIGHT);

      plot->key()->setVisible(false);
    }
  }

  //---

  plots_.push_back(plot);

  return true;
}

//------

void
CQChartsTest::
loadSlot()
{
  if (! loader_) {
    loader_ = new CQChartsLoader(charts_);

    connect(loader_, SIGNAL(loadFile(const QString &, const QString &)),
            this, SLOT(loadFileSlot(const QString &, const QString &)));
  }

  loader_->show();
}

bool
CQChartsTest::
loadFileSlot(const QString &type, const QString &filename)
{
  FileType fileType = stringToFileType(type);

  if (fileType == FileType::NONE) {
    std::cerr << "Bad type specified '" << type.toStdString() << "'\n";
    return false;
  }

  //---

  InputData inputData;

  inputData.commentHeader   = loader_->isCommentHeader();
  inputData.firstLineHeader = loader_->isFirstLineHeader();

  return loadFileModel(filename, fileType, inputData);
}

//------

void
CQChartsTest::
createSlot()
{
  if (viewDatas_.empty())
    return;

  ViewData &viewData = currentViewData();

  ModelP model = viewData.model;

  //---

  CQChartsPlotDlg *dlg = new CQChartsPlotDlg(charts_, model);

  if (viewData.sm)
    dlg->setSelectionModel(viewData.sm);

  connect(dlg, SIGNAL(plotCreated(CQChartsPlot *)),
          this, SLOT(plotDialogCreatedSlot(CQChartsPlot *)));

  if (! dlg->exec())
    return;

  //---

  delete dlg;
}

void
CQChartsTest::
plotDialogCreatedSlot(CQChartsPlot *plot)
{
  connect(plot, SIGNAL(objPressed(CQChartsPlotObj *)),
          this, SLOT(plotObjPressedSlot(CQChartsPlotObj *)));
}

//------

void
CQChartsTest::
plotObjPressedSlot(CQChartsPlotObj *obj)
{
  QString id = obj->id();

  if (id.length())
    std::cerr << id.toStdString() << "\n";
}

//------

void
CQChartsTest::
filterSlot()
{
  QLineEdit *filterEdit = qobject_cast<QLineEdit *>(sender());

  for (auto &viewData : viewDatas_) {
    if (viewData.filterEdit == filterEdit) {
      if (viewData.table)
        viewData.table->setFilter(filterEdit->text());
      else
        viewData.tree->setFilter(filterEdit->text());
    }
  }
}

//------

void
CQChartsTest::
exprSlot()
{
  QString text = exprEdit_->text();

  processExpression(text);

  exprEdit_->setText("");
}

void
CQChartsTest::
processExpression(const QString &expr)
{
  if (! expr.length())
    return;

  //---

  if (viewDatas_.empty())
    return;

  ViewData &viewData = currentViewData();

  ModelP model = viewData.model;

  //---

  CQExprModel *exprModel = qobject_cast<CQExprModel *>(model.data());

  if (! exprModel) {
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(model.data());

    if (proxyModel)
      exprModel = qobject_cast<CQExprModel *>(proxyModel->sourceModel());
  }

  if (! exprModel) {
    std::cerr << "Expression not supported for model\n";
    return;
  }

  //---

  // add column <expr>
  if      (expr[0] == '+')
    exprModel->addExtraColumn(expr.mid(1));
  // delete column <n>
  else if (expr[0] == '-') {
    QString columnStr = expr.mid(1).simplified();

    bool ok;

    int column = columnStr.toInt(&ok);

    if (! ok) {
      std::cerr << "Invalid column number '" << columnStr.toStdString() << "'\n";
      return;
    }

    bool rc = exprModel->removeExtraColumn(column);

    if (! rc) {
      std::cerr << "Failed to delete column '" << column << "'\n";
      return;
    }
  }
  else if (expr[0] == '=') {
    QString columnExprStr = expr.mid(1);

    int pos = columnExprStr.indexOf(':');

    if (pos < 0) {
      std::cerr << "Invalid assign expression '" << columnExprStr.toStdString() << "'\n";
      return;
    }

    QString columnStr = columnExprStr.mid(0, pos).simplified();
    QString exprStr   = columnExprStr.mid(pos + 1).simplified();

    bool ok;

    int column = columnStr.toInt(&ok);

    if (! ok) {
      std::cerr << "Invalid column number '" << columnStr.toStdString() << "'\n";
      return;
    }

    exprModel->assignExtraColumn(column, exprStr);
  }
  else
    exprModel->processExpr(expr);
}

//------

void
CQChartsTest::
tableColumnClicked(int column)
{
  CQChartsTable *table = qobject_cast<CQChartsTable *>(sender());

  ModelP model = table->model();

  columnNumEdit_->setText(QString("%1").arg(column));

  CQChartsColumnTypeMgr *columnTypeMgr = charts_->columnTypeMgr();

  CQBaseModel::Type  type;
  CQChartsNameValues nameValues;

  columnTypeMgr->getModelColumnType(model.data(), column, type, nameValues);

  QString typeStr = columnTypeMgr->encodeTypeData(type, nameValues);

  if (typeStr.length())
    columnTypeEdit_->setText(typeStr);
}

//------

void
CQChartsTest::
typeOKSlot()
{
  if (viewDatas_.empty())
    return;

  ViewData &viewData = currentViewData();

  ModelP model = viewData.model;

  //---

  QString typeStr = columnTypeEdit_->text();

  CQChartsColumnTypeMgr *columnTypeMgr = charts_->columnTypeMgr();

  CQChartsNameValues nameValues;

  CQChartsColumnType *typeData = columnTypeMgr->decodeTypeData(typeStr, nameValues);

  if (! typeData) {
    std::cerr << "Invalid type '" << typeStr.toStdString() << "'\n";
    return;
  }

  QString numStr = columnNumEdit_->text();

  int column = numStr.toInt();

  columnTypeMgr->setModelColumnType(model.data(), column, typeData->type(), nameValues);
}

//------

CQChartsTest::ViewData &
CQChartsTest::
currentViewData()
{
  assert(! viewDatas_.empty());

  int ind = viewTab_->currentIndex();

  if (ind >= 0 && ind < int(viewDatas_.size()))
    return viewDatas_[ind];
  else
    return viewDatas_.back();
}

//------

CQChartsPlot *
CQChartsTest::
initPlotView(const ViewData &viewData, const InitData &initData, int i)
{
  setColumnFormats(viewData.model, initData.columnType);

  //---

  QString typeName = fixTypeName(initData.typeName);

  if (typeName == "")
    return nullptr;

  // ignore if bad type
  CQChartsPlotType *type = charts_->plotType(typeName);

  if (! type) {
    std::cerr << "Invalid type '" << typeName.toStdString() << "' for plot\n";
    return nullptr;
  }

  //---

  // result plot if needed
  bool reuse = false;

  if (initData.overlay || initData.y1y2) {
    if (typeName == "xy" || typeName == "barchart")
      reuse = true;
  }
  else {
    reuse = true;
  }

  //---

  CQChartsPlot *plot = nullptr;

  // create plot from init (argument) data
  if (initData.filterStr.length()) {
    CQSortModel *sortModel = new CQSortModel(viewData.model.data());

    ModelP sortModelP(sortModel);

    sortModel->setFilter(initData.filterStr);

    plot = createPlot(viewData, sortModelP, type, initData.nameValueData, reuse);
  }
  else {
    plot = createPlot(viewData, viewData.model, type, initData.nameValueData, reuse);
  }

  assert(plot);

  //---

  // init plot
  if (initData.overlay || initData.y1y2) {
    if (i > 0) {
      plot->setBackground    (false);
      plot->setDataBackground(false);
    }
  }

  if (initData.title != "")
    plot->setTitle(initData.title);

  if (initData.xintegral)
    plot->xAxis()->setIntegral(true);

  if (initData.yintegral)
    plot->yAxis()->setIntegral(true);

  if (initData.xmin) plot->setXMin(initData.xmin);
  if (initData.ymin) plot->setYMin(initData.ymin);
  if (initData.xmax) plot->setXMax(initData.xmax);
  if (initData.ymax) plot->setYMax(initData.ymax);

  //---

  if (initData.properties != "")
    setPlotProperties(plot, initData.properties);

  return plot;
}

void
CQChartsTest::
setColumnFormats(const ModelP &model, const QString &columnType)
{
  // split into multiple column type definitions
  QStringList fstrs = columnType.split(";", QString::KeepEmptyParts);

  for (int i = 0; i < fstrs.length(); ++i) {
    QString typeStr = fstrs[i].simplified();

    if (! typeStr.length())
      continue;

    // default column to index
    int column = i;

    // if #<col> then use that for column index
    int pos = typeStr.indexOf("#");

    if (pos >= 0) {
      QString columnStr = typeStr.mid(0, pos).simplified();

      int column1;

      if (stringToColumn(model, columnStr, column1))
        column = column1;
      else
        std::cerr << "Bad column name '" << columnStr.toStdString() << "'\n";

      typeStr = typeStr.mid(pos + 1).simplified();
    }

    //---

    CQChartsColumnTypeMgr *columnTypeMgr = charts_->columnTypeMgr();

    // decode to type name and name values
    CQChartsNameValues nameValues;

    CQChartsColumnType *typeData = columnTypeMgr->decodeTypeData(typeStr, nameValues);

    if (! typeData) {
      std::cerr << "Invalid type '" << typeStr.toStdString() <<
                   "' for section '" << column << "'\n";
      continue;
    }

    // store in model
    if (! columnTypeMgr->setModelColumnType(model.data(), column, typeData->type(), nameValues)) {
      std::cerr << "Failed to set column type '" << typeStr.toStdString() <<
                   "' for section '" << column << "'\n";
      continue;
    }
  }
}

QString
CQChartsTest::
fixTypeName(const QString &typeName) const
{
  QString typeName1 = typeName;

  // adjust typename for alias (TODO: add to typeData)
  if      (typeName1 == "piechart")
    typeName1 = "pie";
  else if (typeName1 == "xyplot")
    typeName1 = "xy";
  else if (typeName1 == "scatterplot")
    typeName1 = "scatter";
  else if (typeName1 == "bar")
    typeName1 = "barchart";
  else if (typeName1 == "boxplot")
    typeName1 = "box";
  else if (typeName1 == "parallelplot")
    typeName1 = "parallel";
  else if (typeName1 == "geometryplot")
    typeName1 = "geometry";
  else if (typeName1 == "delaunayplot")
    typeName1 = "delaunay";
  else if (typeName1 == "adjacencyplot")
    typeName1 = "adjacency";

  return typeName1;
}

CQChartsPlot *
CQChartsTest::
createPlot(const ViewData &viewData, const ModelP &model, CQChartsPlotType *type,
           const NameValueData &nameValueData, bool reuse)
{
  CQChartsView *view = getView(reuse);

  //---

  CQChartsPlot *plot = type->create(view, model);

  if (viewData.sm)
    plot->setSelectionModel(viewData.sm);

  connect(plot, SIGNAL(objPressed(CQChartsPlotObj *)),
          this, SLOT(plotObjPressedSlot(CQChartsPlotObj *)));

  //---

  // set plot property for widgets for plot parameters
  for (const auto &parameter : type->parameters()) {
    if      (parameter.type() == "column") {
      auto p = nameValueData.values.find(parameter.name());

      if (p == nameValueData.values.end())
        continue;

      int column;

      if (! stringToColumn(model, (*p).second, column)) {
        std::cerr << "Bad column name '" << (*p).second.toStdString() << "'\n";
        column = -1;
      }

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(column)))
        std::cerr << "Failed to set parameter " << parameter.propName().toStdString() << "\n";
    }
    else if (parameter.type() == "columns") {
      auto p = nameValueData.values.find(parameter.name());

      if (p == nameValueData.values.end())
        continue;

      QStringList strs = (*p).second.split(" ", QString::SkipEmptyParts);

      std::vector<int> columns;

      for (int j = 0; j < strs.size(); ++j) {
        int column;

        if (! stringToColumn(model, strs[j], column)) {
          std::cerr << "Bad column name '" << strs[j].toStdString() << "'\n";
          continue;
        }

        columns.push_back(column);
      }

      QString s = CQChartsUtil::toString(columns);

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(s)))
        std::cerr << "Failed to set parameter " << parameter.propName().toStdString() << "\n";
    }
    else if (parameter.type() == "string") {
      auto p = nameValueData.strings.find(parameter.name());

      if (p == nameValueData.strings.end())
        continue;

      QString str = (*p).second;

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(str)))
        std::cerr << "Failed to set parameter " << parameter.propName().toStdString() << "\n";
    }
    else if (parameter.type() == "real") {
      auto p = nameValueData.reals.find(parameter.name());

      if (p == nameValueData.reals.end())
        continue;

      double r = (*p).second;

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(r)))
        std::cerr << "Failed to set parameter " << parameter.propName().toStdString() << "\n";
    }
    else if (parameter.type() == "bool") {
      auto p = nameValueData.bools.find(parameter.name());

      if (p == nameValueData.bools.end())
        continue;

      bool b = (*p).second;

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(b)))
        std::cerr << "Failed to set parameter " << parameter.propName().toStdString() << "\n";
    }
    else
      assert(false);
  }

  //---

  // add plot to view and show
  plot->setId(QString("%1%2").arg(plot->typeName()).arg(id_));

  view->addPlot(plot, bbox_);

  return plot;
}

void
CQChartsTest::
setPlotProperties(CQChartsPlot *plot, const QString &properties)
{
  QStringList strs = properties.split(",", QString::SkipEmptyParts);

  for (int i = 0; i < strs.size(); ++i) {
    QString str = strs[i].simplified();

    int pos = str.indexOf("=");

    QString name  = str.mid(0, pos).simplified();
    QString value = str.mid(pos + 1).simplified();

    if (! plot->setProperty(name, value))
      std::cerr << "Failed to set property " << name.toStdString() << "\n";
  }
}

//------

bool
CQChartsTest::
loadFileModel(const QString &filename, FileType type, const InputData &inputData)
{
  bool hierarchical;

  QAbstractItemModel *model = loadFile(filename, type, inputData, hierarchical);

  if (! model)
    return false;

  ViewData &viewData = addViewData(hierarchical);

  viewData.model = ModelP(model);

  //---

  if (inputData.sort.simplified().length()) {
    QString columnStr = inputData.sort.simplified();

    Qt::SortOrder order = Qt::AscendingOrder;

    if (columnStr[0] == '+' || columnStr[0] == '-') {
      order = (columnStr[0] == '+' ? Qt::AscendingOrder : Qt::DescendingOrder);

      columnStr = columnStr.mid(1);
    }

    int column;

    if (stringToColumn(viewData.model, columnStr, column))
      viewData.model->sort(column, order);
  }

  //---

  if (hierarchical) {
    viewData.tree->setModel(viewData.model);

    viewData.sm = viewData.tree->selectionModel();
  }
  else {
    viewData.table->setModel(viewData.model);

    viewData.sm = viewData.table->selectionModel();
  }

  updateModelDetails(viewData);

  return true;
}

void
CQChartsTest::
updateModelDetails(const ViewData &viewData)
{
  int numColumns = viewData.model->columnCount();
  int numRows    = viewData.model->rowCount   ();

  QString text;

  text += QString("%1 Columns").arg(numColumns) + "\n";
  text += QString("%1 Rows"   ).arg(numRows);

  viewData.detailsText->setPlainText(text);
}

QAbstractItemModel *
CQChartsTest::
loadFile(const QString &filename, FileType type, const InputData &inputData, bool &hierarchical)
{
  hierarchical = false;

  QAbstractItemModel *model = nullptr;

  if      (type == FileType::CSV) {
    model = loadCsv(filename, inputData);
  }
  else if (type == FileType::TSV) {
    model = loadTsv(filename, inputData);
  }
  else if (type == FileType::JSON) {
    model = loadJson(filename, hierarchical);
  }
  else if (type == FileType::DATA) {
    model = loadData(filename, inputData);
  }
  else if (type == FileType::EXPR) {
    model = createExprModel(inputData.numRows);
  }
  else {
    std::cerr << "Bad file type specified '" <<
      fileTypeToString(type).toStdString() << "'\n";
    return nullptr;
  }

  return model;
}

QAbstractItemModel *
CQChartsTest::
loadCsv(const QString &filename, const InputData &inputData)
{
  CQChartsCsv *csv = new CQChartsCsv(charts_);

  csv->setCommentHeader  (inputData.commentHeader);
  csv->setFirstLineHeader(inputData.firstLineHeader);
  csv->setFilter         (inputData.filter);

  if (! csv->load(filename))
    std::cerr << "Failed to load " << filename.toStdString() << "\n";

  return csv;
}

QAbstractItemModel *
CQChartsTest::
loadTsv(const QString &filename, const InputData &inputData)
{
  CQChartsTsv *tsv = new CQChartsTsv(charts_);

  tsv->setCommentHeader  (inputData.commentHeader);
  tsv->setFirstLineHeader(inputData.firstLineHeader);
  tsv->setFilter         (inputData.filter);

  if (! tsv->load(filename))
    std::cerr << "Failed to load " << filename.toStdString() << "\n";

  return tsv;
}

QAbstractItemModel *
CQChartsTest::
loadJson(const QString &filename, bool &hierarchical)
{
  CQChartsJson *json = new CQChartsJson(charts_);

  if (! json->load(filename))
    std::cerr << "Failed to load " << filename.toStdString() << "\n";

  hierarchical = json->isHierarchical();

  return json;
}

QAbstractItemModel *
CQChartsTest::
loadData(const QString &filename, const InputData &inputData)
{
  CQChartsGnuData *data = new CQChartsGnuData(charts_);

  data->setCommentHeader  (inputData.commentHeader);
  data->setFirstLineHeader(inputData.firstLineHeader);

  if (! data->load(filename))
    std::cerr << "Failed to load " << filename.toStdString() << "\n";

  return data;
}

QAbstractItemModel *
CQChartsTest::
createExprModel(int n)
{
  int nc = 1;
  int nr = n;

  CQDataModel *model = new CQDataModel(nc, nr);

  for (int r = 0; r < nr; ++r) {
    for (int c = 0; c < nc; ++c) {
      QModelIndex ind = model->index(r, c);

      model->setData(ind, QVariant(r*nc + c));
    }
  }

  CQExprModel *exprModel = new CQExprModel(model);

  QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;

  proxyModel->setSourceModel(exprModel);

  return proxyModel;
}

//------

QLineEdit *
CQChartsTest::
addLineEdit(QGridLayout *grid, int &row, const QString &name, const QString &objName) const
{
  QLabel    *label = new QLabel(name);
  QLineEdit *edit  = new QLineEdit;

  label->setObjectName(objName + "Label");
  label->setObjectName(objName + "Edit" );

  grid->addWidget(label, row, 0);
  grid->addWidget(edit , row, 1);

  ++row;

  return edit;
}

bool
CQChartsTest::
stringToColumn(const ModelP &model, const QString &str, int &column) const
{
  bool ok = false;

  int column1 = str.toInt(&ok);

  if (ok) {
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

    if (var.toString() == str) {
      column = column1;
      return true;
    }
  }

  return false;
}

//------

CQChartsView *
CQChartsTest::
view() const
{
  return view_;
}

CQChartsView *
CQChartsTest::
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
CQChartsTest::
addView()
{
  CQChartsView *view = charts_->addView();

  window_ = new CQChartsWindow(view);

  window_->show();

  return view;
}

//------

QSize
CQChartsTest::
sizeHint() const
{
  return QSize(1024, 1024);
}

//------

CQChartsTest::FileType
CQChartsTest::
stringToFileType(const QString &str) const
{
  QString lstr = str.toLower();

  if      (lstr == "csv" ) return FileType::CSV;
  else if (lstr == "tsv" ) return FileType::TSV;
  else if (lstr == "json") return FileType::JSON;
  else if (lstr == "data") return FileType::DATA;
  else                     return FileType::NONE;
}

QString
CQChartsTest::
fileTypeToString(FileType type) const
{
  if      (type == FileType::CSV ) return "csv";
  else if (type == FileType::TSV ) return "tsv";
  else if (type == FileType::JSON) return "json";
  else if (type == FileType::DATA) return "data";
  else                             return "";
}

//------

class CQChartsReadLine : public CReadLine {
 public:
  CQChartsReadLine(CQChartsTest *test) :
   test_(test) {
  }

  void timeout() {
    test_->timeout();
  }

 private:
  CQChartsTest *test_;
};

void
CQChartsTest::
loop()
{
  CQChartsReadLine *readLine = new CQChartsReadLine(this);

  readLine->enableTimeoutHook(1);

  for (;;) {
    readLine->setPrompt("> ");

    auto line = readLine->readLine();

    while (line[line.size() - 1] == '\\') {
      readLine->setPrompt("+> ");

      auto line1 = readLine->readLine();

      line = line.substr(0, line.size() - 1) + line1;
    }

    parseLine(line);

    readLine->addHistory(line);
  }
}

void
CQChartsTest::
timeout()
{
  if (! qApp->activeModalWidget())
    qApp->processEvents();
}

void
CQChartsTest::
parseLine(const std::string &str)
{
  CStrParse line(str);

  line.skipSpace();

  std::string cmd;

  line.readNonSpace(cmd);

  Args args;

  while (! line.eof()) {
    line.skipSpace();

    if (line.isChar('"') || line.isChar('\'')) {
      std::string str;

      if (line.readString(str, /*strip_quotes*/true))
        args.push_back(str);
    }
    else {
      std::string arg;

      if (line.readNonSpace(arg))
        args.push_back(arg);
    }
  }

  if      (cmd == "exit") {
    exit(0);
  }
  else if (cmd == "set") {
    setCmd(args);
  }
  else if (cmd == "get") {
    getCmd(args);
  }
  else if (cmd == "load") {
    loadCmd(args);
  }
  else if (cmd == "plot") {
    plotCmd(args);
  }
  else if (cmd == "source") {
    sourceCmd(args);
  }
}

void
CQChartsTest::
setCmd(const Args & args)
{
  QString viewName;
  QString plotName;
  QString name;
  QString value;

  for (std::size_t i = 0; i < args.size(); ++i) {
    std::string arg = args[i];

    if (arg[0] == '-') {
      std::string opt = arg.substr(1);

      if      (opt == "view") {
        ++i;

        if (i < args.size())
          viewName = args[i].c_str();
      }
      else if (opt == "plot") {
        ++i;

        if (i < args.size())
          plotName = args[i].c_str();
      }
      else if (opt == "name") {
        ++i;

        if (i < args.size())
          name = args[i].c_str();
      }
      else if (opt == "value") {
        ++i;

        if (i < args.size())
          value = args[i].c_str();
      }
    }
  }

  //---

  CQChartsView *view = charts_->getView(viewName);

  if (! view) {
    QStringList ids;

    charts_->getViewIds(ids);

    if (ids.length())
      view = charts_->getView(ids[0]);
  }

  if (! view) {
    std::cerr << "No view '" << plotName.toStdString() << "'\n";
    return;
  }

  //---

  if (plotName != "") {
    CQChartsPlot *plot = view->getPlot(plotName);

    if (! plot) {
      std::cerr << "No plot '" << plotName.toStdString() << "'\n";
      return;
    }

    //---

    if (! plot->setProperty(name, value)) {
      std::cerr << "Failed to set view parameter '" << name.toStdString() << "'\n";
      return;
    }
  }
  else {
    if (! view->setProperty(name, value)) {
      std::cerr << "Failed to set plot parameter '" << name.toStdString() << "'\n";
      return;
    }
  }
}

void
CQChartsTest::
getCmd(const Args &)
{
}

bool
CQChartsTest::
loadCmd(const Args &args)
{
  QString   filename;
  FileType  fileType { FileType::NONE };
  InputData inputData;

  for (std::size_t i = 0; i < args.size(); ++i) {
    std::string arg = args[i];

    if (arg[0] == '-') {
      std::string opt = arg.substr(1);

      if      (opt == "csv")
        fileType = FileType::CSV;
      else if (opt == "tsv")
        fileType = FileType::TSV;
      else if (opt == "json")
        fileType = FileType::JSON;
      else if (opt == "data")
        fileType = FileType::DATA;
      else if (opt == "comment_header")
        inputData.commentHeader = true;
      else if (opt == "first_line_header")
        inputData.firstLineHeader = true;
    }
    else {
      if (filename == "")
        filename = arg.c_str();
    }
  }

  if (filename == "") {
    std::cerr << "No filename\n";
    return false;
  }

  if (fileType == FileType::NONE) {
    std::cerr << "No file type\n";
    return false;
  }

  return loadFileModel(filename, fileType, inputData);
}

void
CQChartsTest::
plotCmd(const Args &args)
{
  QString       typeName;
  NameValueData nameValueData;
  bool          xintegral { false };
  bool          yintegral { false };
  QString       title;
  QString       properties;
  OptReal       xmin, ymin, xmax, ymax;
  bool          y1y2      { false };
  bool          overlay   { false };

  for (std::size_t i = 0; i < args.size(); ++i) {
    std::string arg = args[i];

    if (arg[0] == '-') {
      std::string opt = arg.substr(1);

      if      (opt == "type") {
        ++i;

        if (i < args.size())
          typeName = args[i].c_str();
      }
      else if (opt == "column" || opt == "columns") {
        ++i;

        if (i < args.size()) {
          QString columnsStr = args[i].c_str();

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
              std::cerr << "Invalid " << opt << " option '" << args[i] << "'\n";
            }
          }
        }
      }
      else if (opt == "bool") {
        ++i;

        if (i < args.size()) {
          QString nameValue = args[i].c_str();

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
            std::cerr << "Invalid -bool option '" << args[i] << "'\n";
          }
        }
      }
      else if (opt == "string") {
        ++i;

        if (i < args.size()) {
          QString nameValue = args[i].c_str();

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
      else if (opt == "real") {
        ++i;

        if (i < args.size()) {
          QString nameValue = args[i].c_str();

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
      else if (opt == "xintegral") {
        xintegral = true;
      }
      else if (opt == "yintegral") {
        yintegral = true;
      }
      else if (opt == "title") {
        ++i;

        if (i < args.size())
          title = args[i].c_str();
      }
      else if (opt == "properties") {
        ++i;

        if (i < args.size())
          properties = args[i].c_str();
      }
      else if (opt == "overlay") {
        overlay = true;
      }
      else if (opt == "y1y2") {
        y1y2 = true;
      }
      else if (opt == "xmin") {
        ++i;

        if (i < args.size())
          xmin = std::stod(args[i]);
      }
      else if (opt == "xmax") {
        ++i;

        if (i < args.size())
          xmax = std::stod(args[i]);
      }
      else if (arg == "ymin") {
        ++i;

        if (i < args.size())
          ymin = std::stod(args[i]);
      }
      else if (arg == "ymax") {
        ++i;

        if (i < args.size())
          ymax = std::stod(args[i]);
      }
    }
    else {
    }
  }

  //------

  if (viewDatas_.empty()) {
    std::cerr << "No model data\n";
    return;
  }

  ViewData &viewData = viewDatas_.back();

  ModelP model = viewData.model;

  //------

  typeName = fixTypeName(typeName);

  if (typeName == "")
    return;

  // ignore if bad type
  CQChartsPlotType *type = charts_->plotType(typeName);

  if (! type) {
    std::cerr << "Invalid type '" << typeName.toStdString() << "' for plot\n";
    return;
  }

  //------

  // result plot if needed
  bool reuse = false;

  if (overlay || y1y2) {
    if (typeName == "xy")
      reuse = true;
  }
  else {
    reuse = true;
  }

  //------

  // create plot from init (argument) data
  CQChartsPlot *plot = createPlot(viewData, model, type, nameValueData, reuse);
  assert(plot);

  //---

  // init plot
  if (overlay || y1y2) {
    plot->setBackground    (false);
    plot->setDataBackground(false);
  }

  if (title != "")
    plot->setTitle(title);

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
}

void
CQChartsTest::
sourceCmd(const Args &args)
{
  QString filename;

  for (std::size_t i = 0; i < args.size(); ++i) {
    std::string arg = args[i];

    if (arg[0] == '-') {
    }
    else {
      if (filename == "")
        filename = arg.c_str();
    }
  }

  if (filename == "") {
    std::cerr << "No filename\n";
    return;
  }

  CUnixFile file(filename.toStdString());

  if (! file.open()) {
    std::cerr << "Failed to open file '" << filename.toStdString() << "'\n";
    return;
  }

  // read lines
  std::string line;

  while (file.readLine(line)) {
    parseLine(line);
  }
}
