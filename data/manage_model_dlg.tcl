set model [load_model -csv data/xy_100000.csv -first_line_header]
set model [load_model -tsv data/multi_series.tsv -comment_header -column_type "0#time:format=%Y%m%d"]

manage_model_dlg
