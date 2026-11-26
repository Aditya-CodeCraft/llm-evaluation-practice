# LLM Evaluation Practice

Worked examples of evaluating large-language-model outputs, written by Aditya Gupta (QA engineer, Kolkata).

Each evaluation compares two model responses to the same prompt, scores them against a fixed rubric, states a preference and explains the reasoning in a few lines. This is the format human-feedback platforms use for response rating and LLM quality analysis, and the same discipline as writing a good defect report: evidence first, then a verdict.

**Status:** 10 paired evaluations, 2 code reviews (Python, JavaScript) and 4 sanitizer-reproduced C++ audits complete.

## Layout

| Path | What it holds |
|---|---|
| `rubric.md` | The criteria and 1 to 5 scale used in every evaluation |
| `evaluations/` | One Markdown file per evaluation, numbered `01-`, `02-`, ... (start from `evaluations/TEMPLATE.md`) |
| `code-reviews/` | Written reviews of AI-generated code (start from `code-reviews/TEMPLATE.md`) |
| `cpp-audits/` | AI-generated C++ audited for out-of-bounds access, leaks, data races and undefined behavior, each bug reproduced under ASan, UBSan or TSan (see `cpp-audits/README.md`) |

## How each evaluation is written

1. **Prompt**, verbatim, plus any system instruction or guideline the response had to follow.
2. **Response A** and **Response B**, verbatim, with the model named if known.
3. **Scores** per rubric criterion, 1 to 5, each with a one-line justification.
4. **Verdict**: which response is preferred, and how confident the preference is.
5. **Rationale**: three to five lines naming the specific error, omission or guideline breach that decided it.
6. **What a better response would have done.**

## Rules followed in every file

- Judge the response, not the model name.
- Check facts before scoring accuracy, and say what was checked and how.
- Penalise a confident wrong answer more than honest uncertainty.
- Guideline adherence outranks style.
- Nothing here is copied from paid platform tasks. Prompts are written by me or taken from public sources, and responses are generated for this repository.

## Index

| # | Topic | Verdict | File |
|---|---|---|---|
| 01 | Python helper that returns the last N log lines | B strongly preferred | `evaluations/01-python-log-tail.md` |
| 02 | SQL query for frequent customers in 2025 | A strongly preferred | `evaluations/02-sql-frequent-customers.md` |
| 03 | Bug report from a mobile checkout complaint | B strongly preferred | `evaluations/03-bug-report-from-complaint.md` |
| 04 | Regression testing versus integration testing | A strongly preferred | `evaluations/04-explain-regression-vs-integration.md` |
| 05 | HTTP status codes for four API situations | B strongly preferred | `evaluations/05-http-status-codes.md` |
| 06 | Three-bullet summary of a product update | B strongly preferred | `evaluations/06-three-bullet-summary.md` |
| 07 | Node function with await inside a loop | A strongly preferred | `evaluations/07-javascript-async.md` |
| 08 | Mean, median and outlier for eight days of sales | B strongly preferred | `evaluations/08-small-data-analysis.md` |
| 09 | Request to get around a website rate limit | A strongly preferred | `evaluations/09-borderline-safety-request.md` |
| 10 | Client email about a one-week release slip | A strongly preferred | `evaluations/10-client-delay-email.md` |

## Code reviews

| # | Topic | Verdict | File |
|---|---|---|---|
| 01 | Deduplicating a contact list in Python | Reject | `code-reviews/cr-01-python-dedupe-contacts.md` |
| 02 | Parsing duration strings in JavaScript | Reject | `code-reviews/cr-02-js-parse-duration.md` |

## C++ audits

Each bug is reproduced under a sanitizer, and CI re-runs every reproduction and fix with GCC and Clang (`sh cpp-audits/run.sh`).

| # | Bug class | Sanitizer | Verdict | Folder |
|---|---|---|---|---|
| 01 | Out-of-bounds write in a prefix-sum array | ASan | Reject | `cpp-audits/01-prefix-sum-out-of-bounds/` |
| 02 | Memory leak on the error path of a parser | ASan / LeakSanitizer | Reject | `cpp-audits/02-parse-numbers-leak/` |
| 03 | Data race in a multithreaded sum | TSan | Reject | `cpp-audits/03-parallel-sum-data-race/` |
| 04 | Signed-overflow UB in a binary-search midpoint | UBSan | Reject | `cpp-audits/04-binary-search-overflow/` |
