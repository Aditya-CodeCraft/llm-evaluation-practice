# Evaluation 02: SQL query for frequent customers in 2025

**Date:** (fill in when you write the evaluation)
**Domain:** coding (SQL)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

```text
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
```

**System instruction or guideline the response had to follow:**

```text
None
```

## Response A

```text
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
```

## Response B

```text
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
