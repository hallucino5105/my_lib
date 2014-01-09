#!/usr/bin/env python
# coding: utf-8


import sys


def merr(message, newline=True, flush=False):
    sys.stderr.write(message)
    if newline:
        sys.stderr.write("\n")
    if flush:
        sys.stderr.flush()


def mout(message, newline=True, flush=False):
    sys.stdout.write(message)
    if newline:
        sys.stdout.write("\n")
    if flush:
        sys.stdout.flush()


class MyUtil:
    pass

