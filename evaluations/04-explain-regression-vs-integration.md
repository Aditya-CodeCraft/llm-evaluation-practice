# Evaluation 04: Regression testing versus integration testing

**Date:** 2026-10-18
**Domain:** software QA (explanation)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

````text
I just started as a junior tester on a small web team (Django backend, React frontend, Postgres). In standup today my lead said "add it to the regression suite, and make sure the integration tests cover the new endpoint" and I nodded but honestly I'm not sure I could explain the difference between regression testing and integration testing if someone asked me. Can you explain how they differ and give me one concrete example of each so I can picture it? Keep it practical, I don't need the textbook history.
````

**System instruction or guideline the response had to follow:**

````text
None
````

## Response A

````text
They answer two different questions, which is why your lead could mention both in one sentence without contradiction.

**Integration testing asks: do these pieces work together?** Unit tests check a function or class in isolation, often with mocks. Integration tests remove some of those mocks and exercise real connections between components, because the most common production failures live at the boundaries: a serializer that emits a field the frontend does not expect, a query that works in SQLite but not Postgres, a header that one service sets and another ignores.

*Example:* for your new endpoint, an integration test would spin up a test Postgres database, send a real HTTP request to `POST /api/orders` through the Django test client, and assert both that the response is `201` with a valid `order_id` and that the row exists in the `orders` table with the right values. No mocks between the view and the database.

**Regression testing asks: did this change break something that already worked?** It is not a type of test so much as a reason for running tests. The regression suite is the collection of tests you re-run after every change, usually in CI, specifically to catch unintended side effects. Any kind of test (unit, integration, end-to-end) can be part of it.

*Example:* suppose a bug where the cart total ignored shipping for orders over $100 was fixed two months ago and a test was added. When you later refactor the pricing code, that test runs again. If it goes red, your refactor reintroduced the old bug, and you find out before merge rather than from a customer.

Putting it together: "add it to the regression suite" means write tests for the new behaviour and make sure they run on every future change; "integration tests cover the new endpoint" means test the endpoint against the real stack, not just mocked pieces. One test can easily satisfy both requests.
````

## Response B

````text
Good question, and it is one that trips up plenty of people with years of experience, so no need to feel behind. The two terms describe different axes of testing: one is about *what* you are testing (how parts fit together), the other is about *why* you are running tests (to catch things that used to work and silently broke).

**Integration testing** checks that separately built components actually cooperate when wired together. Unit tests can all pass while the pieces still disagree about data formats, authentication, or ordering. Integration tests exist to expose exactly those seams.

*Example:* your new Django endpoint `POST /api/orders` is tested with a real test database rather than mocks. The test sends a request through the URL router, the view, the serializer, and the ORM, then asserts that a row landed in the `orders` table and the JSON response carries the generated `order_id`. Any one of those layers could pass its own unit tests and still break the chain.

**Regression testing** is about protecting existing behaviour over time. Re-running your existing suite of previously passing tests after a change, to confirm nothing that used to work has broken, is called integration testing because it integrates the new change with everything already shipped. The suite grows as bugs are fixed: each fix gets a test so the bug cannot quietly return.

*Example:* last sprint someone fixed a bug where discount codes were applied twice at checkout. A test for that case now lives in the suite, and when you touch the pricing module next month it runs again automatically. If it fails, you know your change reintroduced the old bug.

So when your lead said "add it to the regression suite," she meant "make sure this behaviour is protected going forward," and "integration tests for the endpoint" meant "prove the endpoint works end to end with the real stack." They are complementary, not competing.
````

## Fact checks performed

- Compared both definitions with the ISTQB Glossary definitions as I know them (not re-opened today): regression testing is re-testing a previously tested system after a change to make sure nothing that worked has broken, and integration testing is testing the interfaces and interactions between components. A matches both. B's integration paragraph matches, but its regression paragraph says re-running previously passing tests after a change "is called integration testing", which is the regression definition under the wrong name.
- Ran a short Python script over the file to confirm the sentence "is called integration testing because it integrates the new change with everything already shipped" appears verbatim in B and nowhere in A. Result: present in B, absent from A.
- Read B's regression section for internal consistency: the heading says "**Regression testing**", the discount-code example describes a regression, and only the defining sentence says "integration testing". So the sentence is the error, not the heading, and the paragraph contradicts itself.
- Counted words and paragraphs with the same script: B is 317 words in 6 paragraphs, A is 317 words in 6 paragraphs. Length and structure are identical, so neither can decide the comparison.
- Checked A's integration example against what the Django testing documentation says about the test client (Django is not installed on this machine, so I could not step through it): the client does not need a running server and calls the Django handler in-process rather than over a socket, so "send a real HTTP request ... through the Django test client" is loosely worded. The rest of the example (Postgres test database, `201`, row in `orders`, no mocks between view and database) is sound.
- Checked both examples for the "one concrete example of each" request: B gives the `POST /api/orders` test and the double-applied discount code; A gives the `POST /api/orders` test and the shipping-over-$100 cart total. Both satisfy the request and neither drifts into history.

## Scores

| Criterion | A | B | Justification |
|---|---|---|---|
| Accuracy | 4 | 2 | B defines regression as "called integration testing", a wrong central definition; A is correct apart from calling the Django test client a "real HTTP request" |
| Relevance | 5 | 4 | Both stay on the question; A's closing "One test can easily satisfy both requests" ties it straight back to the standup |
| Completeness | 5 | 3 | B only hints at the what/why split; A says outright that any kind of test (unit, integration, end-to-end) can sit in a regression suite |
| Clarity | 5 | 2 | B's regression paragraph contradicts its own heading and example, which is the exact confusion the user wanted cleared up; A is clean and consistent |
| Adherence to guidelines | 5 | 5 | No system instruction; both are practical, give one example of each and skip the history as asked |
| Safety | 5 | 5 | Benign request, nothing harmful in either |
| **Total (out of 30)** | 29 | 21 | |

## Verdict

A strongly preferred. Confidence: high.

## Rationale

[critical] B gets the key definition wrong. It says re-running previously passing tests after a change "is called integration testing because it integrates the new change with everything already shipped". That is the definition of regression testing, and it sits under B's own regression heading next to a correct regression example.
A junior tester would come away with the right idea attached to the wrong word, which is the exact thing they asked for help with. The polish elsewhere in B does not make up for that.
A defines both terms correctly, says plainly that regression is a reason for running tests rather than a type of test, and ends by explaining that one test can satisfy both of the lead's requests.
[minor] A says the Django test client sends "a real HTTP request". It actually runs in-process. This costs A a point on accuracy but does not affect the verdict.

## What a better response would have done

A's structure and examples, with B's what-versus-why framing kept and the regression paragraph saying plainly that re-running existing tests after a change is regression testing.
It would also say the Django test client exercises the full request cycle in-process rather than sending a real HTTP request.
