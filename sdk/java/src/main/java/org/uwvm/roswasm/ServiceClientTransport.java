package org.uwvm.roswasm;

/** Explicit OPTIONAL service-client ABI1, independent of Transport's business
 * operation inventory. A custom implementation is a local test/embedding seam,
 * never Native acceptance. Result.data is exactly eight LE feature bytes or one
 * canonical boolean byte on success; nonzero errno is never readiness=false.
 * Handles retain all i64 bits; query wait is a finite nonnegative nanosecond
 * count, at most60e9. No method opens an owner, sends a request, or grants rights.
 */
public interface ServiceClientTransport extends Transport {
    int serviceClientAbiVersion();
    Result observeServiceClient(boolean features,long availability,long source,long waitNs);
}
