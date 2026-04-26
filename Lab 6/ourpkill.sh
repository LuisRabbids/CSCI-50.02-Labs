#!/bin/bash


# Error handling - check argument count (must be 1 or 2)
if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
    echo "Error: Invalid number of arguments." >&2
    echo "Usage: ./ourpkill.sh [-signal] pattern" >&2
    exit 1
fi

# Argument parsing
if [ "$#" -eq 1 ]; then
    SIGNAL="-15"
    PATTERN=$1
else
    SIGNAL=$1
    PATTERN=$2
    
    # Validation - ensure the optional signal argument actually starts with a dash '-'
    if [[ ! "$SIGNAL" == -* ]]; then
        echo "Error: Signal must start with a dash '-' (e.g., -9)." >&2
        exit 1
    fi
fi

# Call ourpgrep.sh to fetch the PIDs
PIDS=$(./ourpgrep.sh "$PATTERN")      # store the output of the script into a variable called PIDS

# If ourpgrep.sh threw an error, stop execution
if [ $? -ne 0 ]; then
    exit 1
fi

# Execute kill
if [ -z "$PIDS" ]; then
    exit 0         # if PIDS is empty (no matching processes) then exit
fi

# Loop through each PID found and send the signal
for PID in $PIDS; do
    # Run the default unix kill command
    kill $SIGNAL $PID 2>/dev/null     # redirect standard error (2>) to /dev/null to keep it quiet
    
    # If the kill command fails, output an error
    if [ $? -ne 0 ]; then
        echo "Error: Failed to send signal $SIGNAL to process $PID." >&2
    fi
done
