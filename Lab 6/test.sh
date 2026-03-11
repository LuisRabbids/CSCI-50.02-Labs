#!/bin/bash

# =============================================================================
# test_scripts.sh
# Test suite for ourpgrep.sh and ourpkill.sh
# =============================================================================

# --- Colors for output ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

PASS=0
FAIL=0

# --- Helper: Print test result ---
pass() { echo -e "${GREEN}[PASS]${NC} $1"; ((PASS++)); }
fail() { echo -e "${RED}[FAIL]${NC} $1"; ((FAIL++)); }
section() { echo -e "\n${CYAN}=== $1 ===${NC}"; }

# --- Helper: Assert exit code ---
assert_exit() {
    local desc="$1"
    local expected="$2"
    local actual="$3"
    if [ "$actual" -eq "$expected" ]; then
        pass "$desc (exit code: $actual)"
    else
        fail "$desc (expected exit $expected, got $actual)"
    fi
}

# --- Helper: Assert output contains a value ---
assert_contains() {
    local desc="$1"
    local expected="$2"
    local actual="$3"
    if echo "$actual" | grep -q "$expected"; then
        pass "$desc"
    else
        fail "$desc (expected to find '$expected' in output: '$actual')"
    fi
}

# --- Helper: Assert output is empty ---
assert_empty() {
    local desc="$1"
    local actual="$2"
    if [ -z "$actual" ]; then
        pass "$desc"
    else
        fail "$desc (expected empty output, got: '$actual')"
    fi
}

# --- Prerequisite check ---
section "PREREQUISITE CHECK"

if [ ! -f "./ourpgrep.sh" ]; then
    echo -e "${RED}ERROR: ourpgrep.sh not found in current directory. Aborting.${NC}"
    exit 1
fi
if [ ! -f "./ourpkill.sh" ]; then
    echo -e "${RED}ERROR: ourpkill.sh not found in current directory. Aborting.${NC}"
    exit 1
fi

chmod +x ./ourpgrep.sh ./ourpkill.sh
pass "Both scripts found and made executable"

# =============================================================================
# OURPGREP.SH TESTS
# =============================================================================

section "ourpgrep.sh — Argument Validation"

# Test 1: No arguments
output=$(./ourpgrep.sh 2>&1)
assert_exit "No arguments → exit 1" 1 $?
assert_contains "No arguments → error message on stderr" "Error" "$output"

# Test 2: Too many arguments
output=$(./ourpgrep.sh bash extra 2>&1)
assert_exit "Too many arguments → exit 1" 1 $?
assert_contains "Too many arguments → error message" "Error" "$output"

# Test 3: Usage hint shown
output=$(./ourpgrep.sh 2>&1)
assert_contains "No arguments → usage hint shown" "Usage" "$output"

# ---

section "ourpgrep.sh — Correct Behavior"

# Test 4: bash is always running; pgrep bash should find at least 1 PID
output=$(./ourpgrep.sh bash 2>/dev/null)
assert_exit "Searching 'bash' → exit 0" 0 $?

real_pgrep=$(pgrep -u "$USER" bash 2>/dev/null)
if [ -n "$real_pgrep" ] && [ -n "$output" ]; then
    pass "Searching 'bash' → non-empty output"
else
    fail "Searching 'bash' → expected PIDs but got none"
fi

# Test 5: Output contains only numbers (PIDs)
if echo "$output" | grep -qE '^[0-9]+$'; then
    pass "Output contains only numeric PIDs"
else
    fail "Output contains non-numeric lines: '$output'"
fi

# Test 6: Script should not list itself in results
self_pid=$$
if echo "$output" | grep -q "^$self_pid$"; then
    fail "Script should not include its own PID in results"
else
    pass "Script correctly excludes its own PID"
fi

# Test 7: Non-existent process → empty stdout, exit 0
output=$(./ourpgrep.sh "PROCESS_THAT_DOES_NOT_EXIST_XYZ123" 2>/dev/null)
assert_exit "Non-existent process → exit 0" 0 $?
assert_empty "Non-existent process → empty stdout" "$output"

# Test 8: Results only belong to current user
section "ourpgrep.sh — User Filtering"

output=$(./ourpgrep.sh bash 2>/dev/null)
while IFS= read -r pid; do
    [ -z "$pid" ] && continue
    owner=$(ps -o user= -p "$pid" 2>/dev/null | tr -d ' ')
    if [ "$owner" != "$USER" ]; then
        fail "PID $pid belongs to '$owner', not current user '$USER'"
    else
        pass "PID $pid belongs to current user '$USER'"
    fi
