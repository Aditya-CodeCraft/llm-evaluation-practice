# Evaluation 03: Bug report from a mobile checkout complaint

**Date:** 2026-10-18
**Domain:** software QA
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

````text
hey, our checkout button is acting up on mobile again. a customer emailed support last night saying she tapped "Place order" and nothing happened, tapped it a few more times, still nothing, so she gave up. she just said "on my phone", didn't say what kind. her cart had 3 items, somewhere around $84. this was yesterday evening. can you write this up as a proper bug report so I can hand it to the dev team?
````

**System instruction or guideline the response had to follow:**

````text
You are a QA triage assistant. Convert the user's complaint into a structured bug report with exactly these sections: Title, Steps to Reproduce, Expected Result, Actual Result, Severity. Include only facts the user actually stated; anything inferred must be labelled as an assumption. If information needed to reproduce the issue is missing (device, OS, browser or app version), ask for it explicitly. Always justify the severity you assign.
````

## Response A

````text
**Title:** "Place order" button unresponsive on mobile checkout (iOS Safari)

**Steps to Reproduce:**
1. Open the store on an iPhone in Safari.
2. Add 3 items to the cart (total approx. $84).
3. Go to checkout and enter a shipping address.
4. Select Apple Pay as the payment method.
5. Tap "Place order".
6. Tap the button two or three more times.

**Expected Result:** The order is submitted, a confirmation page is shown and a confirmation email is sent.

**Actual Result:** Nothing visible happens on any tap: no spinner, no error message, no navigation. The customer abandoned the purchase.

**Root Cause:** The button's tap handler is almost certainly being swallowed because the Apple Pay sheet fails to initialise, which leaves the button logically disabled while it still looks enabled. This matches the symptom of repeated taps doing nothing.

**Severity:** Critical

**Notes:** Occurred yesterday evening. Recommend the payments team hotfix the Apple Pay initialisation path and add a Safari-specific regression test to the checkout suite before the next release.
````

## Response B

````text
**Title:** "Place order" button appears unresponsive on mobile checkout (device and version unknown)

**Steps to Reproduce** (as reported by the customer, not yet verified internally):
1. On a mobile phone, add 3 items to the cart (total approx. $84).
2. Proceed to checkout.
3. Tap "Place order".
4. Tap "Place order" several more times.

**Expected Result:** A single tap submits the order and the customer sees an order confirmation.

**Actual Result:** Per the customer, nothing happened on any tap and she abandoned the purchase. Reported as occurring yesterday evening.

**Known facts:** mobile device, 3 items, roughly $84 cart, repeated taps with no visible response, one customer report via support email.

**Assumptions / unknowns:** We do not know the device, OS, whether she used the app or a mobile browser, the app or browser version, the payment method, whether a server-side error was logged, or whether other customers are affected.

**Severity:** Major (provisional). It blocks purchase completion, which would normally be Critical, but it is a single unreproduced report; upgrade to Critical if it reproduces or further reports arrive.

**To finalise this report I need:** her device model and OS version, app vs. browser (and which browser), the app or browser version, the approximate time, and the payment method she selected.
````

## Fact checks performed

- Searched the prompt text (case-insensitive grep) for "iPhone", "Safari", "iOS", "Apple Pay", "shipping", "spinner" and "error message". Result: zero matches for every term. Response A uses "Safari" 3 times, "Apple Pay" 3 times and each of the others once; Response B uses none of them.
- Checked the prompt for any device or version detail. Result: the only device statements are "on my phone" and "didn't say what kind". A contains no question mark and no request for information; B ends with an explicit five-item list under "To finalise this report I need".
- Read the text after "Severity:" in each response. Result: A gives the single word "Critical" with no reasoning; B gives "Major (provisional)", a justification sentence and an upgrade condition ("upgrade to Critical if it reproduces or further reports arrive").
- Listed the bold section headers in each response against the five required. Result: both contain all five. A has 7 headers, adding "Root Cause" and "Notes". B has 8, adding "Known facts", "Assumptions / unknowns" and "To finalise this report I need". Both breach "exactly these sections" strictly, but B's extras carry the labelled assumptions and the information request the instruction asks for, while A's "Root Cause" is speculation.
- Checked the Actual and Expected Results against the complaint. Result: A's "no spinner, no error message, no navigation" and "a confirmation email is sent" are not in the complaint and are not labelled as assumptions. B's "sees an order confirmation" is also inferred, but its Actual Result is attributed "Per the customer".
- Checked the timing details. Result: the prompt gives "last night" and "yesterday evening"; B still asks for "the approximate time", which is slightly redundant (at best a request for a more precise time).

## Scores

| Criterion | A | B | Justification |
|---|---|---|---|
| Accuracy | 1 | 5 | A invents "iPhone in Safari", "Apple Pay" and a root cause stated as "almost certainly"; B sticks to the reported facts and flags every unknown. |
| Relevance | 3 | 5 | A pads the report with a speculative "Root Cause" and a hotfix recommendation; everything in B serves the bug report. |
| Completeness | 2 | 5 | A never asks for device, OS or version and gives no severity reasoning; B covers facts, unknowns, a justified severity and a concrete information request. |
| Clarity | 4 | 4 | A is tidy but presents guesses as fact; B is clear but repeats facts across "Known facts" and the steps, and asks for "the approximate time" already given. |
| Adherence to guidelines | 1 | 4 | A breaks facts-only, ask-for-missing-info and justify-severity; B follows all three but adds headers beyond "exactly these sections". |
| Safety | 3 | 5 | A's confident Apple Pay diagnosis could send developers down the wrong path; B is appropriately cautious. |
| **Total (out of 30)** | 14 | 28 | |

## Verdict

B strongly preferred. Confidence: high.

## Rationale

- `[critical]` A invents reproduction details the customer never gave: "Open the store on an iPhone in Safari", "Select Apple Pay as the payment method" and the title's "(iOS Safari)", when the complaint only says "on my phone".
- `[critical]` A builds a root cause on that invention, "almost certainly being swallowed because the Apple Pay sheet fails to initialise", which could misdirect the dev team.
- `[major]` A gives "Severity: Critical" with no justification and never asks for device, OS or version, breaking two explicit instructions.
- `[minor]` B adds three headers beyond the five required and asks for "the approximate time" already given, but it labels assumptions, justifies a provisional severity and asks for exactly what is missing.

## What a better response would have done

Kept to the five required sections, stated only what the customer reported, put inferred details (expected confirmation, any guessed platform) under clearly marked assumptions, justified the severity, and closed with a short request for device model, OS, app or browser and version, and payment method.
