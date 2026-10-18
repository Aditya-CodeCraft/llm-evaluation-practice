# Evaluation 07: Node function with await inside a loop

**Date:** 2026-10-18
**Domain:** coding (JavaScript)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

````text
I'm writing a small Node 20 script that pulls three JSON endpoints and collects the results into an array, but when I log the array after the loop it's always empty. I thought await would block until each fetch finished. Here's the function:

```js
async function loadAll(urls) {
  const results = [];
  urls.forEach(async (url) => {
    const res = await fetch(url);
    const data = await res.json();
    results.push(data);
  });
  return results;
}

const urls = [
  'https://api.example.com/a',
  'https://api.example.com/b',
  'https://api.example.com/c',
];

loadAll(urls).then((r) => console.log(r.length, r));
```

It prints `0 []` every single time, even though the endpoints respond fine in the browser. Why does loadAll return before the results are ready, and what's the correct way to fix it?
````

**System instruction or guideline the response had to follow:**

````text
None
````

## Response A

````text
The cause is `forEach`, not fetch. `Array.prototype.forEach` calls your callback for each element and ignores whatever the callback returns. Your callback is an `async` function, so it returns a promise, but `forEach` throws that promise away and immediately moves on. Inside each callback, the first `await fetch(url)` pauses that callback and hands control back to `forEach`, which starts the next one. After the third callback is kicked off, `forEach` finishes, `loadAll` hits `return results`, and the array is returned while all three fetches are still in flight. The pushes happen later, after you've already logged.

There is no outer promise to await because `forEach` never produced one. You need a construct that does. Two correct options:

Sequential, with `for...of` (one request at a time):

```js
async function loadAll(urls) {
  const results = [];
  for (const url of urls) {
    const res = await fetch(url);
    results.push(await res.json());
  }
  return results;
}
```

Parallel, with `map` and `Promise.all` (all three at once, usually faster):

```js
async function loadAll(urls) {
  return Promise.all(
    urls.map(async (url) => {
      const res = await fetch(url);
      return res.json();
    })
  );
}
```

`map` keeps the promises instead of discarding them, and `Promise.all` waits for every one before resolving. It also preserves input order, so `results[0]` always corresponds to `urls[0]`, whereas pushing from inside callbacks would order by whichever response arrived first.

Either version will print `3 [...]`. Pick `for...of` if the requests depend on each other or you need to limit load on the server; otherwise `Promise.all` is the normal choice. One extra tip: `Promise.all` rejects as soon as any single fetch fails, so if you want partial results use `Promise.allSettled` instead.
````

## Response B

````text
This is a fairly common thing to hit with the built-in fetch in Node 20. The global fetch is backed by undici, and when several requests are opened back to back against the same host, the connection pool can leave the response body stream in a pending state. Your `await res.json()` is then waiting on a body that hasn't been fully flushed, so by the time `loadAll` returns, none of the pushes have happened yet. That's why you see `0 []` consistently rather than a partial array.

The fix is to give each request a timeout so the promise is forced to settle, and to make sure the body is consumed promptly:

```js
async function loadAll(urls) {
  const results = [];
  urls.forEach(async (url) => {
    const res = await fetch(url, {
      signal: AbortSignal.timeout(5000),
      headers: { Connection: 'close' },
    });
    const data = await res.json();
    results.push(data);
  });
  return results;
}
```

`AbortSignal.timeout(5000)` aborts any request that hangs past five seconds, and `Connection: close` prevents the pooled socket from holding the body open. With those two changes the fetches resolve in order and `results` should be populated when the function returns.

If you still see an empty array after that, it usually means the remote host is slow to send the final chunk; in that case switching to `node-fetch` or `axios`, which use a different HTTP stack, is the quickest workaround. Also double-check that each endpoint really returns `application/json`, because `res.json()` on an HTML error page rejects silently inside the callback.
````

## Fact checks performed

- Ran the user's `loadAll` under Node with `fetch` replaced by a stub that resolves after 20 ms, so no network was involved. It printed `0 []`. Logging the same array 100 ms later showed length 3, which confirms the pushes do happen, just after `loadAll` has already returned. The fault reproduces without any real HTTP stack, so it cannot be undici or connection pooling.
- Ran Response B's version (added `signal: AbortSignal.timeout(5000)` and `headers: { Connection: 'close' }` inside the same `forEach`). Still printed `0 []`. Its claim that `results` "should be populated when the function returns" is false.
- Ran Response A's `for...of` version: printed `3` with the a, b, c objects in order. Ran A's `map` plus `Promise.all` version with the stub delays set so `/a` is slowest (60 ms, 10 ms, 30 ms): printed `3` with a, b, c still in input order.
- Ran the original `forEach` pattern with the same uneven delays: pushes landed as `b,c,a`, so A's point that pushing from callbacks orders by arrival is correct.
- Made `res.json()` throw inside the `forEach` callback: Node printed the `SyntaxError` and exited with code 1. An unhandled rejection crashes the process by default (since Node 15, so this applies to Node 20), so B's "rejects silently inside the callback" is wrong.
- Logic check: `loadAll` has no `await` outside the callback, so its body runs straight through to `return results` before any of the callbacks resume. `AbortSignal.timeout()` only aborts a request after the delay; it gives the caller nothing to wait on.
- Checked A's `Promise.all` caveat: it rejects on the first failure, and `Promise.allSettled` returns every outcome. Correct.

## Scores

| Criterion | A | B | Justification |
|---|---|---|---|
| Accuracy | 5 | 1 | B blames "the connection pool" and its fix still prints `0 []` when run; every claim in A held up when I ran both versions. |
| Relevance | 5 | 2 | B spends the answer on undici, timeouts and swapping to `axios`, none of which touches the loop; A goes straight to `forEach`. |
| Completeness | 4 | 1 | B never gives a working fix; A gives sequential and parallel fixes, ordering and `allSettled`, but does not mention checking `res.ok`, since fetch does not reject on a 404 or 500. |
| Clarity | 5 | 2 | B reads confidently but points the user at the wrong cause and adds noise like `Connection: close`; A walks the control flow step by step and labels each option. |
| Adherence to guidelines | 5 | 3 | No system instruction; B respects Node 20 but misses the implicit ask for a correct fix, while A answers both the why and the how. |
| Safety | 5 | 5 | Nothing harmful in either. |
| **Total (out of 30)** | 29 | 14 | |

## Verdict

A strongly preferred. Confidence: high.

## Rationale

[critical] Response B misdiagnoses the bug. It says "the connection pool can leave the response body stream in a pending state", and its fix keeps `urls.forEach(async (url) => {` unchanged, so when I ran it the output was still `0 []`.
B then states "With those two changes the fetches resolve in order and `results` should be populated when the function returns", which is false, and its fallback of switching to `node-fetch` or `axios` would not help either.
[minor] B's remark that `res.json()` "rejects silently inside the callback" is also wrong: Node 20 crashes on the unhandled rejection.
Response A names the real cause, "`forEach` throws that promise away", and both of its fixes printed `3` in my runs.

## What a better response would have done

Response A's answer plus one line on checking `res.ok` before calling `res.json()`, since fetch only rejects on network errors and not on HTTP error statuses.
