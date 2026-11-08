// Real host-owned function replacement under a genuine cooperative stop.
#include <uwvm2/uwvm/run/owned_source.h>
#include <uwvm2/uwvm/cmdline/callback/impl.h>
#include <uwvm2/utils/container/string_concat.h>
#include <fast_io.h>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <thread>
namespace lib=uwvm2::runtime::lib;
namespace mode=uwvm2::uwvm::runtime::runtime_mode;
namespace threads=uwvm2::utils::thread;
using domain=threads::cooperative_pause_domain;
static void check(bool value,char const* message) {
 if(!value) {std::fprintf(stderr,"memtag replacement: %s\n",message);std::abort();}
}
static auto deadline() {return std::chrono::steady_clock::now()+std::chrono::seconds(60);}
struct observer {
 std::shared_ptr<domain> control{std::make_shared<domain>(1u)};
 std::mutex mutex;std::condition_variable changed;
 domain::pause_ticket ticket{};
 lib::llvm_jit_debug_activation_capture_owner capture{};
 bool requested{},finished{},resumed{};
 static void point(void* opaque,std::uint_least64_t,threads::cooperative_pause_location where) noexcept {
  auto& self=*static_cast<observer*>(opaque);
  std::lock_guard lock(self.mutex);
  if(self.requested || self.resumed || where.function!=0u || where.offset!=0u)return;
  self.ticket=self.control->request_pause();check(bool(self.ticket),"real pause request");
  self.requested=true;self.changed.notify_all();
 }
 static void before_park(void* opaque,std::uint_least64_t,threads::cooperative_pause_location,
   lib::llvm_jit_debug_local_view) noexcept {
  auto& self=*static_cast<observer*>(opaque);std::lock_guard lock(self.mutex);
  self.capture=lib::llvm_jit_debug_capture_activation_host_api(self.ticket);
  check(bool(self.capture),"actual activation capture");
 }
};
int main(int argc,char** argv) {
 if(argc!=5)return 64;
 bool disabled=std::strcmp(argv[3],"disabled")==0;
 if(!disabled && std::strcmp(argv[3],"enabled")!=0)return 64;
 unsigned expected=static_cast<unsigned>(std::strtoul(argv[4],nullptr,10));
 check(uwvm2::utils::control::debug_permissions::configure_trusted_launch(
  uwvm2::utils::control::debug_permissions::permission_level::full),"trusted host DEV launch");
 fast_io::native_file_loader replacement{fast_io::mnp::os_c_str(argv[2])};
 mode::global_runtime_mode=mode::runtime_mode_t::full_compile;
 mode::global_runtime_compiler=mode::runtime_compiler_t::llvm_jit_only;
 mode::global_runtime_compile_threads=0u;mode::runtime_compile_threads_existed=true;
 mode::global_runtime_llvm_jit_cache_path_mode=mode::runtime_llvm_jit_cache_path_mode_t::disabled;
 mode::runtime_disable_memtag=disabled;
 auto const path=uwvm2::utils::container::u8concat_uwvm(fast_io::mnp::code_cvt(fast_io::mnp::os_c_str(argv[1])));
 auto& arguments=uwvm2::uwvm::cmdline::parsing_result;
 uwvm2::uwvm::cmdline::wasm_file_ppos=nullptr;arguments.clear();
 arguments.emplace_back(uwvm2::utils::cmdline::parameter_parsing_results{
  u8"memtag-replacement",nullptr,uwvm2::utils::cmdline::parameter_parsing_results_type::dir});
 arguments.emplace_back(uwvm2::utils::cmdline::parameter_parsing_results{
  uwvm2::utils::container::u8cstring_view{fast_io::mnp::os_c_str(path.c_str())},nullptr,
  uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg});
 uwvm2::uwvm::cmdline::wasm_file_ppos=std::addressof(arguments.back());
 auto& features=uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(
  uwvm2::uwvm::wasm::storage::wasm_parameter.binfmt1_para);
 features.disable_memory64=false;features.explicit_enable_memory64=true;
 features.disable_memtag_extension=false;
 uwvm2::uwvm::wasm::storage::execute_wasm.module_name=u8"memtag-replacement";
 check(uwvm2::uwvm::run::prepare_owned_full_cli_source()==
   static_cast<int>(uwvm2::uwvm::run::retval::ok),"actual source initialization");
 auto state=std::make_shared<observer>();
 check(lib::llvm_jit_configure_debug_session_host_api(state->control,
   {state,observer::point,observer::before_park},lib::llvm_jit_debug_safe_point_granularity::instruction)==
   lib::llvm_jit_debug_configure_result::ok,"host debug admission");
 check(lib::llvm_jit_prepare_debug_host_api(),"full original compilation");
 std::uint32_t result{0xdeadbeefu};
 std::thread guest([&] {
  lib::full_compile_run_config run{};run.entry_function_index=0u;
  run.entry_abi_buffers.result_buffer=reinterpret_cast<std::byte*>(std::addressof(result));
  run.entry_abi_buffers.result_bytes=sizeof(result);
  lib::full_compile_and_run_main_module(u8"memtag-replacement",run);
  std::lock_guard lock(state->mutex);state->finished=true;state->changed.notify_all();
 });
 domain::pause_ticket ticket{};
 {
  std::unique_lock lock(state->mutex);
  check(state->changed.wait_until(lock,deadline(),[&]{return state->requested || state->finished;}),"stop deadline");
  check(state->requested && !state->finished,"original guest stopped before target entry");ticket=state->ticket;
 }
 check(state->control->wait_until_paused(ticket,deadline())==threads::cooperative_pause_result::paused,
   "complete real cohort stop");
 lib::llvm_jit_debug_activation_snapshot active{};
 check(lib::llvm_jit_debug_query_activation_host_api(state->capture,active) && active.frames.size()==1u &&
   active.frames[0].function==0u,"replacement target absent from owned stack");
 auto const* body=reinterpret_cast<std::byte const*>(replacement.data());
 auto stale=lib::llvm_jit_debug_prepare_function_replacement_host_api(0u,1u,0u,body,replacement.size());
 check(stale.status==lib::llvm_jit_debug_replace_status::stale_generation && !stale.transaction,"stale generation rejected");
 auto prepared=lib::llvm_jit_debug_prepare_function_replacement_host_api(0u,1u,1u,body,replacement.size());
 check(prepared.status==lib::llvm_jit_debug_replace_status::replaced && prepared.transaction,"actual replacement compiled");
 lib::llvm_jit_debug_replace_result committed{};
 check(state->control->while_stopped(ticket,[&] {
  committed=lib::llvm_jit_debug_commit_function_replacement_host_api(prepared.transaction);
 }) && committed.status==lib::llvm_jit_debug_replace_status::replaced && committed.generation==2u,
  "publication under current real stop");
 lib::llvm_jit_debug_discard_function_replacement_host_api(prepared.transaction);
 {
  std::lock_guard lock(state->mutex);state->resumed=true;
 }
 check(state->control->resume(ticket),"resume original caller");guest.join();
 if(lib::runtime_guest_fault_detected_host_api()) {
  check(result==0xdeadbeefu,"fault does not publish a function result");
  lib::reset_runtime_state_host_api();
  return 126;
 }
 check(result==expected,"replacement result obeys original execution policy");
 lib::reset_runtime_state_host_api();
 std::printf("memtag replacement PASS result=%u policy=%s\n",result,disabled?"disabled":"enabled");
}
