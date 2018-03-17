$(function() {
Morris.Line({
  element: 'planet-magnitude',
data: [ 
{time: 1517504038000, magnitude: 5.87},
{time: 1520185735000, magnitude: 5.92},
{time: 1522867338000, magnitude: 5.94},
{time: 1525478400000, magnitude: 5.94},
{time: 1528170609000, magnitude: 5.91},
{time: 1530849347000, magnitude: 5.87},
{time: 1533530206000, magnitude: 5.81},
{time: 1536203165000, magnitude: 5.76},
{time: 1538874007000, magnitude: 5.72},
{time: 1541631188000, magnitude: 5.72},
{time: 1544302021000, magnitude: 5.76},
{time: 1546973001000, magnitude: 5.81},
{time: 1549645481000, magnitude: 5.86},
{time: 1552327248000, magnitude: 5.91},
{time: 1555008825000, magnitude: 5.93},
{time: 1557619200000, magnitude: 5.93},
{time: 1560311234000, magnitude: 5.9},
{time: 1562990564000, magnitude: 5.85},
{time: 1565671649000, magnitude: 5.8},
{time: 1568343288000, magnitude: 5.74},
{time: 1571014117000, magnitude: 5.71},
{time: 1573771293000, magnitude: 5.71},
{time: 1576442133000, magnitude: 5.75},
{time: 1579113129000, magnitude: 5.8},
{time: 1581787031000, magnitude: 5.86},
{time: 1584468750000, magnitude: 5.9},
{time: 1587150312000, magnitude: 5.92},
{time: 1589774866000, magnitude: 5.92},
{time: 1592452001000, magnitude: 5.89},
{time: 1595131865000, magnitude: 5.84},
{time: 1597812467000, magnitude: 5.78},
{time: 1600483424000, magnitude: 5.73},
{time: 1603154241000, magnitude: 5.7},
{time: 1605911414000, magnitude: 5.71},
{time: 1608582262000, magnitude: 5.74},
{time: 1611253273000, magnitude: 5.8},
{time: 1613928576000, magnitude: 5.85},
{time: 1616610245000, magnitude: 5.89},
{time: 1619222400000, magnitude: 5.91},
{time: 1621915163000, magnitude: 5.91},
{time: 1624592910000, magnitude: 5.87},
{time: 1627273229000, magnitude: 5.82},
{time: 1629952634000, magnitude: 5.77},
{time: 1632623576000, magnitude: 5.72},
{time: 1635294381000, magnitude: 5.69},
{time: 1638051551000, magnitude: 5.7},
{time: 1640722408000, magnitude: 5.73},
{time: 1643393435000, magnitude: 5.79},
{time: 1646070109000, magnitude: 5.85},
{time: 1648751734000, magnitude: 5.89},
{time: 1651363200000, magnitude: 5.9},
{time: 1654055576000, magnitude: 5.89},
{time: 1656733950000, magnitude: 5.86},
{time: 1659414636000, magnitude: 5.81},
{time: 1662092819000, magnitude: 5.75},
{time: 1664763744000, magnitude: 5.7},
{time: 1667434538000, magnitude: 5.67},
{time: 1670191707000, magnitude: 5.69},
{time: 1672862572000, magnitude: 5.73},
{time: 1675533617000, magnitude: 5.78},
{time: 1678211630000, magnitude: 5.84},
{time: 1680893222000, magnitude: 5.88},
{time: 1683504000000, magnitude: 5.89},
{time: 1686196120000, magnitude: 5.88},
{time: 1688875108000, magnitude: 5.84},
{time: 1691556070000, magnitude: 5.79},
{time: 1694233022000, magnitude: 5.73},
{time: 1696903931000, magnitude: 5.68},
{time: 1699574713000, magnitude: 5.66},
{time: 1702331881000, magnitude: 5.67},
{time: 1705002757000, magnitude: 5.72}],
        xkey: 'time',
        ykeys: ['magnitude'],
        labels: ['Magnitude'],
        hideHover: 'auto',
        xLabelAngle: 45,
        ymax: 5 ,
        ymin: 6,
        postUnits: ' mag',
        resize: true
    });
});
