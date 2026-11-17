/** @file
 * @brief Allocation-free isolated proposal fixture using one actual sensor input.
 * @details This approved test backend validates the captured input and returns
 * the complete canonical proposal supplied as parameters. It is an echo model,
 * not an LLM or a controller. Host publication must still authenticate the
 * completed job, exact captured buffers, complete output and independent target
 * authority. No target operation, filesystem or socket is used here.
 */
#include <uwvm2/imported/ros/scene/model_worker_protocol.h>
#include <uwvm2/imported/ros/scene/proposal_envelope.h>
namespace worker=uwvm2::imported::ros::scene::model_worker;
namespace proposal=uwvm2::imported::ros::scene::proposals;
extern "C" bool uwvm_model_compute(worker::invocation const& request,worker::emit_function emit,void* context) noexcept
{
    if(!emit || request.input_count!=1 || request.inputs[0].data.size()!=4) { return false; }
    auto input=request.inputs[0].data;
    if(input[0]!=9 || input[1]!=8 || input[2]!=7 || input[3]!=6) { return false; }
    proposal::envelope decoded;
    if(!proposal::decode_envelope(request.parameters,decoded) || decoded.target_alias!="node" ||
       decoded.argument_type!="ros.lifecycle.submit" || decoded.encoding!="le64" || decoded.arguments.size()!=40)
    { return false; }
    return emit(context,worker::response::succeeded,request.parameters);
}
