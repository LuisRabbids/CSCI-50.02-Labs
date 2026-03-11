#!/bin/bash


# Error handling - check if exactly one argument is provided
if [ "$#" -ne 1 ]; then
    echo "Error: Exactly one argument (process name pattern) is required." >&2
    echo "Usage: ./ourpgrep.sh <pattern>" >&2
    exit 1
fi

PATTERN=$1


# Process output
# - ps aux: Lists all processes
# - grep "^$USER": Filters for rows that start with ^ the current user's name
# - grep "$PATTERN": Searches the remaining lines for the provided pattern
# - grep -v "grep": Excludes the grep command itself from the search results
# - grep -v "$0": Excludes this script from showing up in the results
# - grep -v "ourpkill": Excludes the parent kill script so it doesnt accidentally kill itself
# - tr -s " ": Squeezes multiple spaces into a single space
# - cut -d " " -f2: Cuts the string using a space as the delimiter and grabs the 2nd field (the PID)
ps aux | grep "^$USER" | grep "$PATTERN" | grep -v "grep" | grep -v "$0" | grep -v "ourpkill" | tr -s " " | cut -d " " -f2
