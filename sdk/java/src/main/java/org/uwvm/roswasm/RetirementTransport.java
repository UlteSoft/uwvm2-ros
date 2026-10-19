package org.uwvm.roswasm;

/** Internal preparation proof, implemented by the two reviewed final numeric
 * transports. A public/custom Transport has NO such proof and is conservatively
 * treated as effect-unknown at invoke entry. Local tests implementing this
 * package-private contract are models, not native acceptance evidence.
 */
interface RetirementTransport {
    Result retire(String operation,long handle,byte[] empty,RetirementAttempt attempt);
}
