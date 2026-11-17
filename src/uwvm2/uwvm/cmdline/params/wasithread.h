/** @file @brief Independent WASI Threads (I1T) CLI; default zero spawn authority. */
#pragma once
#ifndef UWVM_MODULE
#include "../callback/wasithread.h"
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbraced-scalar-init"
#endif
namespace uwvm2::uwvm::cmdline::params
{
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_global_enable_alias{u8"-I1Tenable"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_global_enable{
        .name{u8"--wasithread-global-enable"},
        .describe{u8"Enable this explicit scope; interfaces remain separately controlled."},
        .usage{u8""},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_global_enable_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::global,details::wasithread_details::action::enable,0>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_global_disable_alias{u8"-I1Tdisable"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_global_disable{
        .name{u8"--wasithread-global-disable"},
        .describe{u8"Hide this scope; no new thread creation is authorized."},
        .usage{u8""},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_global_disable_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::global,details::wasithread_details::action::disable,0>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_global_interface_alias{u8"-I1Tinterface"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_global_interface{
        .name{u8"--wasithread-global-interface"},
        .describe{u8"Set thread-spawn hidden, deny or allow; independent of WASIp1."},
        .usage{u8"thread-spawn <hidden|deny|allow>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_global_interface_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::global,details::wasithread_details::action::interface,2>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_global_limit_alias{u8"-I1Tlimit"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_global_limit{
        .name{u8"--wasithread-global-limit"},
        .describe{u8"Attenuate admitted thread counts. Zero is a real zero; Global caps also cover Single and Group."},
        .usage{u8"<max-extra-threads|max-threads|max-spawns> <uint|unlimited>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_global_limit_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::global,details::wasithread_details::action::limit,2>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_global_inspect_alias{u8"-I1Tinspect"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_global_inspect{
        .name{u8"--wasithread-global-inspect"},
        .describe{u8"Read I1T policy and reservation counters without changing authority."},
        .usage{u8""},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_global_inspect_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::global,details::wasithread_details::action::inspect,0>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_single_create_alias{u8"-I1TScreate"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_single_create{
        .name{u8"--wasithread-single-create"},
        .describe{u8"Create a denied scope; Single also binds its exact module name."},
        .usage{u8"<module>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_single_create_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::single,details::wasithread_details::action::create,1>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_single_enable_alias{u8"-I1TSenable"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_single_enable{
        .name{u8"--wasithread-single-enable"},
        .describe{u8"Enable this explicit scope; interfaces remain separately controlled."},
        .usage{u8"<module>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_single_enable_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::single,details::wasithread_details::action::enable,1>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_single_disable_alias{u8"-I1TSdisable"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_single_disable{
        .name{u8"--wasithread-single-disable"},
        .describe{u8"Hide this scope; no new thread creation is authorized."},
        .usage{u8"<module>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_single_disable_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::single,details::wasithread_details::action::disable,1>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_single_interface_alias{u8"-I1TSinterface"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_single_interface{
        .name{u8"--wasithread-single-interface"},
        .describe{u8"Set thread-spawn hidden, deny or allow; independent of WASIp1."},
        .usage{u8"<module> thread-spawn <hidden|deny|allow>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_single_interface_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::single,details::wasithread_details::action::interface,3>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_single_limit_alias{u8"-I1TSlimit"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_single_limit{
        .name{u8"--wasithread-single-limit"},
        .describe{u8"Attenuate admitted thread counts. Zero is a real zero; Global caps also cover Single and Group."},
        .usage{u8"<module> <max-extra-threads|max-threads|max-spawns> <uint|unlimited>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_single_limit_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::single,details::wasithread_details::action::limit,3>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_single_inspect_alias{u8"-I1TSinspect"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_single_inspect{
        .name{u8"--wasithread-single-inspect"},
        .describe{u8"Read I1T policy and reservation counters without changing authority."},
        .usage{u8"<module>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_single_inspect_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::single,details::wasithread_details::action::inspect,1>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_group_add_module_alias{u8"-I1TGadd-module"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_group_add_module{
        .name{u8"--wasithread-group-add-module"},
        .describe{u8"Bind an exact module to this Group once; unspecified modules use Global."},
        .usage{u8"<group> <module>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_group_add_module_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::group,details::wasithread_details::action::bind,2>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_group_create_alias{u8"-I1TGcreate"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_group_create{
        .name{u8"--wasithread-group-create"},
        .describe{u8"Create a denied scope; Single also binds its exact module name."},
        .usage{u8"<group>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_group_create_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::group,details::wasithread_details::action::create,1>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_group_enable_alias{u8"-I1TGenable"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_group_enable{
        .name{u8"--wasithread-group-enable"},
        .describe{u8"Enable this explicit scope; interfaces remain separately controlled."},
        .usage{u8"<group>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_group_enable_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::group,details::wasithread_details::action::enable,1>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_group_disable_alias{u8"-I1TGdisable"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_group_disable{
        .name{u8"--wasithread-group-disable"},
        .describe{u8"Hide this scope; no new thread creation is authorized."},
        .usage{u8"<group>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_group_disable_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::group,details::wasithread_details::action::disable,1>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_group_interface_alias{u8"-I1TGinterface"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_group_interface{
        .name{u8"--wasithread-group-interface"},
        .describe{u8"Set thread-spawn hidden, deny or allow; independent of WASIp1."},
        .usage{u8"<group> thread-spawn <hidden|deny|allow>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_group_interface_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::group,details::wasithread_details::action::interface,3>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_group_limit_alias{u8"-I1TGlimit"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_group_limit{
        .name{u8"--wasithread-group-limit"},
        .describe{u8"Attenuate admitted thread counts. Zero is a real zero; Global caps also cover Single and Group."},
        .usage{u8"<group> <max-extra-threads|max-threads|max-spawns> <uint|unlimited>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_group_limit_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::group,details::wasithread_details::action::limit,3>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_group_inspect_alias{u8"-I1TGinspect"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_group_inspect{
        .name{u8"--wasithread-group-inspect"},
        .describe{u8"Read I1T policy and reservation counters without changing authority."},
        .usage{u8"<group>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_group_inspect_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::group,details::wasithread_details::action::inspect,1>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};
    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_global_limit_ratio_alias{u8"-I1Tlimit-ratio"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_global_limit_ratio{
        .name{u8"--wasithread-global-limit-ratio"},
        .describe{u8"Floor a finite f32 ratio in [0,1] of the observable system thread ceiling. No authority is granted."},
        .usage{u8"<max-extra-threads|max-threads> <ratio:f32>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_global_limit_ratio_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::global,details::wasithread_details::action::ratio,2>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};

    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_single_limit_ratio_alias{u8"-I1TSlimit-ratio"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_single_limit_ratio{
        .name{u8"--wasithread-single-limit-ratio"},
        .describe{u8"Floor a finite f32 ratio in [0,1] of the observable system thread ceiling. No authority is granted."},
        .usage{u8"<module> <max-extra-threads|max-threads> <ratio:f32>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_single_limit_ratio_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::single,details::wasithread_details::action::ratio,3>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};

    inline constexpr ::uwvm2::utils::container::u8string_view wasithread_group_limit_ratio_alias{u8"-I1TGlimit-ratio"};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasithread_group_limit_ratio{
        .name{u8"--wasithread-group-limit-ratio"},
        .describe{u8"Floor a finite f32 ratio in [0,1] of the observable system thread ceiling. No authority is granted."},
        .usage{u8"<group> <max-extra-threads|max-threads> <ratio:f32>"},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(wasithread_group_limit_ratio_alias),1uz}},
        .handle{::std::addressof(details::wasithread_details::callback<details::wasithread_details::model::scope::group,details::wasithread_details::action::ratio,3>)} ,
        .cate{::uwvm2::utils::cmdline::categorization::wasithread}};

}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
