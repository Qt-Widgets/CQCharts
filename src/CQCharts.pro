TEMPLATE = lib

TARGET = CQCharts

QT += widgets svg

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = .moc

CONFIG += staticlib

SOURCES += \
CQCharts.cpp \
\
CQChartsCsv.cpp \
CQChartsTsv.cpp \
CQChartsJson.cpp \
CQChartsGnuData.cpp \
CQChartsExprModel.cpp \
CQChartsColumn.cpp \
CQChartsColumnType.cpp \
\
CQChartsTable.cpp \
CQChartsTree.cpp \
CQChartsHeader.cpp \
\
CQChartsWindow.cpp \
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
CQChartsDendrogramPlot.cpp \
CQChartsDistributionPlot.cpp \
CQChartsForceDirectedPlot.cpp \
CQChartsGeometryPlot.cpp \
CQChartsHierBubblePlot.cpp \
CQChartsHierScatterPlot.cpp \
CQChartsImagePlot.cpp \
CQChartsParallelPlot.cpp \
CQChartsPiePlot.cpp \
CQChartsRadarPlot.cpp \
CQChartsSankeyPlot.cpp \
CQChartsScatterPlot.cpp \
CQChartsSunburstPlot.cpp \
CQChartsTreeMapPlot.cpp \
CQChartsXYPlot.cpp \
\
CQChartsNoDataObj.cpp \
CQChartsPlotObj.cpp \
CQChartsPlotObjTree.cpp \
CQChartsBoxObj.cpp \
CQChartsTextBoxObj.cpp \
CQChartsDataLabel.cpp \
\
CQChartsAxis.cpp \
CQChartsKey.cpp \
CQChartsTitle.cpp \
CQChartsTheme.cpp \
CQChartsAnnotation.cpp \
CQChartsArrow.cpp \
CQChartsEditHandles.cpp \
CQChartsResizeHandle.cpp \
\
CQChartsValueSet.cpp \
CQChartsPlotSymbol.cpp \
\
CQChartsLoader.cpp \
CQChartsFilterEdit.cpp \
CQChartsModelFilter.cpp \
CQChartsModelExprMatch.cpp \
\
CQChartsPlotDlg.cpp \
\
CQChartsGradientPaletteCanvas.cpp \
CQChartsGradientPalette.cpp \
CQChartsGradientPaletteControl.cpp \
\
CQChartsLineDash.cpp \
CQChartsLineDashEdit.cpp \
\
CQChartsRotatedText.cpp \
CQChartsRoundedPolygon.cpp \
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
CQChartsColor.cpp \
CQChartsDrawUtil.cpp \
CQChartsPosition.cpp \
CQChartsLength.cpp \
CQChartsPath.cpp \
CQChartsStyle.cpp \
CQChartsUtil.cpp \
\
CQCsvModel.cpp \
CQTsvModel.cpp \
CQJsonModel.cpp \
CJson.cpp \
CQGnuDataModel.cpp \
CQSortModel.cpp \
CQExprModel.cpp \
CQExprModelExpr.cpp \
CQFoldedModel.cpp \
CQDataModel.cpp \
CQBaseModel.cpp \
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
CQHistoryLineEdit.cpp \
\
CQTreeView.cpp \
CQTableView.cpp \
CQHeaderView.cpp \
CQFilename.cpp \
CQStrParse.cpp \
\
CQChartsDelaunay.cpp \
CQChartsDendrogram.cpp \
CQChartsHull3D.cpp \
COSNaN.cpp \

HEADERS += \
../include/CQCharts.h \
\
../include/CQChartsCsv.h \
../include/CQChartsTsv.h \
../include/CQChartsJson.h \
../include/CQChartsGnuData.h \
../include/CQChartsExprModel.h \
../include/CQChartsColumn.h \
../include/CQChartsColumnType.h \
\
../include/CQChartsTable.h \
../include/CQChartsTree.h \
../include/CQChartsHeader.h \
\
../include/CQChartsWindow.h \
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
../include/CQChartsDendrogramPlot.h \
../include/CQChartsDistributionPlot.h \
../include/CQChartsForceDirectedPlot.h \
../include/CQChartsGeometryPlot.h \
../include/CQChartsHierBubblePlot.h \
../include/CQChartsHierScatterPlot.h \
../include/CQChartsImagePlot.h \
../include/CQChartsParallelPlot.h \
../include/CQChartsPiePlot.h \
../include/CQChartsRadarPlot.h \
../include/CQChartsSankeyPlot.h \
../include/CQChartsScatterPlot.h \
../include/CQChartsSunburstPlot.h \
../include/CQChartsTreeMapPlot.h \
../include/CQChartsXYPlot.h \
\
../include/CQChartsNoDataObj.h \
../include/CQChartsPlotObj.h \
../include/CQChartsPlotObjTree.h \
../include/CQChartsBoxObj.h \
../include/CQChartsTextBoxObj.h \
../include/CQChartsDataLabel.h \
\
../include/CQChartsAxis.h \
../include/CQChartsAxisObj.h \
../include/CQChartsKey.h \
../include/CQChartsTitle.h \
../include/CQChartsTheme.h \
../include/CQChartsAnnotation.h \
../include/CQChartsArrow.h \
../include/CQChartsEditHandles.h \
../include/CQChartsResizeHandle.h \
\
../include/CQChartsValueSet.h \
../include/CQChartsPlotSymbol.h \
../include/CQChartsPosition.h \
../include/CQChartsPath.h \
../include/CQChartsStyle.h \
../include/CQChartsLength.h \
../include/CQChartsUtil.h \
../include/CQChartsQuadTree.h \
\
../include/CQChartsLoader.h \
../include/CQChartsFilterEdit.h \
../include/CQChartsModelFilter.h \
../include/CQChartsModelExprMatch.h \
\
../include/CQChartsPlotDlg.h \
\
../include/CQChartsGradientPaletteCanvas.h \
../include/CQChartsGradientPalette.h \
../include/CQChartsGradientPaletteControl.h \
\
../include/CQChartsLineDash.h \
../include/CQChartsLineDashEdit.h \
\
../include/CQChartsRotatedText.h \
../include/CQChartsRoundedPolygon.h \
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
../include/CQChartsColor.h \
../include/CQChartsDrawUtil.h \
\
../include/CQCsvModel.h \
../include/CQTsvModel.h \
../include/CQJsonModel.h \
../include/CJson.h \
../include/CQGnuDataModel.h \
../include/CQSortModel.h \
../include/CQExprModel.h \
CQExprModelExpr.h \
../include/CQFoldedModel.h \
../include/CQDataModel.h \
../include/CQBaseModel.h \
\
CQAlphaButton.h \
CQBBox2DEdit.h \
CQColorChooser.h \
CQFilename.h \
CQFontChooser.h \
../include/CQTreeView.h \
../include/CQTableView.h \
CQHeaderView.h \
CQPixmapCache.h \
CQPoint2DEdit.h \
CQAlignEdit.h \
CQRealSpin.h \
../include/CQStrParse.h \
CQToolTip.h \
../include/CQUtil.h \
CQWidgetMenu.h \
CQIconCombo.h \
../include/CQHistoryLineEdit.h \
\
../include/CQChartsDelaunay.h \
../include/CQChartsDendrogram.h \
../include/CQChartsHull3D.h \
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
../../CExpr/include \
../../CReadLine/include \
../../CMath/include \
../../CStrUtil/include \
../../CUtil/include \
../../COS/include \
