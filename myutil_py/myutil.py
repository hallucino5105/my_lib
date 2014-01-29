#!/usr/bin/env python
# coding: utf-8


import sys
import pprint
import datetime


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
        return datetime.datetime.now()


    @staticmethod
    def now_string():
        return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


    @staticmethod
    def readfile(filepath):
        with open(filepath, "r") as f:
            content = f.read()
        return content


    @staticmethod
    def str_to_datetime(dt):
        return datetime.datetime.strptime(dt, "%Y-%m-%d %H:%M:%S")


    @staticmethod
    def datelist_generator(
            delta=60,
            start=datetime.datetime(1970, 01, 01, 00, 00, 00),
            end=datetime.datetime.now(),
            output_format="%Y-%m-%d %H:%M:00"):
        current = datetime.datetime.strptime(start.strftime(output_format), output_format)
        end = datetime.datetime.strptime(end.strftime(output_format), output_format)
        count = int((end - current).total_seconds() / delta) + 1

        datelist = []
        for c in range(count):
            datelist.append(current)
            current += datetime.timedelta(seconds=delta)

        return datelist


    @staticmethod
    def serialize(filepath, content):
        import msgpack

        with open(filepath, "w") as f:
            packed_content = msgpack.packb(content)
            f.write(packed_content)


    @staticmethod
    def deserialize(filepath):
        import os
        import msgpack

        if not os.path.isfile(filepath):
            raise RuntimeError("not found")

        with open(filepath, "r") as f:
            packed_content = f.read()
            return msgpack.unpackb(packed_content)

