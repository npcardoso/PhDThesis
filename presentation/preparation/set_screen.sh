#!/bin/sh

set -e

xrandr --output HDMI2 --off --output HDMI1 --off --output eDP1 --off --output DP2 --off
xrandr --output DP1 --auto --primary
xrandr --output eDP1 --mode 2560x1600 --pos 1680x0 --rotate normal --below DP1
