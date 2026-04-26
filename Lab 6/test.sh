#!/bin/bash

# =============================================================================
# test_scripts.sh
# Test suite for ourpgrep.sh and ourpkill.sh
# =============================================================================

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; CYAN='\033[0;36m'; NC='\033[0m'
PASS=0; FAIL=0

pass() { echo -e "${GREEN}[PASS]${NC} $1"; ((PASS++)); }
fail() { echo -e "${RED}[FAIL]${NC} $1"; ((FAIL++)); }
section() { echo -e "\n${CYAN}=== $1 ===${NC}"; }

assert_exit() {
    local desc="$1" expected="$2" actual="$3"
    [ "$actual" -eq "$expected" ] && pass "$desc (exit code: $actual)" || fail "$desc (expected exit $expected, got $actual)"
}
assert_contains() {
    local desc="$1" expected="$2" actual="$3"
    echo "$actual" | grep -q "$expected" && pass "$desc" || fail "$desc (expected '$expected' in: '$actual')"
}
assert_empty() {
    local desc="$1" actual="$2"
    [ -z "$actual" ] && pass "$desc" || fail "$desc (expected empty, got: '$actual')"
}

# =============================================================================
section "PREREQUISITE CHECK"
[ ! -f "./ourpgrep.sh" ] && { echo -e "${RED}ERROR: ourpgrep.sh not found.${NC}"; exit 1; }
[ ! -f "./ourpkill.sh" ] && { echo -e "${RED}ERROR: ourpkill.sh not found.${NC}"; exit 1; }
chmod +x ./ourpgrep.sh ./ourpkill.sh
pass "Both scripts found and made executable"

# =============================================================================
section "ourpgrep.sh — Argument Validation"

output=$(./ourpgrep.sh 2>&1)
assert_exit "No arguments → exit 1" 1 $?
assert_contains "No arguments → error message on stderr" "Error" "$output"
assert_contains "No arguments → usage hint shown" "Usage" "$output"

output=$(./ourpgrep.sh bash extra 2>&1)
assert_exit "Too many arguments → exit 1" 1 $?
assert_contains "Too many arguments → error message" "Error" "$output"

# =============================================================================
section "ourpgrep.sh — Correct Behavior"

output=$(./ourpgrep.sh bash 2>/dev/null)
assert_exit "Searching 'bash' → exit 0" 0 $?
[ -n "$output" ] && pass "Searching 'bash' → non-empty output" || fail "Searching 'bash' → expected PIDs but got none"

non_numeric=$(echo "$output" | grep -vE '^[0-9]+$')
[ -z "$non_numeric" ] && pass "Output contains only numeric PIDs" || fail "Non-numeric lines in output: '$non_numeric'"

# Check no leaked grep process (grep -v grep should prevent this)
leaked_grep=0
while IFS= read -r pid; do
    [ -z "$pid" ] && continue
    cmd=$(ps -o comm= -p "$pid" 2>/dev/null)
    [ "$cmd" = "grep" ] && leaked_grep=1
done <<< "$output"
[ "$leaked_grep" -eq 0 ] && pass "No leaked grep process PIDs in output" || fail "grep process leaked into results"

output=$(./ourpgrep.sh "PROCESS_THAT_DOES_NOT_EXIST_XYZ123" 2>/dev/null)
assert_exit "Non-existent process → exit 0" 0 $?
assert_empty "Non-existent process → empty stdout" "$output"

# =============================================================================
section "ourpgrep.sh — User Filtering"

output=$(./ourpgrep.sh bash 2>/dev/null)
any_checked=0
while IFS= read -r pid; do
    [ -z "$pid" ] && continue
    # Guard against race condition: skip PIDs that died between listing and checking
    if ! ps -p "$pid" > /dev/null 2>&1; then
        echo -e "${YELLOW}[SKIP]${NC} PID $pid no longer exists (race condition)"
        continue
    fi
    any_checked=1
    owner=$(ps -o user= -p "$pid" 2>/dev/null | tr -d ' ')
    [ "$owner" = "$USER" ] && pass "PID $pid belongs to current user '$USER'" || fail "PID $pid belongs to '$owner', not '$USER'"
done <<< "$output"
[ "$any_checked" -eq 0 ] && echo -e "${YELLOW}[WARN]${NC} No stable PIDs to verify user filtering"

