#!/bin/bash
make all

template="S"

for x in e i v c l n h s o
do
    for y in e i v c l n h s o
    do
        flag="$x$y"
        test_number=0
        echo " "
        echo "Flag -$flag:"
        for test_file in "./tests_grep/test_text_0.txt" "./tests_grep/test_text_1.txt" "./tests_grep/test_text_2.txt" "./tests_grep/test_text_3.txt" "./tests_grep/test_text_4.txt" "./tests_grep/test_text_5.txt" "./tests_grep/test_text_6.txt" "./tests_grep/test_text_7.txt" "./tests_grep/test_text_ptrn.txt"
        do
            ./s21_grep -$flag $template $test_file > 1.txt
            grep -$flag $template $test_file > 2.txt
            if ! cmp -s 1.txt 2.txt ; then
                echo "Test $test_number: Fail"
            fi
            rm 1.txt 2.txt
            test_number=$(( $test_number + 1 ))
        done
    done
done