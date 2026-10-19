# Third-party Source and generated products

- The151 unchanged original Spot SDK5.0.1 proto files and generated field APIs
  derived from its149 production proto files remain subject to the Boston
  Dynamics Software Development Kit License preserved at upstream/spot-sdk-LICENSE.
  Original Source copyright/license comments are retained. This project does not
  relicense vendor schema material or grant robot/network permissions.
- The six original Google WKT files are unchanged from the Native SDK dependency
  protobuf3.19.4 source pin. The separately selected Guest C++ Lite runtime,
  generator metadata and Python tool runtime are fixed at protobuf v21.12.
  Their original BSD3 license is at upstream/protobuf/LICENSE.
- nanopb0.4.9.1 runtime and generator originals are unchanged, with their zlib
  license at upstream/nanopb/LICENSE.txt. External options describe the C field
  representation; they do not edit original proto declarations.
- Project-authored wrappers/operators/examples/contracts use this repository's
  Apache2 terms. No library/generator binary is embedded in the apply Source.
  The actual official protoc binary identity and publisher checksum are retained
  as separate acquisition evidence. Tool invocation is Source generation only.

Git commit/tree/blob and SHA256 records distinguish unchanged original Source,
real generator products, authored wrappers and historical snapshots. Those
records are not installed-SDK/Native/robot/Wasm certificates.

The C target compiles src/nanopb_decode_merged.c, an explicitly marked altered
copy of nanopb0.4.9.1 pb_decode.c, to implement pointer singular-message protobuf
merge semantics. Its original notice/zlib terms and unchanged upstream Source
are preserved. The other nanopb runtime/generator originals remain unchanged.
