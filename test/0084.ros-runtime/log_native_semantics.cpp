/** @file @brief Compare canonical private records with actual public rcutils logging semantics.
 * @details The native handler is a fixed fixture, not a production appender.
 * Public rcutils allocations use a finite recoverable fast_io allocator. Its
 * middleware/libc internals are not claimed to share the VM allocation policy.
 */
#include <uwvm2/imported/ros/scene/log_store.h>
#include <uwvm2/imported/ros/scene/native/communication_allocator.h>
#include <rcutils/logging.h>
#include <cassert>
#include <cstdarg>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
struct captured
{
    std::array<char,1024> message{};std::array<char,256> logger{},file{},function{};
    std::uint64_t line{};std::int64_t time{};int level{};
};
static std::array<captured,8> records;
static std::size_t count;
static void handler(rcutils_log_location_t const* location,int severity,char const* name,rcutils_time_point_value_t timestamp,char const* format,va_list* args)
{
    assert(count<records.size()&&location&&std::string_view(format)=="%s");auto& out=records[count++];
    va_list copy;va_copy(copy,*args);auto size=std::vsnprintf(out.message.data(),out.message.size(),format,copy);va_end(copy);
    assert(size>=0&&static_cast<std::size_t>(size)<out.message.size());
    auto literal=[](auto& target,char const* source){auto length=std::strlen(source);assert(length<target.size());std::memcpy(target.data(),source,length+1);};
    literal(out.logger,name);literal(out.file,location->file_name);literal(out.function,location->function_name);
    out.line=location->line_number;out.time=timestamp;out.level=severity;
}
int main()
{
    s::communication::ros2::query_allocator allocation{65536};
    assert(rcutils_logging_initialize_with_allocator(allocation.native())==RCUTILS_RET_OK);
    rcutils_logging_set_output_handler(handler);
    assert(rcutils_logging_set_logger_level("robot",RCUTILS_LOG_SEVERITY_DEBUG)==RCUTILS_RET_OK);
    rcutils_log_location_t location{"run","robot.cpp",73};
    constexpr std::array levels{RCUTILS_LOG_SEVERITY_DEBUG,RCUTILS_LOG_SEVERITY_INFO,RCUTILS_LOG_SEVERITY_WARN,RCUTILS_LOG_SEVERITY_ERROR,RCUTILS_LOG_SEVERITY_FATAL};
    for(auto level:levels)rcutils_log(&location,level,"robot","%s","literal %n %s {} robot \xe6\x9c\xba\xe5\x99\xa8\xe4\xba\xba");
    assert(count==levels.size());
    s::logs::plan plan;plan.logger_count=1;assert(plan.loggers[0].assign("robot"));plan.records=8;plan.record_bytes=2048;plan.severity_mask=31;
    assert(s::logs::store::required_storage(plan,plan.storage_bytes));s::logs::owner owner{1,2,3};s::logs::store store;
    assert(store.configure(owner,plan)==s::logs::error::success);
    for(std::size_t i{};i<count;++i)
    {
        auto const& expected=records[i];assert(expected.level==levels[i]&&expected.time>0);
        s::logs::request value;value.session=1;value.sequence=i+1;value.level=static_cast<s::logs::severity>(i);
        value.logger=expected.logger.data();value.message=expected.message.data();value.has_time=value.has_location=true;
        value.time={1,0,expected.time};value.file=expected.file.data();value.function=expected.function.data();value.line=expected.line;
        std::array<std::uint8_t,2048> data;s::wire::writer writer{data};assert(s::logs::encode(writer,value));s::logs::receipt receipt;
        assert(store.emit(owner,writer.written(),receipt)==s::logs::error::success&&receipt.sequence==i+1&&receipt.record==i+1);
    }
    for(std::size_t i{};i<count;++i)
    {
        assert(store.drain(owner,[&](auto id,auto data)
        {
            s::logs::request value;assert(id==i+1&&s::logs::decode(data,value));auto const& expected=records[i];
            assert(static_cast<std::size_t>(value.level)==i&&value.logger==expected.logger.data()&&value.message==expected.message.data()&&
                value.has_time&&value.time.nanoseconds==expected.time&&value.has_location&&value.file==expected.file.data()&&
                value.function==expected.function.data()&&value.line==73);return true;
        })==s::logs::error::success);
    }
    assert(rcutils_logging_set_logger_level("robot",RCUTILS_LOG_SEVERITY_ERROR)==RCUTILS_RET_OK);
    rcutils_log(&location,RCUTILS_LOG_SEVERITY_WARN,"robot.child","%s","native parent-level filtering");assert(count==5);
    rcutils_logging_set_output_handler(nullptr);assert(rcutils_logging_shutdown()==RCUTILS_RET_OK&&!allocation.used()&&!allocation.exhausted());
    s::logs::usage usage;assert(store.inspect(owner,usage)==s::logs::error::success&&usage.accepted==5&&usage.exported==5&&!usage.retained);
    std::puts("passed actual rcutils five-severity literal/location/time semantic roundtrip; no native appender installed");
}
