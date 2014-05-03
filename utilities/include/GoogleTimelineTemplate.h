
#include "openclkernelhelper.h"


#ifdef __cplusplus
extern "C" {
#endif

const char* google_html_template = STRINGIFYNNL(
<head>
<script type=\x22text/javascript\x22 src=\x22https:\/\/www.google.com/jsapi?autoload={'modules':[{'name':'visualization',
       'version':'1','packages':['timeline']}]}\x22></script>
<script type=\x22text/javascript\x22>
       google.setOnLoadCallback(drawChart);
       function drawChart() {
         var container = document.getElementById('example1');
         var chart = new google.visualization.Timeline(container);
         var dataTable = new google.visualization.DataTable();
<TIMELINE_CHART_DATA>
           chart.draw(dataTable);
       }
</script>
</head>
<body>
<div id = \x22)STRINGIFYNNL(example1\x22 style = \x22width: 1280px; height: 480px; \x22>< / div>
</body>
</html>
);

#ifdef __cplusplus
}
#endif

