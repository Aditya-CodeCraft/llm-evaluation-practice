# Evaluation 02: SQL query for frequent customers in 2025

**Date:** 2026-10-18
**Domain:** coding (SQL)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

````text
I'm on Postgres and need a query for a quick report: every customer who placed MORE than three orders during 2025, with how many orders they placed and their total spend for the year. Here are the two tables and the test data I'm using.

customers(customer_id INT, name TEXT)
customer_id | name
1 | Ana Ruiz
2 | Ben Okafor
3 | Cleo Marsh

orders(order_id INT, customer_id INT, order_date TIMESTAMP, amount NUMERIC)
order_id | customer_id | order_date | amount
101 | 1 | 2025-01-10 09:15:00 | 40.00
102 | 1 | 2025-03-05 13:40:00 | 25.00
103 | 1 | 2025-06-21 18:02:00 | 60.00
104 | 1 | 2025-12-31 15:42:00 | 15.00
105 | 2 | 2024-12-30 11:00:00 | 90.00
106 | 2 | 2025-02-02 10:10:00 | 30.00
107 | 2 | 2025-05-19 16:30:00 | 30.00
108 | 2 | 2025-08-08 12:00:00 | 30.00

Can you give me the query and tell me what it should return on this data so I can sanity-check it?
````

**System instruction or guideline the response had to follow:**

````text
None
````

## Response A

````text
This is a standard filter-then-aggregate pattern:

```sql
SELECT c.customer_id,
       c.name,
       COUNT(*)      AS orders_in_2025,
       SUM(o.amount) AS total_spend
FROM customers c
JOIN orders o ON o.customer_id = c.customer_id
WHERE o.order_date >= '2025-01-01'
  AND o.order_date <  '2026-01-01'
GROUP BY c.customer_id, c.name
HAVING COUNT(*) > 3
ORDER BY total_spend DESC;
```

Why it's shaped this way:

- The date test lives in WHERE so only 2025 rows reach the aggregation. I used a half-open range (>= Jan 1, < next Jan 1) rather than BETWEEN, because order_date is a TIMESTAMP: a literal like '2025-12-31' means midnight at the start of that day, so a BETWEEN ending there would silently drop anything placed later on the 31st.
- GROUP BY gives one row per customer; COUNT(*) and SUM(amount) are computed over that customer's 2025 rows only.
- The "more than three" rule is an aggregate condition, so it must go in HAVING, which runs after grouping.

Expected output on your data:

customer_id | name | orders_in_2025 | total_spend
1 | Ana Ruiz | 4 | 140.00

Ben Okafor is excluded: he has four orders overall, but 105 is from 2024, leaving three in 2025, and three is not more than three. Cleo Marsh has no orders.
````

## Response B

````text
Here's a query that does it in one pass:

```sql
SELECT c.customer_id,
       c.name,
       COUNT(o.order_id) AS order_count,
       SUM(o.amount)     AS total_spend
FROM customers c
JOIN orders o ON o.customer_id = c.customer_id
WHERE o.order_date BETWEEN '2025-01-01' AND '2025-12-31'
GROUP BY c.customer_id, c.name
HAVING COUNT(o.order_id) > 3
ORDER BY total_spend DESC;
```

How it works:

- The WHERE clause restricts the rows to 2025 before anything is aggregated. BETWEEN is inclusive on both ends, so this captures every order from January 1 through December 31.
- GROUP BY collapses the remaining rows to one per customer, and COUNT / SUM compute the per-customer figures.
- HAVING applies the "more than three" rule after grouping. You can't put COUNT in WHERE because WHERE runs row by row, before any counts exist.
- ORDER BY puts the biggest spenders first, which is usually what a report wants.

Expected output on your data:

customer_id | name | order_count | total_spend
1 | Ana Ruiz | 4 | 140.00

Ben Okafor has four orders in total, but order 105 is from December 2024, so only three count and he is correctly left out. Cleo Marsh has no orders and never survives the inner join.
````

## Fact checks performed

- Ran both queries. There is no Postgres on this machine, so I created the two tables in SQLite (Python `sqlite3`) and inserted the eight rows exactly as given. SQLite compares these timestamps as text, which gives the same boundary result here. Response B's query returned no rows. Response A's query returned one row: 1, Ana Ruiz, 4, 140.
- Ran Response B's query with the HAVING clause removed to see what the date filter keeps: Ana Ruiz 3 orders / 125, Ben Okafor 3 orders / 90. Order 104 (2025-12-31 15:42:00) is the row being dropped.
- Ran Response A's query without HAVING: Ana Ruiz 4 / 140, Ben Okafor 3 / 90, so A's date range keeps all four of Ana's 2025 orders and correctly drops Ben's 2024 order.
- Computed the expected result by hand. Ana: orders 101, 102, 103, 104 = 4 orders, 40 + 25 + 60 + 15 = 140.00. Ben: 106, 107, 108 in 2025 = 3 orders (105 is 2024-12-30), and 3 is not more than 3, so he is out. Cleo has no orders. Correct output is Ana only, 4, 140.00.
- Reasoned through the Postgres semantics, since I could not run Postgres itself: the manual defines `a BETWEEN x AND y` as `a >= x AND a <= y`, and a date-only literal compared with a TIMESTAMP is read as midnight (00:00:00). So `TIMESTAMP '2025-12-31 15:42:00' <= '2025-12-31'` is false and B's upper bound excludes everything after midnight on 31 December. B's claim that BETWEEN "captures every order from January 1 through December 31" is wrong for this column type.

## Scores

| Criterion | A | B | Justification |
|---|---|---|---|
| Accuracy | 5 | 1 | B's query returns nothing on the test data and its stated output (Ana, 4, 140.00) is not what it produces; A's query and stated output match what I ran. |
| Relevance | 5 | 5 | Both give the query, a short explanation and the expected output, which is exactly what was asked. |
| Completeness | 5 | 2 | B misses the TIMESTAMP end-of-year boundary, the one edge case the test data was built to catch; A handles it and explains why, and also spells out why Ben's three orders do not qualify. |
| Clarity | 5 | 4 | Both are well structured, but B states a false rule with confidence, which would mislead a reader trying to sanity-check it. |
| Adherence to guidelines | 5 | 4 | No system instruction; the user asked for the expected result to sanity-check against, and B's expected result does not match its own query. |
| Safety | 5 | 5 | Nothing harmful in either. |
| **Total (out of 30)** | **30** | **21** | |

## Verdict

A strongly preferred. Confidence: high.

## Rationale

[critical] Response B filters with `WHERE o.order_date BETWEEN '2025-01-01' AND '2025-12-31'` on a TIMESTAMP column. The upper bound is midnight at the start of 31 December, so order 104 at 15:42 is dropped, Ana falls to three orders and the query returns no rows.
B then claims "BETWEEN is inclusive on both ends, so this captures every order from January 1 through December 31" and promises Ana with 4 / 140.00, so the user's own sanity check would fail with no explanation.
Response A uses a half-open range (on or after 2025-01-01 and before 2026-01-01), returns exactly the row it promises, and names the pitfall B fell into.
[minor] B only explains Ben's exclusion via the 2024 order; A also states "three is not more than three", which closes off the off-by-one reading. COUNT(o.order_id) versus COUNT(*) makes no difference after an inner join.

## What a better response would have done

Used a half-open date range (or `order_date::date BETWEEN ...`, accepting it cannot use a plain index on order_date) and checked the claimed output against the data, which is what A did. It could also have mentioned that the same boundary issue applies if the column is ever changed to TIMESTAMPTZ, where the session time zone decides where 2025 ends.
