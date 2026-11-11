// Host diagnostics sink binding. APL-2.0.
#pragma once
#include <uwvm2/utils/control/host_diagnostics.h>
#include <uwvm2/imported/ros/common/audit_io.h>
#include <uwvm2/utils/thread/process_exit.h>
#include <uwvm2/uwvm/io/impl.h>
namespace uwvm2::uwvm::io
{
    inline void host_diagnostic_write(std::string_view record) noexcept
    {
        namespace safe=::uwvm2::imported::ros::audit_io;
#if defined(__cpp_exceptions)
        try
#endif
        {
#if defined(__linux__)
            safe::details::errno_guard preserve;
            std::lock_guard lock{u8log_output.mutex};
            // PIPE_BUF framing is not a limit on paths, names or diagnostics.
            // Long records are split; failed delivery is counted, never fatal.
            do
            {
                auto part=record.substr(0,4000);safe::failure reason{safe::failure::io_error};
                bool done=safe::details::linux_write(part,reason,u8log_output.handle.native_handle());
                safe::increment(done?safe::delivered:safe::dropped);
                if(!done){safe::increment(safe::failures[static_cast<std::size_t>(reason)]);break;}
                record.remove_prefix(part.size());
            }while(!record.empty());
#else
            (void)safe::write_record(record);
#endif
        }
#if defined(__cpp_exceptions)
        catch(...) {safe::increment(safe::dropped);}
#endif
    }
    inline bool const host_diagnostic_binding=[]{
        ::uwvm2::utils::control::host_diagnostics::sink=host_diagnostic_write;
        ::uwvm2::utils::control::host_diagnostics::fatal=[]() noexcept
        {
            // A host policy diagnostic promoted to error is a controlled
            // configuration rejection, independent of the Wasm fault action.
            ::uwvm2::utils::thread::process_exit::request_process_exit(
                static_cast<unsigned>(::uwvm2::uwvm::io::diagnostic_exit_status::invalid_option));
        };
        return true;
    }();
}
