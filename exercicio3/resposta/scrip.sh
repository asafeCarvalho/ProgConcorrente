#!/bin/bash

# Script to execute a command a specified number of times.

# The command to be executed
COMMAND_TO_RUN="./main 10000131 27 4"

# Number of times to execute the command
NUM_EXECUTIONS=1000

echo "Starting execution of command: $COMMAND_TO_RUN"
echo "This command will be run $NUM_EXECUTIONS times."

# Loop to execute the command
for (( i=1; i<=$NUM_EXECUTIONS; i++ ))
do
  echo "Execution #$i: $COMMAND_TO_RUN"
  # Execute the command
  $COMMAND_TO_RUN

  # Optional: Add a small delay if needed, e.g., sleep 0.1 (for 0.1 seconds)
  # sleep 0.1

  # Optional: Check the exit status of the command
  # if [ $? -ne 0 ]; then
  #   echo "Error during execution #$i. Aborting."
  #   exit 1
  # fi
done

echo "Finished executing the command $NUM_EXECUTIONS times."
