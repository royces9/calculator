#!/usr/bin/python

import subprocess

#test matrix stuff with this file

def main():
    test_input = ["a = zeros(3)",
                  "a(2) = 3",
                  "a(2, 3) = 1",
                  "b = [4; 8; 9];",
                  "a(b) = 6",
                  "a(b) = a(b) + 2",
                  "a([3, 5, 6]) = 2",
                  "c = b * a",
                  "c = a * b",
                  "c .* b",
                  "a * 5",
                  "integral(x^x, x, 0, 1, 10000)",
    ];

    p = subprocess.Popen(["./calc_d"], shell = True, stdin = subprocess.PIPE);
    for line in test_input:
        b = bytearray();
        b.extend(line.encode("ascii"));
        b.append(13);
        p.stdin.write(b);


if __name__ == "__main__":
    main();
