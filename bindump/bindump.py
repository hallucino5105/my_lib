#!/usr/bin/python
# coding: utf-8


import sys
import string


def bindump(buf, opt=None):
    buflen = len(buf)
    i = 0

    while True:
        finish = False
        firstlen = 16 * i
        lastlen = 16 * (i + 1)

        if lastlen < buflen:
            slicebuf = buf[firstlen:lastlen]
        else:
            slicebuf = buf[firstlen:buflen]
            finish = True

        sys.stdout.write("%08x " % firstlen)

        for n in slicebuf:
            sys.stdout.write("%02x " % ord(n))

        # padding
        if finish:
            x = buflen % 16
            if x != 0:
                n = (16 - x) * 3
                sys.stdout.write(" " * n)

        for n in slicebuf:
            if n in string.printable[:-5]: # without \t,\n,\x,0b, \x, 0c, \r
                sys.stdout.write(n)
            else:
                sys.stdout.write(".")

        print

        if not finish:
            i += 1
        else:
            break



#def main(argv):
#    buf = "tes\a\tt\n" * 10
#    bindump(buf)
#
#
#if __name__ == '__main__':
#    main(sys.argv)
