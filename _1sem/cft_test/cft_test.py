#!/usr/bin/python3

import argparse


def sorting(arr, ascend):
    for n in range(1, len(arr)):
        j = n - 1
        key = arr[n]
        while (arr[j] > key) and (j >= 0):
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key
    if ascend:
        arr.reverse()
    return arr


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", type=str, help="input text file")
    parser.add_argument("output_file", type=str, help="output text file")

    type_group = parser.add_mutually_exclusive_group()
    type_group.add_argument('-i', action='store_true', dest="is_int",
                            help="for integer values in input_file (default)")
    type_group.add_argument('-s', action='store_false', dest="is_int", default=False,
                            help="for string values in input_file")

    scend_group = parser.add_mutually_exclusive_group()
    scend_group.add_argument('-a', action='store_false', dest="desc", default=False,
                             help="for ascending sort (default)")
    scend_group.add_argument('-d', action='store_true', dest="desc",
                             help="for descending sort")

    args = parser.parse_args()
    src = []
    try:
        with open(args.input_file, "r") as f:
            src = [int(i) if args.is_int else i for i in f.read().split("\n")]
        src = sorting(src, args.desc)
        with open(args.output_file, "w") as f:
            f.writelines("\n".join(str(i) for i in src))
    except Exception as err:
        print("{0}".format(err))
