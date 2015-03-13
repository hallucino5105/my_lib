#!/usr/bin/env python
# coding: utf-8


import sys
import os
import re
import math


class Filesize():
    unit = [
        "B", "_dummy_",
        "KB", "KiB",
        "MB", "MiB",
        "GB", "GiB",
        "TB", "TiB",
        "PB", "PiB",
        "EB", "EiB",
        "ZB", "ZiB",
        "YB", "YiB",
    ]


    def __init__(self, conversionRate=1024):
        self.conversionRate = conversionRate


    def _splitUnit(self, unitBytes):
        (byte, unit) = re.match(r"([0-9\.,]*)([a-zA-Z]*)", unitBytes).groups()

        byte = byte.replace(r",", "")
        unit = unit.lower()

        if unit not in [ su.lower() for su in Filesize.unit ]:
            unit = ""

        return float(byte), unit


    def _getUnitExp(self, unit):
        for index, fu in enumerate(Filesize.unit):
            if unit.lower() == fu.lower():
                return int(math.floor(index / 2))

        return 0


    def toRawBytes(self, unitBytes):
        byte, unit = self._splitUnit(unitBytes)
        unitExp = self._getUnitExp(unit)

        return int(byte * (self.conversionRate ** unitExp))


    def toUnitBytes(self, rawBytes, targetUnit="MB"):
        if type(rawBytes) == str:
            rawBytes = int(rawBytes.replace(r",", ""))
        unitExp = self._getUnitExp(targetUnit)

        return rawBytes * (self.conversionRate ** unitExp)

