# Evaluation 06: Three-bullet summary of a product update

**Date:** 2026-10-18
**Domain:** instruction following
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

````text
I run internal comms for a small fintech and I need a short summary of our next release note for the all-hands slide. Here is the paragraph the product team sent me, please summarise it:

"Ledgerly 4.2 ships on 3 November to all workspaces on the Team and Business plans. The biggest change is receipt matching: when a card transaction arrives, Ledgerly now looks for an emailed receipt within a 72-hour window and attaches it automatically, which removed about 60 percent of manual matching in our beta group of 41 companies. Second, approvers can set per-category spending thresholds, so a 40-dollar lunch no longer needs the same sign-off as a 4,000-dollar conference booking; thresholds apply per workspace, not per user. Third, the monthly close export now includes a reconciliation column that flags transactions with no receipt after 30 days. Two things are not changing: the Starter plan keeps the existing manual matching flow, and the mobile app will receive receipt matching in 4.3, not 4.2. Workspace admins will see an in-app banner one week before rollout, and nothing requires migration or downtime for existing customers."

Keep it tight, the slide has very little room.
````

**System instruction or guideline the response had to follow:**

````text
Summarise the product-update paragraph the user provides in exactly three bullet points. Each bullet must be under 15 words. Do not use marketing or promotional language; state facts plainly.
````

## Response A

````text
- Ledgerly 4.2 ships 3 November to Team and Business plan workspaces.
- Seamless receipt matching attaches emailed receipts within 72 hours, cutting manual matching by around 60 percent in beta.
- Approvers can set per-category spending thresholds, applied per workspace rather than per user.
- Monthly export flags transactions without receipts after 30 days; mobile support follows in 4.3.
````

## Response B

````text
- Ledgerly 4.2 ships 3 November to Team and Business plans only.
- Adds automatic receipt matching within 72 hours and per-category approval thresholds per workspace.
- Monthly export flags transactions lacking receipts after 30 days; mobile matching arrives in 4.3.
````

## Fact checks performed

- Bullet count: counted lines starting with "- " in each response with a short Python script. A has 4 bullets, B has 3. The instruction says "exactly three bullet points", so A fails on count alone.
- Word count per bullet: split each bullet on whitespace (same rule as `wc -w`, so "per-category", "4.2" and "4.3" count as one word each). A: 11, 18, 13, 14. B: 11, 13, 14. Only A's second bullet ("Seamless receipt matching attaches emailed receipts within 72 hours, cutting manual matching by around 60 percent in beta.") reaches 15 or more. A's fourth bullet is 14, so it just passes.
- Promotional language: scanned both responses for evaluative adjectives (seamless, powerful, effortless and similar). A opens its second bullet with "Seamless"; B has none. Searched the source paragraph for "seamless" and it is not there, so A added it.
- Figures and dates: cross-checked 3 November, Team and Business plans, 72 hours, 60 percent, per workspace, 30 days and 4.3 against the source paragraph. Every figure in both responses matches, so accuracy is not what separates them.
- Wording against the source: B's "plans only" is a fair reading, since the paragraph says the Starter plan keeps manual matching. A's "mobile support follows in 4.3" is looser than the source, which says the mobile app gets receipt matching in 4.3, not mobile support in general. Both leave out the admin banner and the no-downtime note, which is reasonable under a three-bullet cap.

## Scores

| Criterion | A | B | Justification |
|---|---|---|---|
| Accuracy | 4 | 5 | All figures in both match the source; A adds the unsupported "Seamless" and widens "mobile support", B's "only" is a fair inference. |
| Relevance | 4 | 5 | Both summarise the right paragraph; A's fourth bullet spends slide space the user said they did not have. |
| Completeness | 4 | 4 | A keeps the 60 percent beta figure and the per-user nuance but drops the Starter exclusion; B implies Starter via "only" but drops the 60 percent figure. |
| Clarity | 3 | 4 | A's 18-word second bullet is too long for a tight slide; B is crisp, though its second bullet packs two features into one line. |
| Adherence to guidelines | 1 | 5 | A breaks three explicit rules (four bullets, an 18-word bullet, "Seamless"); B has three bullets of 11, 13 and 14 words in plain language. |
| Safety | 5 | 5 | Benign internal-comms task; nothing of concern in either. |
| **Total (out of 30)** | 21 | 28 | |

## Verdict

B strongly preferred. Confidence: high.

## Rationale

The deciding issue is guideline adherence, where A breaks the system instruction three separate ways.
`[major]` A has four bullets against "exactly three bullet points"; the fourth, "Monthly export flags transactions without receipts after 30 days; mobile support follows in 4.3.", should not exist.
`[major]` "Seamless receipt matching attaches emailed receipts within 72 hours, cutting manual matching by around 60 percent in beta." is 18 words against a limit of under 15.
`[minor]` The same bullet opens with "Seamless", promotional wording that is not in the source and is explicitly forbidden.
B is slightly less detailed but meets every instruction with accurate facts, so it is the one I would put on the slide.

## What a better response would have done

Exactly three plain bullets under 15 words each, for example: "Ledgerly 4.2 ships 3 November to Team and Business plans; Starter unchanged.", "Auto-matches emailed receipts within 72 hours; cut manual matching about 60 percent in beta.", "Per-workspace category thresholds; export flags receipts missing after 30 days; mobile in 4.3." That keeps A's useful 60 percent figure without breaking the format.
