# Evaluation 05: HTTP status codes for four API situations

**Date:** (fill in when you write the evaluation)
**Domain:** factual (web)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

```text
I'm building a small REST API in Flask for a bookmarks app and my code review got flagged for "inconsistent status codes". Right now I just return 200 for almost everything and 500 when anything goes wrong. Can you tell me which HTTP status code I should return in each of these four cases, with a short reason for each so I can explain it in the PR?

1. POST /bookmarks succeeds and a new bookmark row is created.
2. POST /bookmarks receives a JSON body where "url" is missing or isn't a valid URL.
3. A logged-in user (valid session token) tries to DELETE /bookmarks/42, but bookmark 42 belongs to a different user.
4. A client has sent more than 100 requests in a minute and we want to stop serving them until the window resets.

Just the four codes and one line each is fine.
```

**System instruction or guideline the response had to follow:**

```text
None
```

## Response A

```text
Here are the four codes I'd use:

1. **201 Created** — the request succeeded and produced a new resource; return the new bookmark in the body and ideally a `Location` header pointing at `/bookmarks/<id>`.

2. **400 Bad Request** (or **422 Unprocessable Content**) — the request is well-formed JSON but the content fails validation; 400 is the conventional choice, 422 is fine if your team wants to distinguish "unparseable" from "parsed but invalid".

3. **401 Unauthorized** — the user's credentials don't authorize this action on bookmark 42, so the correct signal is 401; 403 is reserved for cases where no credentials were sent at all, which isn't the situation here since the session token is valid.

4. **429 Too Many Requests** — the client exceeded the rate limit; include a `Retry-After` header so well-behaved clients know when to try again.

For the PR description you can summarise it as: 2xx means it worked, 4xx means the client needs to change something, and you only use 5xx when the server itself failed. That alone should satisfy the "inconsistent status codes" comment.
```

## Response B

```text
Here are the four codes I'd use:

1. **201 Created** — the request succeeded and a new resource now exists; return the created bookmark in the body and set a `Location` header to `/bookmarks/<id>`.

2. **400 Bad Request** or **422 Unprocessable Content** — the body was received but fails validation; 400 is the most widely used choice, while 422 is a reasonable option if you want to separate "couldn't parse" from "parsed but invalid".

3. **403 Forbidden** — the server knows who the user is (the token is valid) but they aren't permitted to act on bookmark 42; 401 would be wrong here because 401 means authentication is missing or invalid, not that a known user lacks permission.

4. **429 Too Many Requests** — the client exceeded the limit; add a `Retry-After` header so clients can back off correctly.

For the PR you can frame it as: 2xx means success, 4xx means the client sent something it needs to change, and 5xx is only for genuine server faults. Replacing the blanket 200/500 with these four will address the reviewer's comment.
```

## Fact checks performed

- (What you checked, how, and the result. Run the code, compute by hand, count words, read the documentation.)

## Scores

| Criterion | A | B | Justification |
|---|---|---|---|
| Accuracy | | | |
| Relevance | | | |
| Completeness | | | |
| Clarity | | | |
| Adherence to guidelines | | | |
| Safety | | | |
| **Total (out of 30)** | | | |

## Verdict

(A strongly preferred / A preferred / Tie / B preferred / B strongly preferred). Confidence: (high / medium / low).

## Rationale

Three to five lines. Name the deciding issue with its severity tag, for example `[critical]`, and quote the exact phrase or line that contains it.

## What a better response would have done

One to three lines.
