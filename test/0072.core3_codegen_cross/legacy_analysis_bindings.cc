// Target-independent regression for the legacy pass-manager binding fast path.
// Link against LLVMCore/LLVMSupport and the configured dependency closure.
#include <llvm/IR/LegacyPassManagers.h>
#include <llvm/Pass.h>
#include <array>
#include <cstdlib>
#include <memory>
#include <vector>

using namespace llvm;

class ProbePass final : public ModulePass {
public:
  std::vector<AnalysisID> Required;
  explicit ProbePass(char &ID) : ModulePass(ID) {}
  bool runOnModule(Module &) override { return false; }
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    for (AnalysisID ID : Required) {
      AU.addRequiredID(ID);
      AU.addRequiredID(ID); // AnalysisUsage must still make the sequence unique.
    }
  }
};

class ProbeManager final : public PMDataManager, public PMTopLevelManager {
  char OwnerID{};
  ProbePass Owner;
public:
  ProbeManager() : PMTopLevelManager(this), Owner(OwnerID) {}
  ~ProbeManager() override { PassManagers.clear(); }
  Pass *getAsPass() override { return &Owner; }
  PMDataManager *getAsPMDataManager() override { return this; }
  PassManagerType getTopLevelPassManagerType() override {
    return PMT_ModulePassManager;
  }
};

static void require(bool OK) {
  if (!OK)
    std::abort();
}

int main() {
  ProbeManager PM;
  std::array<char, 34> IDs{};
  std::vector<std::unique_ptr<ProbePass>> Providers, Replacements;
  for (unsigned I = 0; I < 32; ++I) {
    Providers.emplace_back(std::make_unique<ProbePass>(IDs[I]));
    Replacements.emplace_back(std::make_unique<ProbePass>(IDs[I]));
  }
  ProbePass Consumer(IDs[32]), Other(IDs[33]);
  Consumer.setResolver(new AnalysisResolver(PM));
  for (unsigned I = 0; I < 32; ++I)
    Consumer.Required.push_back(&IDs[I]);
  auto *Resolver = Consumer.getResolver();
  auto &Available = *PM.getAvailableAnalysis();
  std::vector<std::pair<AnalysisID, Pass *>> Expected;
  auto Initialize = [&] {
    // Exact original addAnalysisImplsPair semantics, including first binding
    // winning if an existing ID is later offered a different implementation.
    for (AnalysisID ID : Consumer.Required) {
      Pass *Impl = Available.lookup(ID);
      if (!Impl)
        continue;
      Pass *First = nullptr;
      for (auto Pair : Expected)
        if (Pair.first == ID) {
          First = Pair.second;
          break;
        }
      if (First != Impl)
        Expected.emplace_back(ID, Impl);
    }
    PM.initializeAnalysisImpl(&Consumer);
    for (AnalysisID ID : Consumer.Required) {
      Pass *First = nullptr;
      for (auto Pair : Expected)
        if (Pair.first == ID) {
          First = Pair.second;
          break;
        }
      require(Resolver->findImplPass(ID) == First);
    }
  };

  // Empty, partial, repeated and newly available on-the-fly bindings.
  Initialize();
  for (unsigned I = 0; I < 32; I += 2)
    Available[&IDs[I]] = Providers[I].get();
  Initialize();
  Initialize();
  for (unsigned I = 1; I < 32; I += 2)
    Available[&IDs[I]] = Providers[I].get();
  Initialize();
  // Clear followed by a complete initial fill exercises the matching prefix.
  Resolver->clearAnalysisImpls();
  Expected.clear();
  for (unsigned I = 0; I < 16; ++I)
    Initialize();
  // Mismatches at every position must fall back to original semantics.
  for (unsigned I = 0; I < 32; ++I) {
    Available[&IDs[I]] = Replacements[I].get();
    Initialize();
  }
  for (unsigned I = 0; I < 32; ++I) {
    Available.erase(&IDs[I]);
    Initialize();
  }
  Resolver->clearAnalysisImpls();
  Expected.clear();
  // Pre-existing out-of-order bindings cannot incorrectly use the prefix path.
  for (unsigned I = 32; I-- > 0;) {
    Resolver->addAnalysisImplsPair(&IDs[I], Providers[I].get());
    Expected.emplace_back(&IDs[I], Providers[I].get());
    Available[&IDs[I]] = Providers[I].get();
  }
  Initialize();
  PM.initializeAnalysisImpl(&Other); // Empty requirements need no resolver.

  // Known-size last-use copying preserves existing contents and each member.
  SmallVector<Pass *, 32> All;
  for (auto &P : Providers)
    All.push_back(P.get());
  PM.setLastUser(All, &Consumer);
  SmallVector<Pass *, 12> Uses{&Other};
  PM.collectLastUses(Uses, &Consumer);
  require(Uses.size() == 33 && Uses.front() == &Other);
  for (Pass *P : All)
    require(llvm::count(Uses, P) == 1);
  PM.setLastUser(All, &Other);
  Uses.clear();
  PM.collectLastUses(Uses, &Consumer); // A previously grown empty sparse set.
  require(Uses.empty());
  PM.collectLastUses(Uses, &Other);
  require(Uses.size() == 32);
  return 0;
}
