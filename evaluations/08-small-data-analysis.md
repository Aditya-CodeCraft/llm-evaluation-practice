# Evaluation 08: Mean, median and outlier for eight days of sales

**Date:** (fill in when you write the evaluation)
**Domain:** data analysis (arithmetic)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

```text
Here are the daily unit sales from my shop over the last eight days. Can you give me the mean and the median, and tell me which day is the outlier and why? Please show the working so I can check it myself.

| Day | Units sold |
|---|---|
| Mon | 212 |
| Tue | 198 |
| Wed | 205 |
| Thu | 640 |
| Fri | 221 |
| Sat | 189 |
| Sun | 203 |
| Mon (this week) | 216 |
```

**System instruction or guideline the response had to follow:**

```text
None
```

## Response A

```text
Here is the breakdown with every step shown.

**Mean**
212 + 198 = 410
410 + 205 = 615
615 + 640 = 1,255
1,255 + 221 = 1,456
1,456 + 189 = 1,645
1,645 + 203 = 1,848
1,848 + 216 = 2,064
Mean = 2,064 ÷ 8 = **258.0 units per day**

**Median**
Sorted low to high: 189, 198, 203, 205, 212, 216, 221, 640
With eight values the median is the average of the 4th and 5th: (205 + 212) ÷ 2 = **208.5 units per day**

**Outlier: Thursday (640 units)**
Every other day falls between 189 and 221, a spread of only 32 units. Thursday is roughly three times any of those and sits more than 400 units above the median. It is also why the mean (258.0) lands well above the median (208.5): a single extreme value pulls the mean up while the median ignores it. When the two diverge this much in a small dataset, it almost always points to one unusual observation rather than a general trend.

If Thursday was not a promotion or a bulk order, it is worth double-checking the entry for that day.
```

## Response B

```text
Working through it step by step.

**Mean**
212 + 198 = 410
410 + 205 = 615
615 + 640 = 1,255
1,255 + 221 = 1,476
1,476 + 189 = 1,665
1,665 + 203 = 1,868
1,868 + 216 = 2,084
Mean = 2,084 ÷ 8 = **260.5 units per day**

**Median**
Sorted low to high: 189, 198, 203, 205, 212, 216, 221, 640
Eight values, so take the middle pair (4th and 5th): (205 + 212) ÷ 2 = **208.5 units per day**

**Outlier: Thursday (640 units)**
The other seven days all land between 189 and 221. Thursday is about three times any of them and over 400 units above the median. You can also see it in the gap between the mean (260.5) and the median (208.5): the mean is dragged upward by one large value while the median is unaffected. Excluding Thursday, the remaining seven days total 1,444, so they average 1,444 ÷ 7 ≈ 206.3, which sits right next to the median and confirms the rest of the period is a tight, consistent cluster.

Worth checking whether 640 was a genuine event (promotion, wholesale order) or a typo for something like 240.
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
