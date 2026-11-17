// Standalone WASIp1 tour: no inherited host environment or filesystem access.
using u32 = unsigned;
using u64 = unsigned long long;
#define WASI(name) __attribute__((import_module("wasi_snapshot_preview1"), import_name(#name)))
struct Iovec { const void* data; u32 size; };
extern "C" {
WASI(args_sizes_get) u32 args_sizes_get(u32*,u32*);
WASI(args_get) u32 args_get(char**,char*);
WASI(environ_sizes_get) u32 environ_sizes_get(u32*,u32*);
WASI(environ_get) u32 environ_get(char**,char*);
WASI(fd_write) u32 fd_write(u32,const Iovec*,u32,u32*);
WASI(fd_read) u32 fd_read(u32,const Iovec*,u32,u32*);
WASI(fd_close) u32 fd_close(u32);
WASI(fd_prestat_get) u32 fd_prestat_get(u32,void*);
WASI(fd_prestat_dir_name) u32 fd_prestat_dir_name(u32,char*,u32);
WASI(path_open) u32 path_open(u32,u32,const char*,u32,u32,u64,u64,u32,u32*);
WASI(fd_readdir) u32 fd_readdir(u32,void*,u32,u64,u32*);
WASI(clock_time_get) u32 clock_time_get(u32,u64,u64*);
WASI(random_get) u32 random_get(void*,u32);
}
volatile u32 phase, last_error, bytes_read, file_descriptor, counter;
char arguments[1024],environment[1024],contents[128],directory_entries[1024],guest_name[64];
char *argv[32],*envp[32];
u32 argc,argv_bytes,envc,env_bytes,random_bytes[4];
u64 clock_value;
__attribute__((noinline)) void tour_checkpoint(u32 stage) {
    phase = stage; // Set a source breakpoint here to inspect each completed stage.
}
__attribute__((noinline)) u32 update(u32 value) {
    if (value & 1) return value + 3;
    return value + 1;
}
extern "C" void _start() {
    tour_checkpoint(0); // Begin recording WASI calls before continuing.
    last_error = args_sizes_get(&argc,&argv_bytes);
    if (!last_error && argc<=32 && argv_bytes<=sizeof(arguments)) last_error=args_get(argv,arguments);
    last_error = environ_sizes_get(&envc,&env_bytes);
    if (!last_error && envc<=32 && env_bytes<=sizeof(environment)) last_error=environ_get(envp,environment);
    tour_checkpoint(1); // Arguments and environment are now visible in linear memory.
    u32 preopen[2] = {};
    last_error=fd_prestat_get(3,preopen);
    if (!last_error && preopen[1]<=sizeof(guest_name)) last_error=fd_prestat_dir_name(3,guest_name,preopen[1]);
    u32 count=0;
    last_error=fd_readdir(3,directory_entries,sizeof(directory_entries),0,&count);
    u32 fd=0;
    last_error=path_open(3,0,"input.txt",9,0,2,0,0,&fd);
    file_descriptor=fd;
    if (!last_error) {
        Iovec input{contents,sizeof(contents)};
        last_error=fd_read(fd,&input,1,&count);bytes_read=count;
        fd_close(fd);
    }
    tour_checkpoint(2); // Read-only preopen: observe the directory and closed child FD.
    last_error=clock_time_get(1,1,&clock_value);
    last_error=random_get(random_bytes,sizeof(random_bytes));
    for(u32 i=0;i<4;++i) counter+=update(i); // Branch and call navigation exercise.
    const char message[]="WASIp1 tour: arguments, environment, directory, file, clock, random.\n";
    Iovec output{message,sizeof(message)-1};last_error=fd_write(1,&output,1,&count);
    tour_checkpoint(3); // Continue to revoke the guest directory capability.
    last_error=fd_close(3);
    tour_checkpoint(4); // Preopen is gone. Delete only the tour scratch directory here.
    last_error=path_open(3,0,"input.txt",9,0,2,0,0,&fd);
    tour_checkpoint(5); // Expected BADF (8) after unmount; no host file reopened.
    if (!last_error) fd_close(fd);
}
