TEMPLATE = lib

TARGET = CQCharts

QT += widgets

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11

MOC_DIR = .moc

CONFIG += staticlib

SOURCES += \
CQCharts.cpp \
\
CQChartsCsv.cpp \
CQChartsTsv.cpp \
CQChartsJson.cpp \
CQChartsGnuData.cpp \
CQChartsModelColumn.cpp \
CQChartsColumn.cpp \
\
CQChartsTable.cpp \
CQChartsTree.cpp \
CQChartsHeader.cpp \
\
CQChartsView.cpp \
CQChartsViewExpander.cpp \
CQChartsViewSettings.cpp \
CQChartsViewStatus.cpp \
CQChartsViewToolBar.cpp \
CQChartsViewToolTip.cpp \
CQChartsProbeBand.cpp \
\
CQChartsPlot.cpp \
CQChartsAdjacencyPlot.cpp \
CQChartsBarChartPlot.cpp \
CQChartsBoxPlot.cpp \
CQChartsBubblePlot.cpp \
CQChartsChordPlot.cpp \
CQChartsDelaunayPlot.cpp \
CQChartsForceDirectedPlot.cpp \
CQChartsGeometryPlot.cpp \
CQChartsHierBubblePlot.cpp \
CQChartsParallelPlot.cpp \
CQChartsPiePlot.cpp \
CQChartsScatterPlot.cpp \
CQChartsSunburstPlot.cpp \
CQChartsTreeMapPlot.cpp \
CQChartsXYPlot.cpp \
\
CQChartsBoxObj.cpp \
CQChartsTextBoxObj.cpp \
CQChartsDataLabel.cpp \
CQChartsLineObj.cpp \
CQChartsPointObj.cpp \
\
CQChartsAxis.cpp \
CQChartsKey.cpp \
CQChartsTitle.cpp \
\
CQChartsValueSet.cpp \
CQChartsPlotSymbol.cpp \
\
CQChartsLoader.cpp \
\
CQChartsPlotDlg.cpp \
\
CQPropertyView.cpp \
CQPropertyViewDelegate.cpp \
CQPropertyViewEditor.cpp \
CQPropertyViewFilter.cpp \
CQPropertyViewItem.cpp \
CQPropertyViewModel.cpp \
CQPropertyViewTree.cpp \
\
CQPropertyViewAlignType.cpp \
CQPropertyViewColorType.cpp \
CQPropertyViewFontType.cpp \
CQPropertyViewIntegerType.cpp \
CQPropertyViewPointFType.cpp \
CQPropertyViewRealType.cpp \
CQPropertyViewRectFType.cpp \
CQPropertyViewSizeFType.cpp \
CQPropertyViewType.cpp \
\
CQGradientControlIFace.cpp \
CQGradientControlPlot.cpp \
CGradientPalette.cpp \
\
CQCsvModel.cpp \
CQTsvModel.cpp \
CQJsonModel.cpp \
CJson.cpp \
CQGnuDataModel.cpp \
\
CQColorChooser.cpp \
CQAlphaButton.cpp \
CQFontChooser.cpp \
CQBBox2DEdit.cpp \
CQPoint2DEdit.cpp \
CQAlignEdit.cpp \
CQRealSpin.cpp \
CQToolTip.cpp \
CQPixmapCache.cpp \
CQUtil.cpp \
CQWidgetMenu.cpp \
CQIconCombo.cpp \
\
CQHeaderView.cpp \
CQRotatedText.cpp \
CQRoundedPolygon.cpp \
CQFilename.cpp \
CQStrParse.cpp \
\
CDelaunay.cpp \
CHull3D.cpp \
COSNaN.cpp \

HEADERS += \
../include/CQCharts.h \
\
../include/CQChartsCsv.h \
../include/CQChartsTsv.h \
../include/CQChartsJson.h \
../include/CQChartsGnuData.h \
../include/CQChartsModelColumn.h \
../include/CQChartsColumn.h \
\
../include/CQChartsTable.h \
../include/CQChartsTree.h \
../include/CQChartsHeader.h \
\
../include/CQChartsView.h \
../include/CQChartsViewExpander.h \
../include/CQChartsViewSettings.h \
../include/CQChartsViewStatus.h \
../include/CQChartsViewToolBar.h \
../include/CQChartsViewToolTip.h \
../include/CQChartsProbeBand.h \
\
../include/CQChartsPlot.h \
../include/CQChartsAdjacencyPlot.h \
../include/CQChartsBarChartPlot.h \
../include/CQChartsBoxPlot.h \
../include/CQChartsBubblePlot.h \
../include/CQChartsChordPlot.h \
../include/CQChartsDelaunayPlot.h \
../include/CQChartsForceDirectedPlot.h \
../include/CQChartsGeometryPlot.h \
../include/CQChartsHierBubblePlot.h \
../include/CQChartsParallelPlot.h \
../include/CQChartsPiePlot.h \
../include/CQChartsScatterPlot.h \
../include/CQChartsSunburstPlot.h \
../include/CQChartsTreeMapPlot.h \
../include/CQChartsXYPlot.h \
\
../include/CQChartsPlotObj.h \
../include/CQChartsBoxObj.h \
../include/CQChartsTextBoxObj.h \
../include/CQChartsDataLabel.h \
../include/CQChartsLineObj.h \
../include/CQChartsPointObj.h \
\
../include/CQChartsAxis.h \
../include/CQChartsKey.h \
../include/CQChartsTitle.h \
\
../include/CQChartsValueSet.h \
../include/CQChartsPlotSymbol.h \
../include/CQChartsUtil.h \
../include/CQChartsQuadTree.h \
\
../include/CQChartsLoader.h \
\
../include/CQChartsPlotDlg.h \
\
CQPropertyView.h \
CQPropertyViewDelegate.h \
CQPropertyViewEditor.h \
CQPropertyViewFilter.h \
CQPropertyViewItem.h \
CQPropertyViewModel.h \
CQPropertyViewTree.h \
\
CQPropertyViewAlignType.h \
CQPropertyViewAngleType.h \
CQPropertyViewColorType.h \
CQPropertyViewFontType.h \
CQPropertyViewIntegerType.h \
CQPropertyViewLineDashType.h \
CQPropertyViewPaletteType.h \
CQPropertyViewPointFType.h \
CQPropertyViewRealType.h \
CQPropertyViewRectFType.h \
CQPropertyViewSizeFType.h \
CQPropertyViewType.h \
\
CQGradientControlIFace.h \
CQGradientControlPlot.h \
CGradientPalette.h \
\
../include/CQCsvModel.h \
../include/CQTsvModel.h \
../include/CQJsonModel.h \
../include/CJson.h \
../include/CQGnuDataModel.h \
\
CQAlphaButton.h \
CQBBox2DEdit.h \
CQColorChooser.h \
CQFilename.h \
CQFontChooser.h \
CQHeaderView.h \
CQPixmapCache.h \
CQPoint2DEdit.h \
CQAlignEdit.h \
CQRealSpin.h \
CQRotatedText.h \
CQRoundedPolygon.h \
CQStrParse.h \
CQToolTip.h \
../include/CQUtil.h \
CQWidgetMenu.h \
CQIconCombo.h \
\
CDelaunay.h \
CHull3D.h \
CListLink.h \
CStateIterator.h \
../include/CUnixFile.h \
../include/COSNaN.h \

DESTDIR     = ../lib
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
