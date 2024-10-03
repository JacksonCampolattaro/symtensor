// Function to calculate heatmap values

// Initial scatter data
let points = [
    [25, 25],
    [30, 30],
];
const resolution = 256
const heatmapZ = Array.from(Array(resolution), () => new Array(resolution));

// Plotly data and layout
const data = [
    {
        z: heatmapZ,
        type: 'heatmap',
        zsmooth: 'best',
        // type: 'contour',
        // line:{
        //     width:0
        // },
        colorscale: 'Hot',
        showscale: false,
        hovertemplate: '(%{x}, %{y})<extra>%{z}</extra>'
    }
];

const layout = {
    title: {
        text: 'Gravitational Field Surrounding Several Points',
        x: 0.05
    },
    xaxis: {
        range: [0, resolution],
        showgrid: false,
        zeroline: false,
        visible: false,
    },
    yaxis: {
        range: [0, resolution],
        showgrid: false,
        zeroline: false,
        visible: false,
    },
    showlegend: false,
    margin: {t: 50, l: 50, r: 50, b: 50}
};

Plotly.newPlot('container', data, layout);

// Click event handler
document.getElementById('container').on('plotly_click', function (event) {
    const x = Math.round(event.points[0].x);
    const y = Math.round(event.points[0].y);
    points.push([x, y]);
    updateHeatmap();
});

function gravitationalPotential(x, y, points) {
    return points.reduce((sum, [sx, sy]) => {
        const dx = x - sx;
        const dy = y - sy;
        const distanceSquared = dx * dx + dy * dy;
        return sum + 1 / (distanceSquared + Number.EPSILON);
    }, 0);
}

function updateHeatmap() {

    console.log("s")
    for (let x = 0; x < resolution; x++) {
        for (let y = 0; y < resolution; y++) {
            heatmapZ[y][x] = Math.min(gravitationalPotential(x, y, points), 1.0);
        }
    }
    console.log("e")

    Plotly.restyle('container', {
        z: [heatmapZ],
    });
}

window.onload = function () {
    updateHeatmap()
};