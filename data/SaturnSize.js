$(function() {
Morris.Line({
  element: 'planet-size',
data: [ 
{time: 1517471122000, size: 15.26},
{time: 1520145898000, size: 15.85},
{time: 1522820049000, size: 16.65},
{time: 1525492343000, size: 17.5},
{time: 1528163029000, size: 18.11},
{time: 1530919925000, size: 18.24},
{time: 1533590494000, size: 17.83},
{time: 1536261363000, size: 17.06},
{time: 1538933336000, size: 16.21},
{time: 1541607983000, size: 15.51},
{time: 1544284450000, size: 15.08},
{time: 1546905600000, size: 14.98},
{time: 1549611245000, size: 15.21},
{time: 1552285789000, size: 15.76},
{time: 1554959937000, size: 16.54},
{time: 1557634513000, size: 17.4},
{time: 1560305256000, size: 18.07},
{time: 1563062173000, size: 18.29},
{time: 1565732717000, size: 17.96},
{time: 1568403530000, size: 17.23},
{time: 1571074732000, size: 16.37},
{time: 1573748152000, size: 15.64},
{time: 1576425217000, size: 15.16},
{time: 1579046400000, size: 15.01},
{time: 1581751292000, size: 15.19},
{time: 1584425662000, size: 15.7},
{time: 1587099851000, size: 16.46},
{time: 1589774866000, size: 17.33},
{time: 1592447450000, size: 18.06},
{time: 1595118001000, size: 18.37},
{time: 1597874932000, size: 18.13},
{time: 1600545694000, size: 17.44},
{time: 1603216832000, size: 16.58},
{time: 1605888424000, size: 15.8},
{time: 1608566143000, size: 15.28},
{time: 1611187200000, size: 15.08},
{time: 1613891279000, size: 15.21},
{time: 1616565527000, size: 15.68},
{time: 1619239803000, size: 16.41},
{time: 1621915163000, size: 17.28},
{time: 1624589593000, size: 18.07},
{time: 1627260187000, size: 18.48},
{time: 1630017117000, size: 18.33},
{time: 1632687835000, size: 17.69},
{time: 1635358908000, size: 16.82},
{time: 1638030362000, size: 16.01},
{time: 1640707221000, size: 15.44},
{time: 1643328000000, size: 15.18},
{time: 1646006400000, size: 15.27},
{time: 1648705393000, size: 15.69},
{time: 1651379805000, size: 16.39},
{time: 1654055576000, size: 17.27},
{time: 1656731679000, size: 18.1},
{time: 1659402324000, size: 18.6},
{time: 1662159260000, size: 18.55},
{time: 1664829940000, size: 17.97},
{time: 1667500949000, size: 17.11},
{time: 1670172336000, size: 16.26},
{time: 1672848437000, size: 15.63},
{time: 1675529840000, size: 15.32},
{time: 1678147200000, size: 15.35},
{time: 1680845270000, size: 15.72},
{time: 1683519869000, size: 16.4},
{time: 1686196120000, size: 17.27},
{time: 1688873717000, size: 18.14},
{time: 1691544416000, size: 18.74},
{time: 1694301365000, size: 18.8},
{time: 1696972011000, size: 18.29},
{time: 1699642957000, size: 17.43},
{time: 1702314277000, size: 16.55},
{time: 1704989769000, size: 15.86}],
        xkey: 'time',
        ykeys: ['size'],
        labels: ['Size'],
        hideHover: 'auto',
        xLabelAngle: 45,
        ymax: 22 ,
        ymin: 12,
        postUnits: ' as',
        resize: true
    });
});
