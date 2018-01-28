load -csv sea_level.csv -first_line_header

set model = $_rc

model -ind $model -column_type "0#real:format=%gM"

plot -type bar -columns "category=1,value=0,color=2,label=3" -ymax 8000

set plot1 = $_rc

set_property -view view1 -plot $plot1 -name dataLabel.visible -value 1
set_property -view view1 -plot $plot1 -name dataLabel.position -value TOP_OUTSIDE
set_property -view view1 -plot $plot1 -name invertY -value 1
set_property -view view1 -plot $plot1 -name "yaxis.grid.line.major.visible" -value 1
