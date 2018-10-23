import subprocess

#making unit tests for different features
#do this instead of using file scripts
#because file scripts may be broken
#after changes

def main():
    test_input = ["1 + 2",
                  "sin(31)",
                  "pi * 2",
                  "ans + 1",
                  "sin(ans)",
                  "1 + 2 * 3",
                  "2 + 3 * sin(5)",
                  "a = zeros(3)",
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

    p = subprocess.Popen(["./calc2"], shell = True, stdin = subprocess.PIPE);
    for line in test_input:
        b = bytearray();
        b.extend(line.encode("ascii"));
        b.append(13);
        p.stdin.write(b);


if __name__ == "__main__":
    main();
