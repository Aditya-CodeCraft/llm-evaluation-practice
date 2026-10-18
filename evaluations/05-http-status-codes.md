# Evaluation 05: HTTP status codes for four API situations

**Date:** 2026-10-18
**Domain:** factual (web)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

````text
I'm building a small REST API in Flask for a bookmarks app and my code review got flagged for "inconsistent status codes". Right now I just return 200 for almost everything and 500 when anything goes wrong. Can you tell me which HTTP status code I should return in each of these four cases, with a short reason for each so I can explain it in the PR?

1. POST /bookmarks succeeds and a new bookmark row is created.
2. POST /bookmarks receives a JSON body where "url" is missing or isn't a valid URL.
3. A logged-in user (valid session token) tries to DELETE /bookmarks/42, but bookmark 42 belongs to a different user.
4. A client has sent more than 100 requests in a minute and we want to stop serving them until the window resets.

Just the four codes and one line each is fine.
````

**System instruction or guideline the response had to follow:**

````text
None
````

## Response A

````text
Here are the four codes I'd use:

1. **201 Created** — the request succeeded and produced a new resource; return the new bookmark in the body and ideally a `Location` header pointing at `/bookmarks/<id>`.

2. **400 Bad Request** (or **422 Unprocessable Content**) — the request is well-formed JSON but the content fails validation; 400 is the conventional choice, 422 is fine if your team wants to distinguish "unparseable" from "parsed but invalid".

3. **401 Unauthorized** — the user's credentials don't authorize this action on bookmark 42, so the correct signal is 401; 403 is reserved for cases where no credentials were sent at all, which isn't the situation here since the session token is valid.

4. **429 Too Many Requests** — the client exceeded the rate limit; include a `Retry-After` header so well-behaved clients know when to try again.

For the PR description you can summarise it as: 2xx means it worked, 4xx means the client needs to change something, and you only use 5xx when the server itself failed. That alone should satisfy the "inconsistent status codes" comment.
````

## Response B

````text
Here are the four codes I'd use:

1. **201 Created** — the request succeeded and a new resource now exists; return the created bookmark in the body and set a `Location` header to `/bookmarks/<id>`.

2. **400 Bad Request** or **422 Unprocessable Content** — the body was received but fails validation; 400 is the most widely used choice, while 422 is a reasonable option if you want to separate "couldn't parse" from "parsed but invalid".

3. **403 Forbidden** — the server knows who the user is (the token is valid) but they aren't permitted to act on bookmark 42; 401 would be wrong here because 401 means authentication is missing or invalid, not that a known user lacks permission.

4. **429 Too Many Requests** — the client exceeded the limit; add a `Retry-After` header so clients can back off correctly.

For the PR you can frame it as: 2xx means success, 4xx means the client sent something it needs to change, and 5xx is only for genuine server faults. Replacing the blanket 200/500 with these four will address the reviewer's comment.
````

## Fact checks performed

- Read RFC 9110 section 15.5.2 (401 Unauthorized) from the rfc-editor.org text. It says 401 means the request "lacks valid authentication credentials for the target resource" and that the server "MUST send a WWW-Authenticate header field". That is an authentication failure, not the situation in case 3, where the token is valid.
- Read RFC 9110 section 15.5.4 (403 Forbidden). It says "If authentication credentials were provided in the request, the server considers them insufficient to grant access." This directly contradicts Response A's claim that "403 is reserved for cases where no credentials were sent at all". Case 3 is a textbook 403, so Response B is right and Response A is wrong.
- The same section adds that a server wishing to "hide" the existence of a forbidden resource "MAY instead respond with a status code of 404 (Not Found)". Neither response mentions this option for another user's bookmark.
- Confirmed the other three cases against the RFCs: 201 Created (RFC 9110 15.3.2, new resource identified by a Location header), 400 Bad Request (15.5.1) and 422 Unprocessable Content (15.5.21, syntax correct but content cannot be processed), and 429 Too Many Requests (RFC 6585 section 4, "MAY include a Retry-After header"). Both responses are correct on cases 1, 2 and 4, including the Location and Retry-After advice.
- Ran Python 3.12's `http.HTTPStatus` for 201, 400, 401, 403, 422 and 429 as a quick cross-check. The 403 description ("Request forbidden -- authorization will not help") supports B. The 401 description ("No permission -- see authorization schemes") is loosely worded and does not settle the question, and 422 has no description, so the RFC remains the deciding source. Python 3.12 labels 422 "Unprocessable Entity" (renamed "Unprocessable Content" in Python 3.13), and both responses correctly use the RFC 9110 name "Unprocessable Content".
- Checked the format request ("Just the four codes and one line each is fine"). Both responses give two codes for case 2 and add a closing paragraph about 2xx/4xx/5xx that was not asked for, so both go slightly beyond the requested format.

## Scores

| Criterion | A | B | Justification |
|---|---|---|---|
| Accuracy | 2 | 5 | A gets case 3 wrong and inverts the 401/403 definitions in its reason; B's four codes and reasons all match RFC 9110 and RFC 6585. |
| Relevance | 4 | 4 | Both answer all four cases, but both add a closing summary paragraph the user did not ask for. |
| Completeness | 3 | 4 | A covers all four cases but one of the four required answers is wrong; B is correct throughout but does not mention 404 as an option for hiding another user's bookmark. |
| Clarity | 5 | 5 | Both are well structured, numbered to match the prompt and easy to paste into a PR. |
| Adherence to guidelines | 4 | 4 | No system instruction; both stretch "Just the four codes and one line each is fine" with a dual code for case 2 and an extra closing paragraph. |
| Safety | 5 | 5 | Benign request handled appropriately by both. |
| **Total (out of 30)** | 23 | 27 | |

## Verdict

B strongly preferred. Confidence: high.

## Rationale

[critical] Response A, case 3, answers **401 Unauthorized** and says "the user's credentials don't authorize this action on bookmark 42, so the correct signal is 401; 403 is reserved for cases where no credentials were sent at all". This is backwards. RFC 9110 defines 401 as missing or invalid authentication and 403 as refusal when the credentials sent are "insufficient to grant access", which is exactly a valid user touching someone else's bookmark.
The user asked for four facts to justify in a PR, and A would have them write the wrong code with a confidently wrong explanation, which defeats the purpose of fixing "inconsistent status codes".
Response B gives 403 with the correct distinction ("401 means authentication is missing or invalid, not that a known user lacks permission"). Cases 1, 2 and 4 are effectively identical in both responses, so case 3 decides it.
[minor] Both add a closing paragraph and offer two codes for case 2 despite "Just the four codes and one line each is fine".

## What a better response would have done

Given B's four codes as single lines without the closing paragraph, and noted for case 3 that 404 is a legitimate alternative to 403 if the API should not reveal that bookmark 42 exists.
