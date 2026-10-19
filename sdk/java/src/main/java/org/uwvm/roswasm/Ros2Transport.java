package org.uwvm.roswasm;

/** Copied-buffer ROS2 transport. Constructing this object opens nothing.
 * Select this class explicitly; no runtime backend fallback is used.
 * Generated from the complete ABI registry. */
public final class Ros2Transport implements Transport, RetirementTransport {
    private final OpenCleanup openCleanup = new OpenCleanup();
    /** Reserved result-delivery cleanup diagnostics, reset on the next open. */
    public OpenCleanup lastOpenCleanup() { return openCleanup; }
    public int backend() { return 2; }
    public Result invoke(String operation, long handle, byte[] input, int capacity, long other, long correlation) {
        return invokePrepared(operation, handle, input, capacity, other, correlation, null);
    }
    public Result retire(String operation, long handle, byte[] empty, RetirementAttempt attempt) {
        if (!"capability_close".equals(operation) && !"capability_revoke".equals(operation))
            throw new IllegalArgumentException("retirement operation");
        return invokePrepared(operation, handle, empty, 0, 0, 0, attempt);
    }
    private Result invokePrepared(String operation, long handle, byte[] input, int capacity, long other, long correlation, RetirementAttempt attempt) {
        if (operation == null) throw new IllegalArgumentException("operation");
        LinearMemory.Frame f = LinearMemory.enter(input, capacity);
        long openedHandle = 0;
        try {
            int code;
            switch (operation) {
            case "abi_version":
                return Result.value(Ros2Raw.abi_version());
            case "capability_open":
                openCleanup.reset();
                if (f.input.length == 0 || f.input.length > 255) throw new IllegalArgumentException("alias length");
                f.clearValueSlot();
                code = Ros2Raw.capability_open(0, f.inputOffset(), f.input.length, f.slotOffset());
                if (code == 0) openedHandle = f.slotValue();
                Result opened = f.finish(code, LinearMemory.VALUE);
                openedHandle = 0; return opened;
            case "capability_close":
                if (attempt != null) attempt.markAttempted("uwvm_ros2_v1.capability_close");
                code = Ros2Raw.capability_close(handle);
                if (attempt != null) attempt.returned(code);
                return f.finish(code, LinearMemory.NONE);
            case "capability_revoke":
                if (attempt != null) attempt.markAttempted("uwvm_ros2_v1.capability_revoke");
                code = Ros2Raw.capability_revoke(handle);
                if (attempt != null) attempt.returned(code);
                return f.finish(code, LinearMemory.NONE);
            case "topic_publish":
                code = Ros2Raw.topic_publish(handle, 0, f.inputOffset(), f.input.length);
                return f.finish(code, LinearMemory.NONE);
            case "topic_subscribe":
                code = Ros2Raw.topic_subscribe(handle);
                return f.finish(code, LinearMemory.NONE);
            case "event_poll":
                code = Ros2Raw.event_poll(handle, 0, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "event_watch":
                code = Ros2Raw.event_watch(handle, other, correlation);
                return f.finish(code, LinearMemory.NONE);
            case "event_poll_v1":
                code = Ros2Raw.event_poll_v1(handle, 0, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "event_ready":
                if (capacity != 32) throw new IllegalArgumentException("event_ready needs32 bytes");
                code = Ros2Raw.event_ready(handle, 0, f.outputOffset());
                return f.finish(code, LinearMemory.READY);
            case "service_submit":
                f.clearValueSlot();
                code = Ros2Raw.service_submit(handle, other, 0, f.inputOffset(), f.input.length, f.slotOffset());
                return f.finish(code, LinearMemory.VALUE);
            case "service_call":
                code = Ros2Raw.service_call(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "service_advertise":
                code = Ros2Raw.service_advertise(handle);
                return f.finish(code, LinearMemory.NONE);
            case "service_respond":
                code = Ros2Raw.service_respond(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "action_send_goal":
                code = Ros2Raw.action_send_goal(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "action_cancel_own_goal":
                code = Ros2Raw.action_cancel_own_goal(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "action_cancel_any_goal":
                code = Ros2Raw.action_cancel_any_goal(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "action_receive_feedback":
                code = Ros2Raw.action_receive_feedback(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "action_receive_result":
                code = Ros2Raw.action_receive_result(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "action_advertise":
                code = Ros2Raw.action_advertise(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "action_respond":
                code = Ros2Raw.action_respond(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "tf_lookup":
                code = Ros2Raw.tf_lookup(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "tf_broadcast":
                code = Ros2Raw.tf_broadcast(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "parameter_get":
                code = Ros2Raw.parameter_get(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "parameter_set":
                code = Ros2Raw.parameter_set(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "parameter_list":
                code = Ros2Raw.parameter_list(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "parameter_describe":
                code = Ros2Raw.parameter_describe(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "parameter_declare":
                code = Ros2Raw.parameter_declare(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "parameter_undeclare":
                code = Ros2Raw.parameter_undeclare(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "graph_inspect_topic":
                code = Ros2Raw.graph_inspect_topic(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "graph_inspect_node":
                code = Ros2Raw.graph_inspect_node(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "graph_inspect_service":
                code = Ros2Raw.graph_inspect_service(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "graph_inspect_action":
                code = Ros2Raw.graph_inspect_action(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "parameter_subscribe_events":
                code = Ros2Raw.parameter_subscribe_events(handle);
                return f.finish(code, LinearMemory.NONE);
            case "graph_list_topics":
                code = Ros2Raw.graph_list_topics(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "graph_list_nodes":
                code = Ros2Raw.graph_list_nodes(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "graph_list_services":
                code = Ros2Raw.graph_list_services(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "lifecycle_get_state":
                code = Ros2Raw.lifecycle_get_state(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "lifecycle_change_state":
                code = Ros2Raw.lifecycle_change_state(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "time_read_ros":
                code = Ros2Raw.time_read_ros(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "time_read_system":
                code = Ros2Raw.time_read_system(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "time_read_steady":
                code = Ros2Raw.time_read_steady(handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset());
                return f.finish(code, LinearMemory.LENGTH);
            case "log_write":
                code = Ros2Raw.log_write(handle, 0, f.inputOffset(), f.input.length);
                return f.finish(code, LinearMemory.NONE);
            default: return new Result(95);
            }
        } catch (RuntimeException|Error primary) {
            if (openedHandle != 0) {
                openCleanup.started(primary);
                try {
                    int cleanup = Ros2Raw.capability_close(openedHandle);
                    openCleanup.returned(cleanup);
                } catch (RuntimeException|Error cleanup) { openCleanup.failed(cleanup); }
            }
            throw primary;
        } finally { LinearMemory.leave(f); }
    }
}
