/** Actual C reactor denial witness; compile with the complete original SDK.
 * The explicit macro selects one reviewed vendor example source, not a plugin.
 * Run with no Runtime grants. This establishes local Guest refusal only; an
 * independent native observer is required to claim absence of native effects.
 */
#ifndef UWVM_INDUSTRIAL_C_SOURCE
#error "Select the exact actual industrial C example source"
#endif
#include UWVM_INDUSTRIAL_C_SOURCE

int main(void) {
    if(initialize_case(2)!=22)return 1;
    if(initialize_case(0)!=0)return 2;
    if(denied_verified()!=1||primary_status()!=0||cleanup_status()!=0)return 3;
    if(initialize_case(0)!=9||close_case()!=9)return 4;
    return 0;
}
