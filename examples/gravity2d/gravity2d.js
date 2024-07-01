Highcharts.chart('container', {
    chart: {
        height: '100%',
        events: {
            click: function (e) {
                // const x = Math.round(e.xAxis[0].value),
                //     y = Math.round(e.yAxis[0].value);
                this.series[1].addPoint([e.xAxis[0].value, e.yAxis[0].value]);

                // Update heatmap values
                updateHeatmap(this);
            }
        },
    },
    title: {
        text: 'Gravitational Field Surrounding Several Points',
        align: 'left'
    },
    subtitle: {
        text: 'Click the plot area to add a point. Click a point to remove it.',
        align: 'left'
    },
    accessibility: {
        announceNewData: {
            enabled: true
        }
    },
    xAxis: {
        min: 0,
        max: 100,
        gridLineWidth: 1,
        visible: false,
    },
    yAxis: {
        min: 0,
        max: 100,
        gridLineWidth: 1,
        visible: false,
    },
    legend: {
        enabled: false
    },
    colorAxis: {
        stops: [
            [0, '#000004'],
            [0.1, '#140B3A'],
            [0.2, '#3B0F70'],
            [0.3, '#641A80'],
            [0.4, '#8C2981'],
            [0.5, '#B63679'],
            [0.6, '#E65163'],
            [0.7, '#FB8761'],
            [0.8, '#FEC287'],
            [0.9, '#FEEE9E'],
            [1, '#FCFDBF']
        ],
        min: 0,
        max: 1,
    },
    plotOptions: {
        series: {
            stickyTracking: false,
            point: {
                events: {
                    // Remove points on click
                    click: function () {
                        if (this.series.data.length > 1) {
                            this.remove();
                            updateHeatmap(Highcharts.charts[0]);
                        }
                    }
                }
            }
        }
    },
    series: [
        {
            name: "field",
            type: "heatmap",
            data: [],
            enableMouseTracking: false,
            backgroundColor: '#000000',
            states: {
                inactive: {
                    enabled: false,
                    opacity: 1,
                }
            }
        },
        {
            name: "sources",
            type: "scatter",
            data: [[20, 20], [25, 25]],
            marker: {
                radius: 8
            },
        },
    ]
});

function updateHeatmap(chart) {
    const heatmapSeries = chart.series[0];
    const scatterSeries = chart.series[1];
    const scatterData = scatterSeries.data.map(point => [point.x, point.y]);
    const gridSize = 100;


    const calculateValue = (x, y) => {
        return scatterData.reduce((sum, [sx, sy]) => {
            const dx = x - sx;
            const dy = y - sy;
            const distanceSquared = dx * dx + dy * dy;
            return sum + (distanceSquared > 0 ? 1 / distanceSquared : 0);
        }, 0);
    };



    // Calculate values and find max value for normalization
    const heatmapData = [];
    for (let x = 0; x < gridSize; x++) {
        for (let y = 0; y < gridSize; y++) {
            const value = calculateValue(x, y);
            heatmapData.push([x, y, value]);
        }
    }


    heatmapSeries.setData(heatmapData); // Ensure proper redraw
}

// Initial update of the heatmap
const chart = Highcharts.charts[0];
updateHeatmap(chart);