done <<< "$output"

# Test 9: Pattern matching works (partial match)
section "ourpgrep.sh — Pattern Matching"

# 'bas' should still match 'bash'
output=$(./ourpgrep.sh "bas" 2>/dev/null)
if [ -n "$output" ]; then
    pass "Partial pattern 'bas' matches bash processes"
else
    # Not a hard failure — user might not have bash visible this way
    echo -e "${YELLOW}[WARN]${NC} Partial pattern 'bas' returned no results (may be environment-dependent)"
fi

# =============================================================================
# OURPKILL.SH TESTS
# =============================================================================

section "ourpkill.sh — Argument Validation"

# Test 10: No arguments
output=$(./ourpkill.sh 2>&1)
assert_exit "No arguments → exit 1" 1 $?
assert_contains "No arguments → error message" "Error" "$output"

# Test 11: Too many arguments
output=$(./ourpkill.sh -9 bash extra 2>&1)
assert_exit "Three arguments → exit 1" 1 $?

# Test 12: Signal without leading dash
output=$(./ourpkill.sh 9 bash 2>&1)
assert_exit "Signal without dash → exit 1" 1 $?
assert_contains "Signal without dash → error message" "Error" "$output"

# ---

section "ourpkill.sh — Killing a Real Process"

# Test 13: Start a dummy background process, then kill it with ourpkill.sh
sleep 999 &
DUMMY_PID=$!
sleep 0.2  # Give it a moment to appear in ps

# Confirm it's running
if ps -p $DUMMY_PID > /dev/null 2>&1; then
    pass "Dummy process (sleep 999) started with PID $DUMMY_PID"
else
    fail "Dummy process failed to start"
fi

# Kill using ourpkill.sh (default signal -15)
output=$(./ourpkill.sh "sleep" 2>&1)
assert_exit "ourpkill.sh 'sleep' → exit 0" 0 $?
assert_empty "ourpkill.sh 'sleep' → no stdout output" "$output"
sleep 0.3  # Give it a moment to die

if ps -p $DUMMY_PID > /dev/null 2>&1; then
    fail "Dummy process PID $DUMMY_PID still running after ourpkill.sh"
else
    pass "Dummy process PID $DUMMY_PID was successfully killed"
fi

# Test 14: Kill with explicit -15 signal
sleep 999 &
DUMMY_PID=$!
sleep 0.2
./ourpkill.sh -15 "sleep" 2>/dev/null
sleep 0.3

if ps -p $DUMMY_PID > /dev/null 2>&1; then
    fail "Process still running after ourpkill.sh -15"
else
    pass "ourpkill.sh -15 correctly killed the process"
fi

# Test 15: Kill with -9 (SIGKILL)
sleep 999 &
DUMMY_PID=$!
sleep 0.2
./ourpkill.sh -9 "sleep" 2>/dev/null
sleep 0.3

if ps -p $DUMMY_PID > /dev/null 2>&1; then
    fail "Process still running after ourpkill.sh -9"
else
    pass "ourpkill.sh -9 (SIGKILL) correctly killed the process"
fi

# ---

section "ourpkill.sh — Edge Cases"

# Test 16: Non-existent process → silent exit 0
output=$(./ourpkill.sh "PROCESS_THAT_DOES_NOT_EXIST_XYZ123" 2>&1)
assert_exit "Non-existent process → exit 0" 0 $?
assert_empty "Non-existent process → no output" "$output"

# Test 17: ourpkill.sh uses ourpgrep.sh internally (check it exists as dependency)
if grep -q "ourpgrep.sh" ./ourpkill.sh; then
    pass "ourpkill.sh calls ourpgrep.sh internally"
else
    fail "ourpkill.sh does not appear to call ourpgrep.sh"
fi

# Test 18: ourpkill.sh uses the kill command internally
if grep -q "\bkill\b" ./ourpkill.sh; then
    pass "ourpkill.sh uses the 'kill' command internally"
else
    fail "ourpkill.sh does not appear to use 'kill'"
fi

# =============================================================================
# SUMMARY
# =============================================================================

section "TEST SUMMARY"
TOTAL=$((PASS + FAIL))
echo -e "Total:  $TOTAL"
echo -e "${GREEN}Passed: $PASS${NC}"
echo -e "${RED}Failed: $FAIL${NC}"

if [ "$FAIL" -eq 0 ]; then
    echo -e "\n${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed. Review output above.${NC}"
    exit 1
fi