"""Frozen real Guest entry; one stdin credit admits one observation, no timer.

Define configure() in the explicitly selected frozen deployment entry. Supply
one exact original definition, aliases, concrete QoS80 pair, generated request
and a full server handler (when server=True). None is inferred from discovery.
"""
from roswasm import Backend, Session
from roswasm.owned_service.applications import TypeQueryApplication

application = None


def run(plan, full_request, *, server=False, handler=None, credits):
    global application
    session = Session(Backend(2))
    application = TypeQueryApplication(session, (plan,), server=server)
    primary = None
    try:
        application.open()
        if not server:
            submitted = application.query(full_request)
            if submitted.status:
                return submitted.status
        for _ in credits:
            result = application.step()
            if result.status not in (0, 11):
                return result.status
            if result.delivery is not None:
                item = result.delivery
                if item.control:
                    return item.error or 125
                if server:
                    # Full decode/ACK completed; user handler is outside take.
                    response = handler(item.value, item.information)
                    return application.respond(item.ticket, response)
                # Caller observes the full TypeDescription/type_sources extra
                # information, not only its type name or hash.
                print(item.value)
                return 0
        return 11
    except BaseException as error:
        primary = error
        raise
    finally:
        cleanup = application.close()
        # The globally retained app is the failure carrier even when a primary
        # exception cannot accept attributes or cleanup observation allocates.
        if primary is None and cleanup:
            raise RuntimeError('TypeQuery cleanup failed; inspect application.facts()', cleanup)


def main():
    from roswasm.owned_service.drive import main as drive
    return drive('type_query')
