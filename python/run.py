import unittest
from hashtable import Table


class TestsHashTable(unittest.TestCase):
    def test_one(self):
        table = Table(45000)
        self.assertEqual(45000, table.size, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")
        del table

        #try:
        #    table = Table(0)
        #except Exception as e:
        #    self.assertEqual(type(e), ValueError)

    def test_two(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000)
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        for n in range(1, 5001):
            in_ = table.insert("key_" + str(n), n)
            self.assertGreaterEqual(in_, 0)
            self.assertLess(in_, table.size)

        self.assertEqual(table.count, 5000)
        self.assertFalse(table.empty)
        for m in range(1, 5000):
            value = table.get("key_" + str(m))
            self.assertEqual(value, m)

        del table

    def test_three(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        for n in range(1, 5000 + 1):
            in_ = table.insert("key_" + str(n), n)
            self.assertGreaterEqual(in_, 0)
            self.assertLess(in_, table.size)

        self.assertEqual(table.count, 5000)
        self.assertFalse(table.empty)

        for m in range(1, 50 + 1):
            check = table.delete("key_" + str(m))
            self.assertTrue(check)
        self.assertEqual(table.count, 4950)
        self.assertFalse(table.empty)

        for m in range(100, 1100):
            check = table.delete("key_" + str(m))
            self.assertTrue(check)
        self.assertEqual(table.count, 3950)
        self.assertFalse(table.empty)

        for m in range(0, 50 + 1):
            value = table.get("key_" + str(m))
            self.assertEqual(value, None)

        for m in range(51, 100):
            value = table.get("key_" + str(m))
            self.assertEqual(value, m)

        for m in range(100, 1100):
            value = table.get("key_" + str(m))
            self.assertEqual(value, None)

        del table

    def test_four(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        for n in range(1, 5000 + 1):
            try:
                in_ = table.insert(n, n)
                self.assertGreaterEqual(in_, 0)
                self.assertLess(in_, table.size)
            except Exception as e:
                self.assertEqual(type(e), TypeError)

        del table

    def test_five(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        for n in range(1, 5000 + 1):
            in_ = table.insert("key_" + str(n), n)
            self.assertGreaterEqual(in_, 0)
            self.assertLess(in_, table.size)

        self.assertEqual(5000, table.count, msg="Must be zero.")
        self.assertFalse(table.empty, msg="The table must be empty.")

        for m in range(1, 5000 + 1):
            try:
                table.delete(m)
            except Exception as e:
                self.assertEqual(type(e), TypeError)

        del table

    def test_six(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        table.insert("key_1", 1)

        for m in range(2, 5000 + 1):
            check = table.delete("key_" + str(m))
            self.assertFalse(check)

        del table

    def test_seven(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        for n in range(1, 5000 + 1):
            in_ = table.insert("key_" + str(n), n)
            self.assertGreaterEqual(in_, 0)
            self.assertLess(in_, table.size)

        self.assertEqual(5000, table.count, msg="Must be zero.")
        self.assertFalse(table.empty, msg="The table must be empty.")

        for m in range(1, 5000 + 1):
            value = table.get("key_" + str(m))
            self.assertEqual(value, m)

        for b in range(1, 5000 + 1):
            check = table.delete("key_" + str(b))
            self.assertTrue(check)
        self.assertEqual(table.count, 0)
        self.assertTrue(table.empty)

        del table

    def test_eight(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        for n in range(1, 5000 + 1):
            in_ = table.insert("key_" + str(n), n)
            self.assertGreaterEqual(in_, 0)
            self.assertLess(in_, table.size)

        self.assertEqual(5000, table.count, msg="Must be zero.")
        self.assertFalse(table.empty, msg="The table must be empty.")

        key, value = table.find("key_4567")
        self.assertEqual(key, "key_4567")
        self.assertEqual(value, 4567)

        key, value = table.find("key_12")
        self.assertEqual(key, "key_12")
        self.assertEqual(value, 12)

        key, value = table.find("key_2342")
        self.assertEqual(key, "key_2342")
        self.assertEqual(value, 2342)

        key, value = table.find("key_795456")
        self.assertEqual(key, "")
        self.assertEqual(value, None)

        del table

    def test_eight(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        for n in range(1, 5000 + 1):
            in_ = table.insert("key_" + str(n), n)
            self.assertGreaterEqual(in_, 0)
            self.assertLess(in_, table.size)

        self.assertEqual(5000, table.count, msg="Must be zero.")
        self.assertFalse(table.empty, msg="The table must be empty.")

        key, value = table.at(4567)
        self.assertIs(type(key), type(""))
        self.assertIs(type(value), type(0))

        key, value = table.at(12)
        self.assertIs(type(key), type(""))
        self.assertIs(type(value), type(0))

        key, value = table.at(2342)
        self.assertIs(type(key), type(""))
        self.assertIs(type(value), type(0))

        key, value = table.at(78456)
        self.assertEqual(key, "")
        self.assertEqual(value, None)

        check = table.delete("key_675")
        self.assertTrue(check)

        in_ = table.insert("key86345", 67435)
        self.assertGreaterEqual(in_, 0)
        self.assertLess(in_, table.size)

        key, value = table.at(in_)
        self.assertEqual(key, "key86345")
        self.assertEqual(value, 67435)

        check = table.delete("key86345")
        self.assertTrue(check)

        key, value = table.at(in_)
        self.assertEqual(key, None)
        self.assertEqual(value, None)

        del table

    def test_nine(self):
        table = Table(5000)
        self.assertEqual(table.size, 5000, msg="The values are not the same.")
        self.assertEqual(0, table.count, msg="Must be zero.")
        self.assertTrue(table.empty, msg="The table must be empty.")

        for n in range(1, 5000 + 1):
            in_ = table.insert("key_" + str(n), n)
            self.assertGreaterEqual(in_, 0)
            self.assertLess(in_, table.size)

        self.assertEqual(5000, table.count, msg="Must be zero.")
        self.assertFalse(table.empty, msg="The table must be empty.")

        index = 1
        for item in table.items():
            self.assertIs(type(item), type(tuple()))
            self.assertEqual(item[0], "key_" + str(index))
            self.assertEqual(item[1], index)
            index += 1

        del table


if __name__ == "__main__":
    unittest.main()
