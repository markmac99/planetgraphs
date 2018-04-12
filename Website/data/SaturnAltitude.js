$(function() {
Morris.Line({
  element: 'planet-altitude',
data: [ 
{time: 1517471122000, altitude: 11.6},
{time: 1520145898000, altitude: 14.49},
{time: 1522820049000, altitude: 15.8},
{time: 1525492343000, altitude: 15.93},
{time: 1528163029000, altitude: 15.83},
{time: 1530919925000, altitude: 15.68},
{time: 1533590494000, altitude: 15.56},
{time: 1536261363000, altitude: 15.47},
{time: 1538933336000, altitude: 15.34},
{time: 1541607983000, altitude: 14.08},
{time: 1544284450000, altitude: 9.43},
{time: 1546905600000, altitude: 0},
{time: 1549611245000, altitude: 9.5},
{time: 1552285789000, altitude: 13.21},
{time: 1554959937000, altitude: 15.61},
{time: 1557634513000, altitude: 16.66},
{time: 1560305256000, altitude: 16.44},
{time: 1563062173000, altitude: 16.11},
{time: 1565732717000, altitude: 15.83},
{time: 1568403530000, altitude: 15.67},
{time: 1571074732000, altitude: 15.67},
{time: 1573748152000, altitude: 15.49},
{time: 1576425217000, altitude: 11.69},
{time: 1579046400000, altitude: 0},
{time: 1581751292000, altitude: 7.56},
{time: 1584425662000, altitude: 11.98},
{time: 1587099851000, altitude: 15.45},
{time: 1589774866000, altitude: 18.02},
{time: 1592447450000, altitude: 18.01},
{time: 1595118001000, altitude: 17.53},
{time: 1597874932000, altitude: 17.06},
{time: 1600545694000, altitude: 16.8},
{time: 1603216832000, altitude: 16.83},
{time: 1605888424000, altitude: 17.14},
{time: 1608566143000, altitude: 14.35},
{time: 1611187200000, altitude: 0},
{time: 1613891279000, altitude: 5.76},
{time: 1616565527000, altitude: 10.79},
{time: 1619239803000, altitude: 15.33},
{time: 1621915163000, altitude: 19.6},
{time: 1624589593000, altitude: 20.46},
{time: 1627260187000, altitude: 19.86},
{time: 1630017117000, altitude: 19.23},
{time: 1632687835000, altitude: 18.85},
{time: 1635358908000, altitude: 18.86},
{time: 1638030362000, altitude: 19.27},
{time: 1640707221000, altitude: 17.46},
{time: 1643328000000, altitude: 0},
{time: 1646006400000, altitude: 0},
{time: 1648705393000, altitude: 9.63},
{time: 1651379805000, altitude: 15.28},
{time: 1654055576000, altitude: 21.43},
{time: 1656731679000, altitude: 23.69},
{time: 1659402324000, altitude: 23.01},
{time: 1662159260000, altitude: 22.24},
{time: 1664829940000, altitude: 21.73},
{time: 1667500949000, altitude: 21.69},
{time: 1670172336000, altitude: 22.14},
{time: 1672848437000, altitude: 21.02},
{time: 1675529840000, altitude: 7.07},
{time: 1678147200000, altitude: 0},
{time: 1680845270000, altitude: 8.49},
{time: 1683519869000, altitude: 15.27},
{time: 1686196120000, altitude: 23.5},
{time: 1688873717000, altitude: 27.57},
{time: 1691544416000, altitude: 26.88},
{time: 1694301365000, altitude: 26},
{time: 1696972011000, altitude: 25.36},
{time: 1699642957000, altitude: 25.23},
{time: 1702314277000, altitude: 25.69},
{time: 1704989769000, altitude: 25.01}],
        xkey: 'time',
        ykeys: ['altitude'],
        labels: ['Altitude'],
        hideHover: 'auto',
        xLabelAngle: 45,
        ymax: 33 ,
        ymin: 0,
        postUnits: '\xB0',
        resize: true
    });
});
