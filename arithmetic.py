#!/usr/bin/python

import subprocess

#test arithmetic with this file

def main():
    test_input = ["1 + 2",
                  "(1) + (2)",
                  "1) + (2)",
                  "(1 + )2)",
                  "(((1) + (2)))",
                  "2 ^ (1 + 4 * 3)",
                  "2 ^ (((1) + (4 * 3)))",
                  "sin(1)",
                  "2 * sin(1)",
                  "sin(1) * 2",
                  "3 + 2 * sin(1)",
                  "2 * sin(1) + 3",
                  ];

    p = subprocess.Popen(["./calc2"], shell = True, stdin = subprocess.PIPE);

    for line in test_input:
        b = bytearray();
        b.extend(line.encode("ascii"));
        b.append(13);
        p.stdin.write(b);


if __name__ == "__main__":
    main();


                                                    
