# -------------------------------------------------
# Copyright 2015-2019 Dominic Ford
#
# This file is part of StarCharter.
#
# StarCharter is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# StarCharter is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with StarCharter.  If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------

# Default settings which affect all the charts below
DEFAULTS
#ra_central=8.66
#dec_central=68.2
#angular_width=29.0
mag_min=7
width=15.0
aspect=1.41421356
ra_dec_lines=1
messier_names=1
plot_messier=1
constellation_boundaries=1
constellation_sticks=1
coords=ra_dec
projection=gnomonic
star_names=1
star_flamsteed_labels=0
constellation_names=1
plot_galaxy_map=1
plot_equator=1
plot_ecliptic=0
plot_galactic_plane=1
font_size=1.2
draw_ephemeris=IDIDID,STASTASTA,ENDENDEND
ephemeris_compute_path=$SHAREDDIR/ephemeris-compute/bin/ephem.bin
ephemeris_col=1
ephemeris_autoscale=1

# Produce a PNG copy of this star chart
#CHART
#output_filename=output/FNAFNAFNA.png

# Produce a smaller version of this chart
CHART
width=12
font_size=1.1
output_filename=output/FNAFNAFNA-mini.png

# Produce a SVG copy of this star chart
#CHART
#output_filename=output/FNAFNAFNA.svg

# Produce a PDF copy of this star chart
#CHART
#output_filename=output/FNAFNAFNA.pdf

# Produce an EPS copy of this star chart
#CHART
#output_filename=output/FNAFNAFNA.eps


