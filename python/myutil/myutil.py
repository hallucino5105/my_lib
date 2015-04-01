#!/usr/bin/env python
# coding: utf-8


# usage:
# sys.path.append(os.path.expanduser("~/my_lib/myutil_py"))
# from myutil import myutil

import sys
import os
import pprint
import time
import datetime
import re
import Queue


class MyPrettyPrinter(pprint.PrettyPrinter):
    def format(self, object, context, maxlevels, level):
        if isinstance(object, unicode):
            return (object.encode('utf8'), True, False)
        return pprint.PrettyPrinter.format(self, object, context, maxlevels, level)


def merr(message="", newline=True, flush=False):
    if not message:
        sys.stderr.write("\n")

    else:
        if isinstance(message, str) or isinstance(message, unicode):
            sys.stderr.write(message)
        else:
            MyPrettyPrinter(stream=sys.stderr).pprint(message)

        if newline:
            sys.stderr.write("\n")

    if flush:
        sys.stderr.flush()


def mout(message="", newline=True, flush=False):
    if not message:
        sys.stdout.write("\n")

    else:
        if isinstance(message, str) or isinstance(message, unicode):
            sys.stdout.write(message)
        else:
            MyPrettyPrinter(stream=sys.stdout).pprint(message)

        if newline:
            sys.stdout.write("\n")

    if flush:
        sys.stdout.flush()


class myutil:
    @classmethod
    def readfile(cls, filepath):
        with open(filepath, "r") as f:
            content = f.read()
        return content


    @classmethod
    def now(cls):
        return datetime.datetime.now()


    @classmethod
    def nowString(cls, fmt="%Y-%m-%d %H:%M:%S"):
        return datetime.datetime.now().strftime(fmt)


    @classmethod
    def datetimeToStr(cls, dtime, fmt="%Y-%m-%d %H:%M:%S"):
        return dtime.strftime(fmt)


    @classmethod
    def strToDatetime(cls, stime, fmt="%Y-%m-%d %H:%M:%S"):
        return datetime.datetime.strptime(stime, fmt)


    @classmethod
    def datetimeToSeconds(cls, dtime):
        return time.mktime(datetime.datetime.timetuple(dtime))


    @classmethod
    def secondToDatetime(cls, seconds):
        return datetime.datetime.fromtimestamp(seconds)


    @classmethod
    def datetimeAlignment(cls, dtime, interval):
        s = int(cls.datetimeToSeconds(dtime))
        return cls.secondsToDatetime(s - s % interval)


    @classmethod
    def datelistGenerator(
        cls,
        delta=60,
        start=datetime.datetime(1970, 01, 01, 00, 00, 00),
        end=datetime.datetime.now(),
        outputFormat="%Y-%m-%d %H:%M:00"
    ):
        current = datetime.datetime.strptime(start.strftime(outputFormat), outputFormat)
        end = datetime.datetime.strptime(end.strftime(outputFormat), outputFormat)

        td = end - current
        totalSeconds = (td.microseconds + (td.seconds + td.days * 24 * 3600) * 10**6) / 10**6
        #print totalSeconds, td.totalSeconds()

        #count = int((end - current).totalSeconds() / delta) + 1
        count = int(totalSeconds / delta) + 1

        datelist = []
        for c in range(count):
            datelist.append(current)
            current += datetime.timedelta(seconds=delta)

        return datelist


    @classmethod
    def serialize(cls, filepath, content):
        import msgpack

        with open(filepath, "w") as f:
            packedContent = msgpack.packb(content)
            f.write(packedContent)


    @classmethod
    def deserialize(cls, filepath):
        import msgpack

        if not os.path.isfile(filepath):
            raise RuntimeError("not found")

        with open(filepath, "r") as f:
            packedContent = f.read()
            return msgpack.unpackb(packedContent)


    # 連続したスペースを単独スペースに置換する
    @classmethod
    def trimText(cls, text, replaceSpace=True, replaceNewLine=False):
        formattedText = re.sub(r" +", " ", text) if replaceSpace else text
        formattedText = formattedText.replace("\n", "") if replaceNewLine else formattedText
        return formattedText


    """ kind = [ "linear", "quadratic", "cubic" ] とか？ """
    @classmethod
    def interpolate(cls, x1, y1, x2, x2Alias=None, kind="linear", boundsError=False, fillValue=0):
        from scipy import interpolate

        def xMapper(x1, x2):
            x1i = [ x2.index(v) for v in x1 ]
            x2i = [ i for i, v in enumerate(x2) ]
            return x1i, x2i

        x1m, x2m = xMapper(x1, x2)
        #print x1m, x2m

        f  = interpolate.interp1d(x1m, y1, kind=kind, boundsError=boundsError, fillValue=fillValue)
        y2 = f(x2m)
        #print x1, x2, x1m, x2m, y1, y2

        if not x2Alias:
            return zip(x2, y2)
        else:
            return zip(x2Alias, y2)


    @classmethod
    def interpolateTerm(cls, x1, y1, x2, kind="linear"):
        x1_ = [ 0, len(x2)-1 ]
        x2_ = [ i for i in range(len(x2)) ]

        return cls.interpolate(x1=x1_, y1=y1, x2=x2_, x2_=x2, kind=kind)

        #f   = interpolate.interp1d(x1_, y1, kind=kind)
        #y2  = f(x2_)

        #return zip(x2, y2)

    # zipだとtupleで帰ってくるので
    # list版を作る
    @classmethod
    def zip2(cls, *elem):
        ret = zip(*elem)
        for i in xrange(len(ret)):
            ret[i] = list(ret[i])

        return ret


    @classmethod
    def __bufferedReader(cls, count, fileOpener, rowReader):
        fo = fileOpener()
        rowBuf = []

        while True:
            for i in xrange(count):
                try:
                    row = rowReader(fo)
                    if not row:
                        yield rowBuf
                        return

                except StopIteration:
                    yield rowBuf
                    return

                rowBuf.append(row)

            yield rowBuf
            rowBuf = []

        fo.close()


    @classmethod
    def bufferedReader(cls, filepath, count=1):
        fileOpener = lambda: open(filepath, "r")
        rowReader = lambda fo: fo.readline()
        return cls.__bufferedReader(count, fileOpener, rowReader)


    @classmethod
    def bufferedReaderCSV(cls, filepath, options={ "delimiter": ",", "lineterminator": "\n" }, count=1):
        import csv
        fileOpener = lambda: csv.reader(open(filepath, "r"), **options)
        rowReader = lambda fo: fo.next()
        return cls.__bufferedReader(count, fileOpener, rowReader)

