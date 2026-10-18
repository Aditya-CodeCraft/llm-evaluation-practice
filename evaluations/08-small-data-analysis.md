# Evaluation 08: Mean, median and outlier for eight days of sales

**Date:** 2026-10-18
**Domain:** data analysis (arithmetic)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

````text
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
````

**System instruction or guideline the response had to follow:**

````text
None
````

## Response A

````text
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
````

## Response B

````text
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
````

## Fact checks performed

- Re-ran the running total in Python step by step on `[212, 198, 205, 640, 221, 189, 203, 216]`: 212, 410, 615, 1,255, 1,476, 1,665, 1,868, 2,084. Total 2,084, mean 2,084 ÷ 8 = 260.5. B matches every step; A does not.
- Checked A's fifth line on its own: 1,255 + 221 = 1,476, not "1,456". Every later line in A carries the 20-unit shortfall, so its total of 2,064 and "258.0 units per day" are both wrong (2,064 ÷ 8 is 258.0, so the division itself is fine).
- Sorted the values and averaged the 4th and 5th: 189, 198, 203, 205, 212, 216, 221, 640 gives (205 + 212) ÷ 2 = 208.5. `statistics.median` agrees. Both responses are correct here.
- Checked B's mean excluding Thursday: 2,084 − 640 = 1,444, and 1,444 ÷ 7 = 206.29, so "≈ 206.3" is right.
- Checked the outlier claims: the other seven days span 189 to 221, a range of 32 (A's "spread of only 32 units" is right). 640 is 431.5 above the median, so A's "more than 400 units above the median" and B's "over 400 units above the median" both hold. 640 is 2.9 times 221 and 3.4 times 189, so "roughly three times" is fair.
- Applied a standard IQR test as a sanity check: Q1 = 199.25, Q3 = 219.75, upper fence 250.5. Only Thursday's 640 is above it, so Thursday is the only outlier. Neither response names a formal rule like this.

## Scores

| Criterion | A | B | Justification |
|---|---|---|---|
| Accuracy | 2 | 5 | A's "1,255 + 221 = 1,456" puts the requested mean at 258.0 instead of 260.5; every figure in B checks out. |
| Relevance | 5 | 5 | Both answer exactly the three things asked, with working, and nothing off topic. |
| Completeness | 3 | 4 | A covers all parts but one of the three requested figures is wrong; B adds the excluding-Thursday mean, though neither names a formal outlier rule. |
| Clarity | 5 | 5 | Both lay out the running total, the sorted list and the outlier reasoning in a form that is easy to check line by line. |
| Adherence to guidelines | 5 | 5 | No system instruction; both show the working the user asked for. |
| Safety | 5 | 5 | Nothing to flag in either. |
| **Total (out of 30)** | **25** | **29** | |

## Verdict

B strongly preferred. Confidence: high.

## Rationale

`[major]` Response A slips in the running total at "1,255 + 221 = 1,456" (should be 1,476), so it reports "Mean = 2,064 ÷ 8 = **258.0 units per day**" instead of 260.5, and then reuses the wrong figure in "the mean (258.0) lands well above the median".
The user asked for the mean specifically and wanted working they could check, so a wrong bolded headline figure is a direct failure on a requested deliverable.
I have kept it at major rather than critical because A's median (208.5) and its outlier call (Thursday, 640) are both correct and the conclusion still holds.
B gets all three figures right and strengthens the outlier case with "1,444 ÷ 7 ≈ 206.3", which sits next to the median. A's "spread of only 32 units" is a nice touch B lacks, but it does not offset the arithmetic error.

## What a better response would have done

Get the running total right (2,084, mean 260.5), median 208.5, and flag Thursday using a stated rule, for example the IQR fence (Q3 + 1.5 × IQR = 250.5), alongside the mean without Thursday (206.3).
Then suggest checking whether 640 was a genuine event or an entry error, as both responses already do.
