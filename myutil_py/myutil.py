#!/usr/bin/env python
# coding: utf-8


# usage:
# sys.path.append(os.path.expanduser("~/my_lib/myutil_py"))
# from myutil import myutil

import sys
import pprint
import time
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
    def readfile(filepath):
        with open(filepath, "r") as f:
            content = f.read()
        return content


    @staticmethod
    def now():
        return datetime.datetime.now()


    @staticmethod
    def now_string():
        return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


    @staticmethod
    def datetime_to_str(dtime):
        return dtime.strftime("%Y-%m-%d %H:%M:%S")


    @staticmethod
    def str_to_datetime(stime):
        return datetime.datetime.strptime(stime, "%Y-%m-%d %H:%M:%S")


    @staticmethod
    def datetime_to_seconds(dtime):
        return time.mktime(datetime.datetime.timetuple(dtime))


    @staticmethod
    def seconds_to_datetime(seconds):
        return datetime.datetime.fromtimestamp(seconds)


    @staticmethod
    def datetime_alignment(dtime, interval):
        s = int(myutil.datetime_to_seconds(dtime))
        return myutil.seconds_to_datetime(s - s % interval)


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


    @staticmethod
    def trim_text(text):
        import re

        formatted_text1 = text.replace("\n", "")
        formatted_text2 = re.sub(r"\s+", " ", formatted_text1)

        return formatted_text2


    @staticmethod
    def interpolate(x1, y1, x2, kind="linear", bounds_error=False, fill_value=0):
        """ kind = [ "linear", "quadratic", "cubic" ] とか？ """

        from scipy import interpolate

        f  = interpolate.interp1d(x1, y1, kind=kind, bounds_error=bounds_error, fill_value=fill_value)
        y2 = f(x2)

        return zip(x2, y2)


    @staticmethod
    def interpolate_term(x1, y1, x2, kind="linear"):
        from scipy import interpolate

        x1_ = [ 0, len(x2)-1 ]
        x2_ = [ i for i in range(len(x2)) ]
        f   = interpolate.interp1d(x1_, y1, kind=kind)
        y2  = f(x2_)

        return zip(x2, y2)

