$(function() {
Morris.Line({
  element: 'planet-altitude',
data: [ 
{time: 1517469987000, altitude: 17.97},
{time: 1520145825000, altitude: 15.21},
{time: 1522820049000, altitude: 14.5},
{time: 1525494564000, altitude: 15.27},
{time: 1528170609000, altitude: 16.47},
{time: 1530842406000, altitude: 14.92},
{time: 1533598136000, altitude: 11.96},
{time: 1536268527000, altitude: 12.61},
{time: 1538941818000, altitude: 16.59},
{time: 1541616673000, altitude: 22.92},
{time: 1544292059000, altitude: 30.99},
{time: 1546967613000, altitude: 39.85},
{time: 1549645481000, altitude: 47.66},
{time: 1552327248000, altitude: 46.37},
{time: 1555008825000, altitude: 37.87},
{time: 1557690319000, altitude: 26.2},
{time: 1560370971000, altitude: 14.57},
{time: 1563049155000, altitude: 6.57},
{time: 1565654400000, altitude: 0},
{time: 1568332800000, altitude: 0},
{time: 1571034521000, altitude: 12.2},
{time: 1573716201000, altitude: 18.74},
{time: 1576397209000, altitude: 19.42},
{time: 1579075610000, altitude: 16.08},
{time: 1581751292000, altitude: 13.42},
{time: 1584425662000, altitude: 12.56},
{time: 1587099851000, altitude: 13.64},
{time: 1589774866000, altitude: 17.67},
{time: 1592452001000, altitude: 26.48},
{time: 1595131865000, altitude: 38.32},
{time: 1597809137000, altitude: 43.9},
{time: 1600480800000, altitude: 44.9},
{time: 1603236271000, altitude: 43.26},
{time: 1605906317000, altitude: 43.7},
{time: 1608578945000, altitude: 47.62},
{time: 1611253156000, altitude: 53.03},
{time: 1613928576000, altitude: 58.13},
{time: 1616610245000, altitude: 54.94},
{time: 1619291796000, altitude: 43.42},
{time: 1621973102000, altitude: 28.89},
{time: 1624652845000, altitude: 16.02},
{time: 1627329785000, altitude: 8.14},
{time: 1629936000000, altitude: 0},
{time: 1632614400000, altitude: 0},
{time: 1635292800000, altitude: 0},
{time: 1637999136000, altitude: 10.87},
{time: 1640679205000, altitude: 11.72},
{time: 1643356279000, altitude: 10.28},
{time: 1646031220000, altitude: 9.45},
{time: 1648705393000, altitude: 9.97},
{time: 1651379805000, altitude: 12.57},
{time: 1654055576000, altitude: 19},
{time: 1656733950000, altitude: 31.32},
{time: 1659414636000, altitude: 46.92},
{time: 1662096012000, altitude: 58.07},
{time: 1664771907000, altitude: 60.66},
{time: 1667444204000, altitude: 62.09},
{time: 1670113193000, altitude: 63.11},
{time: 1672868256000, altitude: 62.66},
{time: 1675539931000, altitude: 62.86},
{time: 1678213806000, altitude: 63.63},
{time: 1680893222000, altitude: 59.64},
{time: 1683574740000, altitude: 45.42},
{time: 1686255591000, altitude: 28.85},
{time: 1688934148000, altitude: 16.16},
{time: 1691610025000, altitude: 9.11},
{time: 1694284453000, altitude: 5.29},
{time: 1696896000000, altitude: 0},
{time: 1699574400000, altitude: 0},
{time: 1702252800000, altitude: 0},
{time: 1704931200000, altitude: 0}],
        xkey: 'time',
        ykeys: ['altitude'],
        labels: ['Altitude'],
        hideHover: 'auto',
        xLabelAngle: 45,
        ymax: 76 ,
        ymin: 0,
        postUnits: '\xB0',
        resize: true
    });
});