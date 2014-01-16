#!/usr/bin/env python
# coding: utf-8


import sys
from datetime import datetime
from pprint import pprint


def merr(message, newline=True, flush=False):
    if type(message) == str or type(message) == unicode:
        sys.stderr.write(message)
    else:
        pprint(message, sys.stderr)

    if newline:
        sys.stderr.write("\n")
    if flush:
        sys.stderr.flush()


def mout(message, newline=True, flush=False):
    if type(message) == str or type(message) == unicode:
        sys.stdout.write(message)
    else:
        pprint(message, sys.stdout)

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

