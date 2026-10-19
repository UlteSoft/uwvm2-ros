using System;
namespace Uwvm.RosWasm;

public enum RetirementOperation { Close,Revoke }
public enum RetirementPhase { NotAttempted,AttemptedEffectUnknown,Returned }
public enum RetirementBoundary { None,NativeInteropCallSite,CustomTransportEntry }

/// <summary>Value storage: recording an existing exception/original ID allocates nothing.
/// NativeInteropCallSite means the managed interop expression was entered; it
/// does not prove that the Host executed, committed or delivered an operation.</summary>
public readonly struct RetirementDiagnostic {
    public readonly ulong OriginalHandle;
    public readonly RetirementOperation Operation;
    public readonly RetirementPhase Phase;
    public readonly RetirementBoundary Boundary;
    public readonly uint? ReturnedError;
    public readonly Exception? PrimaryException,CleanupException;
    internal RetirementDiagnostic(ulong handle,RetirementOperation operation,RetirementPhase phase,
        RetirementBoundary boundary,uint? error,Exception? primary,Exception? cleanup) {
        OriginalHandle=handle;Operation=operation;Phase=phase;Boundary=boundary;
        ReturnedError=error;PrimaryException=primary;CleanupException=cleanup;
    }
}
