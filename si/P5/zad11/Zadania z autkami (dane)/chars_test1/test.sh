#!/bin/bash

echo "Press ENTER after each test case"

for task_file in task*txt
do
    # echo "$task_file"
    python3 show_optimal_policy.py $task_file
done