# =============================================================================
section "ourpgrep.sh — Pattern Matching"

output=$(./ourpgrep.sh "bas" 2>/dev/null)
[ -n "$output" ] && pass "Partial pattern 'bas' matches bash processes" \
    || echo -e "${YELLOW}[WARN]${NC} Partial pattern 'bas' returned no results (environment-dependent)"

# =============================================================================
section "ourpkill.sh — Argument Validation"

output=$(./ourpkill.sh 2>&1)
assert_exit "No arguments → exit 1" 1 $?
assert_contains "No arguments → error message" "Error" "$output"

output=$(./ourpkill.sh -9 bash extra 2>&1)
assert_exit "Three arguments → exit 1" 1 $?

output=$(./ourpkill.sh 9 bash 2>&1)
assert_exit "Signal without dash → exit 1" 1 $?
assert_contains "Signal without dash → error message" "Error" "$output"

# =============================================================================
section "ourpkill.sh — Killing a Real Process"

check_exit_143() {
    local desc="$1" code="$2"
    if [ "$code" -eq 0 ]; then
        pass "$desc → exit 0"
    elif [ "$code" -eq 143 ]; then
        fail "$desc → exit 143 (ourpkill.sh killed itself — add 'grep -v ourpkill' to ourpgrep.sh pipeline)"
    else
        fail "$desc → unexpected exit code $code"
    fi
}

# Test default signal (-15)
sleep 999 & DUMMY=$!; sleep 0.2
ps -p $DUMMY > /dev/null 2>&1 && pass "Dummy process started: PID $DUMMY" || fail "Dummy process failed to start"
output=$(./ourpkill.sh "sleep" 2>&1); CODE=$?; sleep 0.3
check_exit_143 "ourpkill.sh 'sleep'" $CODE
assert_empty "ourpkill.sh → no stdout output" "$output"
! ps -p $DUMMY > /dev/null 2>&1 && pass "PID $DUMMY killed successfully" || { kill -9 $DUMMY 2>/dev/null; fail "PID $DUMMY still alive"; }

# Test explicit -15
sleep 999 & DUMMY=$!; sleep 0.2
./ourpkill.sh -15 "sleep" 2>/dev/null; sleep 0.3
! ps -p $DUMMY > /dev/null 2>&1 && pass "ourpkill.sh -15 killed the process" || { kill -9 $DUMMY 2>/dev/null; fail "Still alive after -15"; }

# Test -9 (SIGKILL)
sleep 999 & DUMMY=$!; sleep 0.2
./ourpkill.sh -9 "sleep" 2>/dev/null; sleep 0.3
! ps -p $DUMMY > /dev/null 2>&1 && pass "ourpkill.sh -9 killed the process" || { kill -9 $DUMMY 2>/dev/null; fail "Still alive after -9"; }

# =============================================================================
section "ourpkill.sh — Edge Cases"

output=$(./ourpkill.sh "PROCESS_THAT_DOES_NOT_EXIST_XYZ123" 2>&1); CODE=$?
check_exit_143 "Non-existent process" $CODE
assert_empty "Non-existent process → no output" "$output"

grep -q "ourpgrep.sh" ./ourpkill.sh && pass "ourpkill.sh calls ourpgrep.sh internally" || fail "ourpkill.sh does not call ourpgrep.sh"
grep -qE '\bkill\b' ./ourpkill.sh && pass "ourpkill.sh uses the 'kill' command internally" || fail "ourpkill.sh does not use 'kill'"

# =============================================================================
section "TEST SUMMARY"
TOTAL=$((PASS + FAIL))
echo -e "Total:  $TOTAL"
echo -e "${GREEN}Passed: $PASS${NC}"
if [ "$FAIL" -gt 0 ]; then
    echo -e "${RED}Failed: $FAIL${NC}"
    echo ""
    echo -e "${YELLOW}TIP:${NC} If you see exit 143 errors, fix ourpgrep.sh by adding"
    echo -e "     ${CYAN}grep -v 'ourpkill'${NC} to the pipeline (prevents ourpkill.sh killing itself)."
    echo -e "\n${RED}Some tests failed.${NC}"; exit 1
else
    echo -e "${GREEN}Failed: 0${NC}"
    echo -e "\n${GREEN}All tests passed!${NC}"; exit 0
fi