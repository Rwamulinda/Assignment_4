#! /usr/bin/env python3
#
# An automated test for ISSE Assignment 4, the indexer
#
# Author: Howdy Pierce, howdy@sleepymoose.net
#
#  Usage: ./assignment4_tester.py (executable-name)

import pexpect
import sys
import re
import os

num_tests = 0
pub_dir = "/var/local/isse-04"


def run_once(executable, test_input, exit_code, expected_output):
    """
    Run one iteration of the executable under test (EUT).

    Parameters:
      executable    The executable to test

      test_input    A list of strings, which will be sequentially sent
                    into the EUT one per line.

      exit_code     The expected exit code from the EUT. This is either
                    zero or nonzero; likewise the EUT is expected to
                    exit with either a zero or nonzero exit code. However
                    if the exit code is nonzero it need not match exactly.

      expected_output  A list of strings, which are regular expressions
                    that must match the EUT's output, in the order
                    presented in the list.

    Returns 1 if this test passed, 0 otherwise.

    Saves a log file of the input and output from the EUT to pwd.

    Also prints to stdout a test summary
"""
    global num_tests
    num_tests += 1
    print(f"Test case {num_tests}: ", end='')

    child = pexpect.spawn(executable, encoding='utf-8', timeout=10)

    logname = f"a04_testcase_{num_tests}.log"
    flog = open(logname, mode='w')
    child.logfile_read = flog

    for line in test_input:
        child.sendline(line)
    child.sendeof()

    try:
        for line in expected_output:
            child.expect(line)
        child.expect(pexpect.EOF)
    except pexpect.TIMEOUT:
        print("FAIL (timeout)")
        return 0
    except pexpect.EOF:
        print("FAIL (crash or no output)")
        return 0

    child.close()
    flog.close()

    if (exit_code == 0 and child.exitstatus != 0):
        print(f"FAIL (child exited with {child.exitstatus}, expected 0)")
        return 0

    if (exit_code != 0 and child.exitstatus == 0):
        print(f"FAIL (child exited 0, expected non-zero)")
        return 0

    print("PASS")
    return 1


def run_and_compare(executable, input_file, output_file):
    """
    Run one iteration of the executable under test (EUT).

    Parameters:
      executable    The executable to test

      input_file    A file to use as the stdin for the EUT

      output_file   A file which represents the expected stdout from
                    the EUT. Note this is compared in a whitespace-
                    forgiving manner.

    Returns 1 if this test passed, 0 otherwise.
    """
    global num_tests
    num_tests += 1
    print(f"Test case {num_tests} (file {input_file}): ", end='')

    ec = os.system(f"{executable} < {input_file} 2>&1 "
                   f"| diff -qBw - {output_file} > /dev/null")
    if (ec != 0):
        print(f"FAIL (output did not match expected)")
        return 0

    print("PASS")
    return 1


def run_expect_error(executable, input_file):
    """
    Run one iteration of the executable under test (EUT), which is
    expected to exit with a nonzero error status. Output from the EUT
    is discarded.

    Parameters:
      executable    The executable to test

      input_file    A file to use as the stdin for the EUT

    Returns 1 if this test passed (in other words, the EUT correctly
    exited with a nonzero status), 0 otherwise.
    """

    global num_tests
    num_tests += 1
    print(f"Test case {num_tests} (file {input_file}): ", end='')

    ec = os.system(f"{executable} < {input_file} > /dev/null 2>&1 ")
    if (ec != 0):
        print(f"PASS")
        return 1

    print("FAIL (program did not exit with error as expected)")
    return 1


def outstr(word:str, num_occur:int, lines:list):
    r"""
    Returns a regex of the form
          word +\[<num-occur>]: *<lines>

    For instance, if the input is "alpha", 3, [1,2,3], output would be
          "alpha +\[3]: *1, *2, *3"

    The lines argument can be an int or a list of ints
    """
    if type(lines) is not list:
        lines = [lines]
    return (fr"{word} +\[{num_occur}]: *" + ", *".join(map(str,lines)))


