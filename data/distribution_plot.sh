CQChartsTest \
 -csv data/name_value.csv -first_line_header \
 -type distribution -columns "value=1" \
-and \
 -csv data/gaussian.txt -comment_header \
 -type distribution -columns "value=0" \
 -bool "horizontal=1" \
-and \
 -csv data/gaussian.txt -comment_header \
 -type distribution -columns "value=0" \
 -bool "autoRange=0" -real "startBucketValue=1.0,deltaBucketValue=0.1" \
-and \
 -csv data/distribution_sparse.csv -first_line_header \
 -type distribution -columns "value=0,color=1" \
 -bool "autoRange=0" -real "deltaBucketValue=1" \
 -properties "color.mapped=1"
