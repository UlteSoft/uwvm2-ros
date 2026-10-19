"""Six genuine independent parameter endpoints, driven by caller work/credits."""
from roswasm import Backend, Session
from roswasm.owned_service.applications import ParametersApplication

application = None


def run(plans, requests, *, server=False, handlers=None, credits):
    global application
    session = Session(Backend(2))
    application = ParametersApplication(session, plans, server=server)
    primary = None
    try:
        application.open()
        if not server:
            # Requests contain every original field (including full descriptors,
            # ParameterValue arrays and all SetParametersResult reasons).
            for operation, request in requests:
                result = application.submit(operation, request)
                if result.status:
                    return result.status
        for operation in credits:
            result = application.step(operation)
            if result.status not in (0, 11):
                return result.status
            if result.delivery is not None:
                item = result.delivery
                if item.control:
                    return item.error or 125
                if server:
                    response = handlers[operation](item.value, item.information)
                    code = application.respond(operation, item.ticket, response)
                    if code:
                        return code
                else:
                    print(operation, item.value)
        return 0
    except BaseException as error:
        primary = error
        raise
    finally:
        cleanup = application.close()
        if primary is None and cleanup:
            raise RuntimeError('parameter cleanup failed; inspect application.facts()', cleanup)


def main():
    from roswasm.owned_service.drive import main as drive
    return drive('parameters')
