
#include "openclkernelhelper.h"

const char* google_html_template = STRINGIFY(

<head>

<script type=\"text/javascript\" src=\"https://www.google.com/jsapi?autoload={'modules':[{'name':'visualization',
       'version':'1','packages':['timeline']}]}\"></script>
<script type=\"text/javascript\">
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
     <div id=\"example1\" style=\"width: 1280px; height: 480px;\"></div>
</body>

</html>

);
