#include <CQChartsTest.h>
#include <CQCharts.h>
#include <CQChartsTable.h>
#include <CQChartsTree.h>
#include <CQChartsCsv.h>
#include <CQChartsTsv.h>
#include <CQChartsJson.h>
#include <CQChartsGnuData.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQChartsAxis.h>
#include <CQChartsXYPlot.h>

#include <CQChartsLoader.h>
#include <CQChartsPlotDlg.h>

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

      if      (arg == "csv")
        initData.fileType = CQChartsTest::FileType::CSV;
      else if (arg == "tsv")
        initData.fileType = CQChartsTest::FileType::TSV;
      else if (arg == "json")
        initData.fileType = CQChartsTest::FileType::JSON;
      else if (arg == "data")
        initData.fileType = CQChartsTest::FileType::DATA;
      else if (arg == "type") {
        ++i;

        if (i < argc)
          initData.typeName = argv[i];
      }
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
              std::cerr << "Invalid " << arg << " option '" << argv[i] << "'" << std::endl;
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
      else if (arg == "bool") {
        ++i;

        if (i < argc) {
          QString nameValue = argv[i];

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
            std::cerr << "Invalid -bool option '" << argv[i] << "'" << std::endl;
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
      else if (arg == "column_type") {
        ++i;

        if (i < argc)
          initData.columnType = argv[i];
      }
      else if (arg == "comment_header") {
        initData.commentHeader = true;
      }
      else if (arg == "first_line_header") {
        initData.firstLineHeader = true;
      }
      else if (arg == "xintegral") {
        initData.xintegral = true;
      }
      else if (arg == "yintegral") {
        initData.yintegral = true;
      }
      else if (arg == "plot_title") {
        ++i;

        if (i < argc)
          initData.title = argv[i];
      }
      else if (arg == "properties") {
        ++i;

        if (i < argc)
          initData.properties = argv[i];
      }
      else if (arg == "overlay") {
        overlay = true;
      }
      else if (arg == "y1y2") {
        y1y2 = true;
      }
      else if (arg == "and") {
        initDatas.push_back(initData);

        xmin1 = boost::make_optional(false, 0.0);
        xmax1 = boost::make_optional(false, 0.0);
        xmin2 = boost::make_optional(false, 0.0);
        xmax2 = boost::make_optional(false, 0.0);
        ymin1 = boost::make_optional(false, 0.0);
        ymax1 = boost::make_optional(false, 0.0);
        ymin2 = boost::make_optional(false, 0.0);
        ymax2 = boost::make_optional(false, 0.0);

        initData = CQChartsTest::InitData();
      }
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
      else if (arg == "loop") {
        loop = true;
      }
      else {
        std::cerr << "Invalid option '" << argv[i] << "'" << std::endl;
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

  double dx = 1000.0/nc;
  double dy = 1000.0/nr;

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

    if (initData.overlay || initData.y1y2) {
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

  test.show();

  if (test.view())
    test.view()->raise();

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

  QHBoxLayout *layout = CQUtil::makeLayout<QHBoxLayout>(centralWidget(), 0, 0);

  //---

  QFrame *plotFrame = CQUtil::makeWidget<QFrame>("plotFrame");

  QVBoxLayout *plotLayout = new QVBoxLayout(plotFrame);
  plotLayout->setMargin(0); plotLayout->setSpacing(2);

  layout->addWidget(plotFrame);

  //---

  filterEdit_ = CQUtil::makeWidget<QLineEdit>("filter");

  plotLayout->addWidget(filterEdit_);

  connect(filterEdit_, SIGNAL(returnPressed()), this, SLOT(filterSlot()));

  //---

  // table/tree widgets
  stack_ = new QStackedWidget;

  stack_->setObjectName("stack");

  table_ = new CQChartsTable;
  tree_  = new CQChartsTree;

  stack_->addWidget(table_);
  stack_->addWidget(tree_);

  plotLayout->addWidget(stack_);

  connect(table_, SIGNAL(columnClicked(int)), this, SLOT(tableColumnClicked(int)));

  //---

  QGroupBox *typeGroup = new QGroupBox;

  typeGroup->setObjectName("typeGroup");
  typeGroup->setTitle("Column Type");

  plotLayout->addWidget(typeGroup);

  QVBoxLayout *typeGroupLayout = new QVBoxLayout(typeGroup);

  QFrame *columnFrame = new QFrame;

  columnFrame->setObjectName("columnFrame");

  typeGroupLayout->addWidget(columnFrame);

  QGridLayout *columnLayout = new QGridLayout(columnFrame);

  int row = 0;

  columnTypeEdit_ = addLineEdit(columnLayout, row, "Type", "type");

  QPushButton *typeOKButton = new QPushButton("Set");

  typeOKButton->setObjectName("typeOK");

  connect(typeOKButton, SIGNAL(clicked()), this, SLOT(typeOKSlot()));

  columnLayout->addWidget(typeOKButton);
}

CQChartsTest::
~CQChartsTest()
{
  delete charts_;

  model_.clear();
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

bool
CQChartsTest::
initPlot(const InitData &initData)
{
  int i = plots_.size();

  //---

  setId(QString("%1").arg(i + 1));

  int r = i / initData.nc;
  int c = i % initData.nc;

  if (initData.overlay || initData.y1y2) {
    setBBox(CBBox2D(0, 0, 1000, 1000));
  }
  else {
    double x1 =  c     *initData.dx;
    double x2 = (c + 1)*initData.dx;
    double y1 =  r     *initData.dy;
    double y2 = (r + 1)*initData.dy;

    setBBox(CBBox2D(x1, 1000.0 - y2, x2, 1000.0 - y1));
  }

  //---

  if (initData.filenames.size() > 0) {
    if (initData.fileType != FileType::NONE) {
      loadFileModel(initData.filenames[0], initData.fileType,
                    initData.commentHeader, initData.firstLineHeader);
    }
    else {
      std::cerr << "No plot type specified" << std::endl;
    }
  }

  if (! model_)
    return false;

  //---

  CQChartsPlot *plot = init(model_, initData, i);

  if (! plot)
    return false;

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

void
CQChartsTest::
loadFileSlot(const QString &type, const QString &filename)
{
  FileType fileType = stringToFileType(type);

  if (fileType == FileType::NONE) {
    std::cerr << "Bad type specified '" << type.toStdString() << "'" << std::endl;
    return;
  }

  //---

  bool commentHeader   = loader_->isCommentHeader();
  bool firstLineHeader = loader_->isFirstLineHeader();

  loadFileModel(filename, fileType, commentHeader, firstLineHeader);
}

//------

void
CQChartsTest::
createSlot()
{
  if (! model_)
    return;

  //---

  CQChartsPlotDlg *dlg = new CQChartsPlotDlg(charts_, model_);

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

  plot->view()->show();
}

//------

void
CQChartsTest::
plotObjPressedSlot(CQChartsPlotObj *obj)
{
  QString id = obj->id();

  if (id.length())
    std::cerr << id.toStdString() << std::endl;
}

//------

void
CQChartsTest::
filterSlot()
{
  if (stack_->currentIndex() == 0)
    table_->setFilter(filterEdit_->text());
  else
    tree_->setFilter(filterEdit_->text());
}

//------

void
CQChartsTest::
tableColumnClicked(int column)
{
  tableColumn_ = column;

  QVariant var = model_->headerData(tableColumn_, Qt::Horizontal, CQCharts::Role::ColumnType);

  QString type = var.toString();

  if (type.length())
    columnTypeEdit_->setText(type);
}

//------

void
CQChartsTest::
typeOKSlot()
{
  QString type = columnTypeEdit_->text();

  model_->setHeaderData(tableColumn_, Qt::Horizontal, type, CQCharts::Role::ColumnType);
}

//------

CQChartsPlot *
CQChartsTest::
init(const ModelP &model, const InitData &initData, int i)
{
  setColumnFormats(model, initData.columnType);

  //---

  QString typeName = fixTypeName(initData.typeName);

  if (typeName == "")
    return nullptr;

  // ignore if bad type
  CQChartsPlotType *type = charts_->plotType(typeName);

  if (! type) {
    std::cerr << "Invalid type '" << typeName.toStdString() << "' for plot" << std::endl;
    return nullptr;
  }

  //---

  // result plot if needed
  bool reuse = false;

  if (initData.overlay || initData.y1y2) {
    if (typeName == "xy")
      reuse = true;
  }
  else {
    reuse = true;
  }

  //---

  // create plot from init (argument) data
  CQChartsPlot *plot = createPlot(model, type, initData.nameValues, initData.nameBools, reuse);
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
  QStringList fstrs = columnType.split(";", QString::KeepEmptyParts);

  for (int i = 0; i < fstrs.length(); ++i) {
    QString type = fstrs[i].simplified();

    if (! type.length())
      continue;

    int column = i;

    int pos = type.indexOf("#");

    if (pos >= 0) {
      QString columnStr = type.mid(0, pos).simplified();

      int column1;

      if (stringToColumn(model, columnStr, column1))
        column = column1;
      else
        std::cerr << "Bad column name '" << columnStr.toStdString() << "'" << std::endl;

      type = type.mid(pos + 1).simplified();
    }

    if (! model->setHeaderData(column, Qt::Horizontal, type, CQCharts::Role::ColumnType)) {
      std::cerr << "Failed to set column type '" << type.toStdString() <<
                   "' for section '" << column << "'" << std::endl;
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
createPlot(const ModelP &model, CQChartsPlotType *type, const NameValues &nameValues,
           const NameBools &nameBools, bool reuse)
{
  CQChartsView *view = getView(reuse);

  //---

  CQChartsPlot *plot = type->create(view, model);

  connect(plot, SIGNAL(objPressed(CQChartsPlotObj *)),
          this, SLOT(plotObjPressedSlot(CQChartsPlotObj *)));

  //---

  // set plot property for widgets for plot parameters
  for (const auto &parameter : type->parameters()) {
    if      (parameter.type() == "column") {
      auto p = nameValues.find(parameter.name());

      if (p == nameValues.end())
        continue;

      int column;

      if (! stringToColumn(model, (*p).second, column)) {
        std::cerr << "Bad column name '" << (*p).second.toStdString() << "'" << std::endl;
        column = -1;
      }

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(column)))
        std::cerr << "Failed to set parameter " << parameter.propName().toStdString() << std::endl;
    }
    else if (parameter.type() == "columns") {
      auto p = nameValues.find(parameter.name());

      if (p == nameValues.end())
        continue;

      QStringList strs = (*p).second.split(" ", QString::SkipEmptyParts);

      std::vector<int> columns;

      for (int j = 0; j < strs.size(); ++j) {
        int column;

        if (! stringToColumn(model, strs[j], column)) {
          std::cerr << "Bad column name '" << strs[j].toStdString() << "'" << std::endl;
          continue;
        }

        columns.push_back(column);
      }

      QString s = CQChartsUtil::toString(columns);

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(s)))
        std::cerr << "Failed to set parameter " << parameter.propName().toStdString() << std::endl;
    }
    else if (parameter.type() == "bool") {
      auto p = nameBools.find(parameter.name());

      if (p == nameBools.end())
        continue;

      bool b = (*p).second;

      if (! CQUtil::setProperty(plot, parameter.propName(), QVariant(b)))
        std::cerr << "Failed to set parameter " << parameter.propName().toStdString() << std::endl;
    }
    else
      assert(false);
  }

  //---

  // init plot
  if (type->name() == "xy") {
    CQChartsXYPlot *xyPlot = dynamic_cast<CQChartsXYPlot *>(plot);
    assert(plot);

    if      (xyPlot->isBivariate()) {
      xyPlot->setFillUnder(true);
      xyPlot->setPoints   (false);
    }
    else if (xyPlot->isStacked()) {
      xyPlot->setFillUnder(true);
      xyPlot->setPoints   (false);
    }
  }

  //---

  // add plot to view and show
  plot->setId(QString("%1%2").arg(plot->typeName()).arg(id_));

  view->addPlot(plot, bbox_);

  view->show();

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
      std::cerr << "Failed to set property " << name.toStdString() << std::endl;
  }
}

//------

bool
CQChartsTest::
loadFileModel(const QString &filename, FileType type, bool commentHeader, bool firstLineHeader)
{
  model_.clear();

  bool hierarchical;

  QAbstractItemModel *model =
    loadFile(filename, type, commentHeader, firstLineHeader, hierarchical);

  if (! model)
    return false;

  model_ = ModelP(model);

  if (hierarchical)
    setTreeModel(model_);
  else
    setTableModel(model_);

  return true;
}

QAbstractItemModel *
CQChartsTest::
loadFile(const QString &filename, FileType type, bool commentHeader, bool firstLineHeader,
         bool &hierarchical)
{
  hierarchical = false;

  QAbstractItemModel *model = nullptr;

  if      (type == FileType::CSV) {
    model = loadCsv(filename, commentHeader, firstLineHeader);
  }
  else if (type == FileType::TSV) {
    model = loadTsv(filename, commentHeader, firstLineHeader);
  }
  else if (type == FileType::JSON) {
    model = loadJson(filename, hierarchical);
  }
  else if (type == FileType::DATA) {
    model = loadData(filename, commentHeader, firstLineHeader);
  }
  else {
    std::cerr << "Bad file type specified '" <<
      fileTypeToString(type).toStdString() << "'" << std::endl;
    return nullptr;
  }

  return model;
}

QAbstractItemModel *
CQChartsTest::
loadCsv(const QString &filename, bool commentHeader, bool firstLineHeader)
{
  CQChartsCsv *csv = new CQChartsCsv(charts_);

  csv->setCommentHeader  (commentHeader);
  csv->setFirstLineHeader(firstLineHeader);

  csv->load(filename);

  return csv;
}

QAbstractItemModel *
CQChartsTest::
loadTsv(const QString &filename, bool commentHeader, bool firstLineHeader)
{
  CQChartsTsv *tsv = new CQChartsTsv(charts_);

  tsv->setCommentHeader  (commentHeader);
  tsv->setFirstLineHeader(firstLineHeader);

  tsv->load(filename);

  return tsv;
}

QAbstractItemModel *
CQChartsTest::
loadJson(const QString &filename, bool &hierarchical)
{
  CQChartsJson *json = new CQChartsJson(charts_);

  json->load(filename);

  hierarchical = json->isHierarchical();

  return json;
}

QAbstractItemModel *
CQChartsTest::
loadData(const QString &filename, bool commentHeader, bool firstLineHeader)
{
  CQChartsGnuData *data = new CQChartsGnuData(charts_);

  data->setCommentHeader  (commentHeader);
  data->setFirstLineHeader(firstLineHeader);

  data->load(filename);

  return data;
}

//------

void
CQChartsTest::
setTableModel(const ModelP &model)
{
  table_->setModel(model);

  stack_->setCurrentIndex(0);
}

void
CQChartsTest::
setTreeModel(const ModelP &model)
{
  tree_->setModel(model);

  stack_->setCurrentIndex(1);
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
      view_ = charts_->addView();
  }
  else {
    view_ = charts_->addView();
  }

  return view_;
}

//------

QSize
CQChartsTest::
sizeHint() const
{
  return QSize(1600, 1200);
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
    std::cerr << "No view '" << plotName.toStdString() << "'" << std::endl;
    return;
  }

  //---

  if (plotName != "") {
    CQChartsPlot *plot = view->getPlot(plotName);

    if (! plot) {
      std::cerr << "No plot '" << plotName.toStdString() << "'" << std::endl;
      return;
    }

    //---

    if (! plot->setProperty(name, value)) {
      std::cerr << "Failed to set view parameter '" << name.toStdString() << "'" << std::endl;
      return;
    }
  }
  else {
    if (! view->setProperty(name, value)) {
      std::cerr << "Failed to set plot parameter '" << name.toStdString() << "'" << std::endl;
      return;
    }
  }
}

void
CQChartsTest::
getCmd(const Args &)
{
}

void
CQChartsTest::
loadCmd(const Args &args)
{
  QString  filename;
  FileType fileType { FileType::NONE };
  bool     commentHeader = false;
  bool     firstLineHeader = false;

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
        commentHeader = true;
      else if (opt == "first_line_header")
        firstLineHeader = true;
    }
    else {
      if (filename == "")
        filename = arg.c_str();
    }
  }

  if (filename == "") {
    std::cerr << "No filename" << std::endl;
    return;
  }

  if (fileType == FileType::NONE) {
    std::cerr << "No file type" << std::endl;
    return;
  }

  loadFileModel(filename, fileType, commentHeader, firstLineHeader);
}

