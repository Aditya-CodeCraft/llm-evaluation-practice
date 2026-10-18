# Evaluation 06: Three-bullet summary of a product update

**Date:** (fill in when you write the evaluation)
**Domain:** instruction following
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

```text
I run internal comms for a small fintech and I need a short summary of our next release note for the all-hands slide. Here is the paragraph the product team sent me, please summarise it:

"Ledgerly 4.2 ships on 3 November to all workspaces on the Team and Business plans. The biggest change is receipt matching: when a card transaction arrives, Ledgerly now looks for an emailed receipt within a 72-hour window and attaches it automatically, which removed about 60 percent of manual matching in our beta group of 41 companies. Second, approvers can set per-category spending thresholds, so a 40-dollar lunch no longer needs the same sign-off as a 4,000-dollar conference booking; thresholds apply per workspace, not per user. Third, the monthly close export now includes a reconciliation column that flags transactions with no receipt after 30 days. Two things are not changing: the Starter plan keeps the existing manual matching flow, and the mobile app will receive receipt matching in 4.3, not 4.2. Workspace admins will see an in-app banner one week before rollout, and nothing requires migration or downtime for existing customers."

Keep it tight, the slide has very little room.
```

**System instruction or guideline the response had to follow:**

```text
Summarise the product-update paragraph the user provides in exactly three bullet points. Each bullet must be under 15 words. Do not use marketing or promotional language; state facts plainly.
```

## Response A

```text
- Ledgerly 4.2 ships 3 November to Team and Business plan workspaces.
- Seamless receipt matching attaches emailed receipts within 72 hours, cutting manual matching by around 60 percent in beta.
- Approvers can set per-category spending thresholds, applied per workspace rather than per user.
- Monthly export flags transactions without receipts after 30 days; mobile support follows in 4.3.
```

## Response B

```text
- Ledgerly 4.2 ships 3 November to Team and Business plans only.
- Adds automatic receipt matching within 72 hours and per-category approval thresholds per workspace.
- Monthly export flags transactions lacking receipts after 30 days; mobile matching arrives in 4.3.
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
