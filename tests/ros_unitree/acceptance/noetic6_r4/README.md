# Noetic r6d current metadata source

This is a separately reviewed source package for a fresh all513 acquisition.
The source author ran no SSH command, candidate import, test, compiler or SDK
acquisition. The producer, model, rebuild/storage helpers and restart helper
are exact reviewed r3 bytes. Only the owner/stager metadata changes: fresh r6d
paths and one explicit, already-prepared finite cache-view receipt/root.

The stage request template records Root-observed boot3 inputs and the genuine
39,015-byte content receipt SHA256
`9985cef5f0398c4f48b05142c9b6850e2b6c14b286c4f9de64b759af8ba6de9a`.
Root must reopen and independently confirm every current selection before
writing its actual request. The template does not forecast any r6d output pin.
The old r5 partial SDK remains unqualified; all513 signatures, model decodes,
extractions and final ledger/physical checks remain mandatory.

Root stages the source package and actual request under a fresh owned controls
directory, then invokes the source stager inside its actual64GiB/swap0 guard:

```text
/usr/bin/python3.14 -B /ABSOLUTE_STAGED_SOURCE/stage.py \
  --request /ABSOLUTE_STAGED_SOURCE/actual-current-facts.json \
  --request-sha256 ACTUAL_REQUEST_SHA256 \
  --payload /ABSOLUTE_STAGED_SOURCE/reviewed-source-package.json \
  --payload-sha256 ACTUAL_REVIEWED_PACKAGE_SHA256
```

The stager performs no actor, network, clone, trim or package execution. It
validates both whole original42-file and sealed41-file trees, writes fresh
`B/noetic-amd64-native-sdk-acquisition-controls-r6d`, and emits an actual
`unexecuted_owner_argv` in
`B/noetic-amd64-native-sdk-acquisition-stage-r6d.json`. Root reviews that actual
metadata receipt and launches exactly its bound owner command in the guard.
The actor and owner outputs are fresh `M/...acquisition-r6d` and
`M/...acquisition-root-supervisor-r6d`. Existing r6c outputs remain intact.

The owner repeats finite receipt/tree admission and final preservation, then
uses the unchanged producer. Old raw failed provenance is retained as failed
history. The fourth cache root is `M/...cacheview-r6d/debs`; the unregistered
87,900-byte partial remains only in the original r6c directory. No skip rule is
added to the scanner, and the next fresh signed closure decides eligibility.

The maintained factory and complete design rationale are in
`../noetic6_next/derive_r6d_metadata.py` and
`../noetic6_next/NOETIC_R6D_CACHE_PLAN.md`. They are source artifacts, not evidence
of a successful SDK acquisition or ROS compatibility result.
