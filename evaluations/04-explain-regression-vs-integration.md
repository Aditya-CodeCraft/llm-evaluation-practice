# Evaluation 04: Regression testing versus integration testing

**Date:** (fill in when you write the evaluation)
**Domain:** software QA (explanation)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

```text
I just started as a junior tester on a small web team (Django backend, React frontend, Postgres). In standup today my lead said "add it to the regression suite, and make sure the integration tests cover the new endpoint" and I nodded but honestly I'm not sure I could explain the difference between regression testing and integration testing if someone asked me. Can you explain how they differ and give me one concrete example of each so I can picture it? Keep it practical, I don't need the textbook history.
```

**System instruction or guideline the response had to follow:**

```text
None
```

## Response A

```text
Good question, and it is one that trips up plenty of people with years of experience, so no need to feel behind. The two terms describe different axes of testing: one is about *what* you are testing (how parts fit together), the other is about *why* you are running tests (to catch things that used to work and silently broke).

**Integration testing** checks that separately built components actually cooperate when wired together. Unit tests can all pass while the pieces still disagree about data formats, authentication, or ordering. Integration tests exist to expose exactly those seams.

*Example:* your new Django endpoint `POST /api/orders` is tested with a real test database rather than mocks. The test sends a request through the URL router, the view, the serializer, and the ORM, then asserts that a row landed in the `orders` table and the JSON response carries the generated `order_id`. Any one of those layers could pass its own unit tests and still break the chain.

**Regression testing** is about protecting existing behaviour over time. Re-running your existing suite of previously passing tests after a change, to confirm nothing that used to work has broken, is called integration testing because it integrates the new change with everything already shipped. The suite grows as bugs are fixed: each fix gets a test so the bug cannot quietly return.

*Example:* last sprint someone fixed a bug where discount codes were applied twice at checkout. A test for that case now lives in the suite, and when you touch the pricing module next month it runs again automatically. If it fails, you know your change reintroduced the old bug.

So when your lead said "add it to the regression suite," she meant "make sure this behaviour is protected going forward," and "integration tests for the endpoint" meant "prove the endpoint works end to end with the real stack." They are complementary, not competing.
```

## Response B

```text
They answer two different questions, which is why your lead could mention both in one sentence without contradiction.

**Integration testing asks: do these pieces work together?** Unit tests check a function or class in isolation, often with mocks. Integration tests remove some of those mocks and exercise real connections between components, because the most common production failures live at the boundaries: a serializer that emits a field the frontend does not expect, a query that works in SQLite but not Postgres, a header that one service sets and another ignores.

*Example:* for your new endpoint, an integration test would spin up a test Postgres database, send a real HTTP request to `POST /api/orders` through the Django test client, and assert both that the response is `201` with a valid `order_id` and that the row exists in the `orders` table with the right values. No mocks between the view and the database.

**Regression testing asks: did this change break something that already worked?** It is not a type of test so much as a reason for running tests. The regression suite is the collection of tests you re-run after every change, usually in CI, specifically to catch unintended side effects. Any kind of test (unit, integration, end-to-end) can be part of it.

*Example:* suppose a bug where the cart total ignored shipping for orders over $100 was fixed two months ago and a test was added. When you later refactor the pricing code, that test runs again. If it goes red, your refactor reintroduced the old bug, and you find out before merge rather than from a customer.

Putting it together: "add it to the regression suite" means write tests for the new behaviour and make sure they run on every future change; "integration tests cover the new endpoint" means test the endpoint against the real stack, not just mocked pieces. One test can easily satisfy both requests.
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
