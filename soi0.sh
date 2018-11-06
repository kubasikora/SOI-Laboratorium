#!/bin/sh
find /* | awk '{print length, $0}' | sort -nr | head -1