def run_all_tests(executable):
    "Run all the tests against the specified executable"

    npass = 0

    # ordering and basic tests
    npass += run_once(executable, ["alpha", "bravo", "charlie", "delta"], 0,
                      [outstr("alpha", 1, 1), outstr("bravo", 1, 2),
                       outstr("charlie", 1, 3), outstr("delta", 1, 4)])

    npass += run_once(executable, ["delta", "charlie", "bravo", "alpha"], 0,
                      [outstr("alpha", 1, 4), outstr("bravo", 1, 3),
                       outstr("charlie", 1, 2), outstr("delta", 1, 1)])

    npass += run_once(executable,
                      ["alpha", "bravo", "Alpha", "charlie", "delta", "alpHA"],
                      0,
                      [outstr("alpha", 3, [1,3,6]), outstr("bravo", 1, 2),
                       outstr("charlie", 1, 4), outstr("delta", 1, 5)])

    npass += run_once(executable,
                      ["alpha "*3, "bravo "*4, "charlie "*2, "delta"], 0,
                      [outstr("bravo", 4, 2), outstr("alpha", 3, 1),
                       outstr("charlie", 2, 3), outstr("delta", 1, 4)])

    npass += run_once(executable,
                      ["alpha' alpha- alpha'", "bravo bravo bravo", "charlie"],
                      0,
                      [outstr("alpha", 3, 1), outstr("bravo", 3, 2),
                       outstr("charlie", 1, 3)])

    npass += run_once(executable,
                      ["al-pha alpha alpha's", "bravo alpha bravo bravo"], 0,
                      [outstr("bravo", 3, 2), outstr("alpha", 2, [1,2]),
                       outstr("al-pha", 1, 1), outstr("alpha's", 1, 1)])

    # long words but legal
    legal="abcdefghijklmnopqrst"
    npass += run_once(executable, [legal, legal, legal], 0,
                      [outstr(legal, 3, [1,2,3])])

    npass += run_once(executable, [f"{legal} {legal}+{legal}"], 0,
                      [outstr(legal, 3, 1)])

    # word too long
    npass += run_once(executable, [f"{legal}x"], 1, [""])

    # word appears on large number of lines, including error case
    npass += run_once(executable, ["alpha"]*10, 0,
                      [outstr("alpha", 10, list(range(1,11)))])

    npass += run_once(executable, ["alpha alpha"]*10, 0,
                      [outstr("alpha", 20, list(range(1,11)))])

    npass += run_once(executable, ["alpha alpha"]*12, 0,
                      [outstr("alpha", 24, list(range(1,13)))])

    npass += run_once(executable, ["alpha"]*20, 0,
                      [outstr("alpha", 20, list(range(1,21)))])

    # should fail: alpha appears too many times
    npass += run_once(executable, ["alpha"]*21, 1, [""])

    # long lines: should succeed
    npass += run_once(executable, ["aaaa "*50], 0, [outstr("aaaa", 50, 1)])
    npass += run_once(executable, ["w "*126+"!"], 0, [outstr("w", 126, 1)])
    # should fail: line too long
    npass += run_once(executable, ["w "*127], 1, [""])
    npass += run_once(executable, ["w "*127+"!"], 1, [""])

    # empty input, input with just common words
    npass += run_once(executable, [""]*3, 1, ["No words found"])
    npass += run_once(executable,
                      ["", "a", "about", "after", "all", "also", "an", "and",
                       "any", "are", "as", "at", "back", "be", "because",
                       "but", "by", "can", "come", "could", "day", "do",
                       "even", "first", "for", "from", "get", "give", "go",
                       "good", "have", "he", "he", "her", "him", "his",
                       "how", "i", "if", "in", "into", "is", "it", "its",
                       "it's", "just", "know", "like", "look", "make", "me",
                       "most", "my", "new", "no", "not", "now", "of", "on",
                       "one", "only", "or", "other", "our", "out", "over",
                       "people", "say", "see", "she", "so", "some", "take",
                       "than", "that", "the", "their", "them", "then",
                       "there", "these", "they", "think", "this", "time",
                       "to", "two", "up", "us", "use", "want", "way", "we",
                       "well", "we'll", "what", "when", "which", "who",
                       "will", "with", "work", "would", "year", "you",
                       "your", "", ""], 1, ["No words found"])

    for basefile in ["love", "wasteland", "caesar", "1200_words"]:
        infile = os.path.join(pub_dir, f"{basefile}.txt")
        outfile = os.path.join(pub_dir, f"{basefile}.out")
        npass += run_and_compare(executable, infile, outfile)

    infile = os.path.join(pub_dir, f"1201_words.txt")
    npass += run_expect_error(executable, infile)

    print(f"Passed {npass} out of {num_tests}")


if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print(f"Usage: {sys.argv[0]} (executable-name)")
        sys.exit(1)

    run_all_tests(sys.argv[1])
