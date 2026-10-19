package org.ros2.rcljava.qos;
/** A deployment-selected default-profile token, not a fabricated RMW query.
 * The official defaultProfile() queries native RMW values. This migration mode
 * instead requires an independently qualified alias for that exact deployment
 * profile. A token does not change native QoS. Other profiles fail explicitly
 * until complete native profile/export support is implemented.
 */
public final class QoSProfile {
    private final String key;
    private QoSProfile() {this("official-default-profile");}
    private QoSProfile(String key) {this.key=key;}
    public static QoSProfile defaultProfile() { return new QoSProfile(); }
    public static final QoSProfile DEFAULT = defaultProfile();
    public String deploymentKey() { return key; }
    /** Independently bound service QoS token, never an RMW introspection claim. */
    public static final QoSProfile SERVICES_DEFAULT=new QoSProfile("official-default-service-profile");
    public static QoSProfile keepLast(int depth) { throw unavailable(); }
    public static QoSProfile keepAll() { throw unavailable(); }
    public static QoSProfile sensorData() { throw unavailable(); }
    public static QoSProfile systemDefault() { throw unavailable(); }
    public static QoSProfile parametersDefault() { throw unavailable(); }
    public static QoSProfile servicesDefault() { return SERVICES_DEFAULT; }
    public static QoSProfile parameterEventsDefault() { throw unavailable(); }
    private static UnsupportedOperationException unavailable() { return new UnsupportedOperationException("native QoS profile is not implemented by this migration subset"); }
}
