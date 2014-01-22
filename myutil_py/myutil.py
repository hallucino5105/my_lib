#!/usr/bin/env python
# coding: utf-8


import sys
import pprint
from datetime import datetime


class MyPrettyPrinter(pprint.PrettyPrinter):
    def format(self, object, context, maxlevels, level):
        if isinstance(object, unicode):
            return (object.encode('utf8'), True, False)
        return pprint.PrettyPrinter.format(self, object, context, maxlevels, level)


def merr(message, newline=True, flush=False):
    if isinstance(message, str) or isinstance(message, unicode):
        sys.stderr.write(message)
    else:
        MyPrettyPrinter(stream=sys.stderr).pprint(message)

    if newline:
        sys.stderr.write("\n")
    if flush:
        sys.stderr.flush()


def mout(message, newline=True, flush=False):
    if isinstance(message, str) or isinstance(message, unicode):
        sys.stdout.write(message)
    else:
        MyPrettyPrinter(stream=sys.stdout).pprint(message)

    if newline:
        sys.stdout.write("\n")
    if flush:
        sys.stdout.flush()


class myutil:
    @staticmethod
    def now():
        return datetime.now()

    @staticmethod
    def now_string():
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    @staticmethod
    def readfile(filepath):
        with open(filepath, "r") as f:
            content = f.read()
        return content

