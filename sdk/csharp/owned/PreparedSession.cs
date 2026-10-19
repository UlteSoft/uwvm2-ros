using System;
namespace Uwvm.RosWasm;
public sealed partial class Session {
    /// <summary>Reserve a real core Capability in the actual Session ledger BEFORE
    /// an owned numeric open. The private owned group already captured strict alias
    /// bytes. A pending zero handle grants nothing and consumes one bounded row.</summary>
    internal Capability ReserveOwnedOwner(string alias) {
        lock(gate) {
            if(!Active)throw new Owned.OwnedServiceException(125);
            if(Retiring)throw new Owned.OwnedServiceException(17);
            if(count==owners.Length)throw new Owned.OwnedServiceException(90);
            var owner=new Capability(this,alias);owners[count++]=owner;return owner;
        }
    }
}