void
CQChartsTest::
plotCmd(const Args &args)
{
  QString    typeName;
  NameValues nameValues;
  NameBools  nameBools;
  bool       xintegral { false };
  bool       yintegral { false };
  QString    title;
  QString    properties;
  OptReal    xmin, ymin, xmax, ymax;
  bool       y1y2      { false };
  bool       overlay   { false };

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

              nameValues[name] = value;
            }
            else {
              std::cerr << "Invalid " << opt << " option '" << args[i] << "'" << std::endl;
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
            nameBools[name] = b;
          else {
            std::cerr << "Invalid -bool option '" << args[i] << "'" << std::endl;
          }
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

  if (! model_) {
    std::cerr << "No model data" << std::endl;
    return;
  }

  //------

  typeName = fixTypeName(typeName);

  if (typeName == "")
    return;

  // ignore if bad type
  CQChartsPlotType *type = charts_->plotType(typeName);

  if (! type) {
    std::cerr << "Invalid type '" << typeName.toStdString() << "' for plot" << std::endl;
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
  CQChartsPlot *plot = createPlot(model_, type, nameValues, nameBools, reuse);
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
    std::cerr << "No filename" << std::endl;
    return;
  }

  CUnixFile file(filename.toStdString());

  if (! file.open()) {
    std::cerr << "Failed to open file '" << filename.toStdString() << "'" << std::endl;
    return;
  }

  // read lines
  std::string line;

  while (file.readLine(line)) {
    parseLine(line);
  }
}
