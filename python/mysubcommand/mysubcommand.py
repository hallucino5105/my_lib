#!/usr/bin/env python
# coding: utf-8


import re
import subprocess


class mysubcommand():
    def __init__(self, cmd, sync=True):
        self.cmd  = cmd
        self.sync = sync

        self.returncode  = 0
        self.stdout_data = ""
        self.stderr_data = ""

        self.__execute()


    def __execute(self):
        p = subprocess.Popen(self.cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        if self.sync:
            self.stdout_data, self.stderr_data = p.communicate()
            self.returncode = p.returncode


    # 素直に出力を返す
    def print0(self):
        return self.returncode, self.stdout_data, self.stderr_data


    # 後方の空白削除して行を配列に
    def print1(self, discard_stderr=True):
        if self.returncode != 0:
            raise RuntimeError("subcommand returned error code: %d" % self.returncode)

        ret = []
        ret.append(self.stdout_data.rstrip().split("\n"))
        ret.append(self.stderr_data.rstrip().split("\n"))

        if discard_stderr:
            return ret[0]
        else:
            return tuple(ret)


    # さらに行毎に空白で分割
    def print2(self, discard_stderr=True, delimiter="\s+"):
        ret = list(self.print1(discard_stderr=False))
        ret[0] = [ re.split(delimiter, l) for l in ret[0] ]
        ret[1] = [ re.split(delimiter, l) for l in ret[1] ]

        if discard_stderr:
            return ret[0]
        else:
            return tuple(ret)

