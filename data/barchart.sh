# Bar Chart

# Tests:
#   Simple (Single set of bars)
#   Simple (Multiple bars per category)
#   Simple (Multiple bars per category grouped by row)
#   Stacked
#   Percent
#   Range

CQChartsTest \
 -csv multi_bar.csv -first_line_header \
 -type bar -columns "category=0,value=1" -column_type "1#integer" \
 -plot_title "bar chart" \
-and \
 -csv multi_bar.csv -first_line_header \
 -type bar -columns "category=0,value=1 2 3 4 5 6 7" \
 -plot_title "multiple bar chart" -column_type "1#integer" \
 -bool "horizontal=1" \
-and \
 -csv multi_bar.csv -first_line_header \
 -type bar -columns "category=0,value=1 2 3 4 5 6 7" \
 -plot_title "multiple bar chart" -column_type "1#integer" \
 -properties "columns.rowGrouping=1" \
-and \
 -csv money_bar_stack_neg.csv -first_line_header \
 -type barchart -columns "category=0,value=1 2 3 4" \
 -bool "stacked=1,percent=1"
-and \
 -csv temp_bar_range.csv -first_line_header \
 -type barchart -columns "category=0,value=2 3" \
 -bool "range=1,horizontal=1" \
 -properties "dataLabel.visible=1,dataLabel.position=TOP_OUTSIDE" \
 -properties "key.visible=0,barMargin=12,fill.color=palette:0.2"
