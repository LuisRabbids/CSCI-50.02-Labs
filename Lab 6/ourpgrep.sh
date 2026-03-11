#!/bin/bash


# Error Handling - check if exactly one argument is provided
if [ "$#" -ne 1 ]; then
    echo "Error: Exactly one argument (process name pattern) is required." >&2
    echo "Usage: ./ourpgrep.sh <pattern>" >&2
    exit 1
fi

PATTERN=$1


# Process output
# - ps aux: Lists all processes
# - grep "^$USER": Filters for rows that START with (^) the current user's name
# - grep "$PATTERN": Searches the remaining lines for the provided pattern
# - grep -v "grep": Excludes the 'grep' command itself from the search results
# - grep -v "$0": Excludes THIS script from showing up in the results
# - tr -s " ": Squeezes multiple spaces into a single space
# - cut -d " " -f2: Cuts the string using a space as the delimiter and grabs the 2nd field (the PID)
ps aux | grep "^$USER" | grep "$PATTERN" | grep -v "grep" | grep -v "$0" | tr -s " " | cut -d " " -f2
