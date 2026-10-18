# Evaluation 07: Node function with await inside a loop

**Date:** (fill in when you write the evaluation)
**Domain:** coding (JavaScript)
**Models compared:** A = generated for this repository, B = generated for this repository
**Source of prompt:** written for this repository

## Prompt

```text
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
```

**System instruction or guideline the response had to follow:**

```text
None
```

## Response A

```text
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
```

## Response B

```text
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
