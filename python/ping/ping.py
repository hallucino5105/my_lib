#!/usr/bin/env python
# coding: utf-8


import sys
import os

sys.path.extend([
    "./my_lib/python/myutil",
    "/home/hoshino/my_lib/python/myutil",
    "/Users/hoshino/my_lib/python/myutil",
])
from myutil import myutil, mout, merr


def ping(hosts):
    ret = []

    for host in hosts:
        p = subprocess.Popen(
            ["ping", "-c", "1", host],
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE
        )
        out, error = p.communicate()
        if error:
            ret.append(( False, host, "ServerName->" + host + ", Message->\"" + error.rstrip() + "\"" ))
        else:
            ret.append(( True, host, "ServerName->" + host ))

    return ret

