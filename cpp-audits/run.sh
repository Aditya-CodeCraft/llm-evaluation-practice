#!/bin/sh
# Builds every audit twice (buggy.cpp, fixed.cpp) under the sanitizer named in
# its SANITIZER file and checks that:
#   * buggy.cpp produces a sanitizer report (the bug reproduces), and
#   * fixed.cpp runs clean and its self-checks pass.
# Usage: cpp-audits/run.sh [g++|clang++]   (default: clang++)
# Exit status is non-zero if any expectation fails.
set -u
CXX=${1:-clang++}
ROOT=$(cd "$(dirname "$0")" && pwd)
OUT=${OUT:-/tmp/cpp-audits}
mkdir -p "$OUT"
export ASAN_OPTIONS=detect_leaks=1:abort_on_error=0
export UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1
export TSAN_OPTIONS=halt_on_error=1
status=0

for dir in "$ROOT"/[0-9]*/; do
    name=$(basename "$dir")
    san=$(cat "$dir/SANITIZER")
    for variant in buggy fixed; do
        bin="$OUT/$name-$variant"
        if ! $CXX -std=c++17 -g -O1 -fno-omit-frame-pointer -fsanitize="$san" -pthread \
                "$dir/$variant.cpp" -o "$bin"; then
            echo "FAIL  $name/$variant: does not compile"; status=1; continue
        fi
        "$bin" > "$bin.log" 2>&1
        rc=$?
        if grep -qE 'ERROR: (Address|Leak)Sanitizer|WARNING: ThreadSanitizer|runtime error:' "$bin.log"; then
            reported=yes
        else
            reported=no
        fi
        if [ "$variant" = buggy ] && [ "$reported" = yes ]; then
            echo "ok    $name/buggy: $san sanitizer reports: $(grep -m1 -oE '(ERROR: [A-Za-z]+Sanitizer: [a-z-]+|WARNING: ThreadSanitizer: [a-z ]+|runtime error: [^0-9]*)' "$bin.log")"
        elif [ "$variant" = fixed ] && [ "$reported" = no ] && [ $rc -eq 0 ]; then
            echo "ok    $name/fixed: clean, self-checks pass"
        else
            echo "FAIL  $name/$variant: sanitizer report=$reported exit=$rc (log: $bin.log)"; status=1
        fi
    done
done
exit $status
