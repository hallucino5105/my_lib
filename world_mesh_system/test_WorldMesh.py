#!/usr/bin/env python2.7
# coding: utf-8


import sys
import os
import unittest
import decimal

sys.path.insert(0, os.path.expanduser("~/ym0_program/mylib/world_mesh_system_py"))
from WorldMesh import WorldMesh


class WorldMeshTest(unittest.TestCase):
    def setUp(self):
        self.test_instance = WorldMesh("35.449", "139.63")


    def tearDown(self):
        pass


    def test1(self):
        self.assertIsInstance(WorldMesh("35.449", "139.63"), WorldMesh)
        self.assertIsInstance(WorldMesh("35.449", "-139.63"), WorldMesh)
        self.assertIsInstance(WorldMesh("-35.449", "139.63"), WorldMesh)
        self.assertIsInstance(WorldMesh("-35.449", "-139.63"), WorldMesh)
        self.assertIsInstance(WorldMesh("0", "0"), WorldMesh)
        self.assertIsInstance(WorldMesh("-90", "180"), WorldMesh)
        self.assertIsInstance(WorldMesh("90", "180"), WorldMesh)
        self.assertIsInstance(WorldMesh("-90", "-180"), WorldMesh)
        self.assertIsInstance(WorldMesh("90", "-180"), WorldMesh)
        self.assertIsInstance(WorldMesh("90.0", "180.0"), WorldMesh)
        self.assertIsInstance(WorldMesh("1e-1", "1e-1"), WorldMesh)

        with self.assertRaises(TypeError): WorldMesh(35.449, 139.63)
        with self.assertRaises(TypeError): WorldMesh(True, True)
        with self.assertRaises(TypeError): WorldMesh(False, False)
        with self.assertRaises(TypeError): WorldMesh(None, None)

        with self.assertRaises(ValueError): WorldMesh("90.01", "0")
        with self.assertRaises(ValueError): WorldMesh("0", "180.0000000000000000000000000000000001")
        with self.assertRaises(ValueError): WorldMesh("-90.001", "0")
        with self.assertRaises(ValueError): WorldMesh("0", "-180.001")
        with self.assertRaises(ValueError): WorldMesh("1135.449", "11139.63")
        with self.assertRaises(ValueError): WorldMesh("-1135.449", "-11139.63")
        with self.assertRaises(ValueError): WorldMesh("1135.449", "-11139.63")
        with self.assertRaises(ValueError): WorldMesh("-1135.449", "11139.63")

        with self.assertRaises(decimal.InvalidOperation): WorldMesh("a", "a")
        with self.assertRaises(decimal.InvalidOperation): WorldMesh("123.000hoge", "123.000hage")


    def test2(self):
        t = self.test_instance

        self.assertIsInstance(t.GetMesh1String(5), str)

        with self.assertRaises(TypeError): t.GetMesh1String("5")
        with self.assertRaises(TypeError): t.GetMesh1String(-0.1)
        with self.assertRaises(TypeError): t.GetMesh1String(19.1)
        with self.assertRaises(TypeError): t.GetMesh1String(None)
        with self.assertRaises(TypeError): t.GetMesh1String(True)

        with self.assertRaises(ValueError): t.GetMesh1String(-1)
        with self.assertRaises(ValueError): t.GetMesh1String(100)


def test_main():
    unittest.main()


def main():
    test_main()
    pass


if __name__ == "__main__":
    main()

