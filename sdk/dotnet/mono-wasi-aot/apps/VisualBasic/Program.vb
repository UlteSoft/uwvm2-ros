Imports System
Imports Uwvm.RosWasm.Aot
Imports Uwvm.RosWasm.Owned
Imports Uwvm.RosWasm.Generated.Control
Imports R = Uwvm.RosWasm.Generated.Control.BoundedRecords
Module Program
    Private Sub TypeQuery(life As ApplicationLifetime, a As String())
        If a.Length <> 7 Then Throw New ArgumentException("type-query call observe available-or-- native-type RIHS steps")
        Dim credits = ApplicationLifetime.Steps(a(6))
        Dim definition = ControlRoots.Ros2Type_description_interfacesSrvGetTypeDescription()
        life.Group = New OwnedGroup(1, 1, definition.MaximumRequest, definition.MaximumResponse)
        Dim client = life.Group.OpenClient(definition, a(1), a(2), If(a(3) = "-", Nothing, a(3)))
        If a(3) <> "-" AndAlso Not client.Available(0UL) Then Throw New InvalidOperationException("actual separately granted availability false")
        Dim request As New R._Ros2Type_description_interfacesSrvGetTypeDescription_Request With {.type_name = a(4), .type_hash = a(5), .include_type_sources = True}
        Dim ticket = client.Submit(request) ' DBG_ROS_SUBMIT_VISUALBASIC
        Dim completed = False
        For credit = 1 To credits
            Dim delivery = client.Poll()
            If Not delivery.Present Then Continue For
            If delivery.IsControl OrElse delivery.Ticket.Value <> ticket.Value OrElse Not delivery.Value.successful OrElse delivery.Value.type_sources.Length = 0 Then Throw New InvalidOperationException("actual complete TypeQuery refusal/correlation/control")
            Console.WriteLine("source-files=" & delivery.Value.type_sources.Length & " referenced=" & delivery.Value.type_description.referenced_type_descriptions.Length & " native-seq=" & delivery.Information.NativeSequence) ' DBG_ROS_DELIVERY_VISUALBASIC
            completed = True
            Exit For
        Next
        If Not completed Then
            client.Abandon(ticket)
            Throw New InvalidOperationException("caller credits exhausted; abandonment is not cancellation or resend")
        End If
    End Sub
    Private Sub GetParameter(life As ApplicationLifetime, a As String())
        If a.Length <> 5 Then Throw New ArgumentException("get call observe parameter-name steps")
        Dim credits = ApplicationLifetime.Steps(a(4))
        Dim definition = ControlRoots.Ros2Rcl_interfacesSrvGetParameters()
        life.Group = New OwnedGroup(1, 1, definition.MaximumRequest, definition.MaximumResponse)
        Dim client = life.Group.OpenClient(definition, a(1), a(2), Nothing)
        Dim request As New R._Ros2Rcl_interfacesSrvGetParameters_Request With {.names = New String() {a(3)}}
        Dim ticket = client.Submit(request)
        Dim completed = False
        For credit = 1 To credits
            Dim delivery = client.Poll()
            If Not delivery.Present Then Continue For
            If delivery.IsControl OrElse delivery.Ticket.Value <> ticket.Value OrElse delivery.Value.values.Length <> 1 Then Throw New InvalidOperationException("complete GetParameters failed")
            Dim value = delivery.Value.values(0)
            Console.WriteLine("type=" & value.[type] & " signed-int64=" & value.integer_value)
            completed = True
            Exit For
        Next
        If Not completed Then
            client.Abandon(ticket)
            Throw New InvalidOperationException("caller credits exhausted")
        End If
    End Sub
    Private Sub SetParameter(life As ApplicationLifetime, a As String())
        If a.Length <> 6 Then Throw New ArgumentException("set call observe parameter-name signed-int64 steps")
        Dim credits = ApplicationLifetime.Steps(a(5))
        Dim number = Long.Parse(a(4), Globalization.CultureInfo.InvariantCulture)
        Dim definition = ControlRoots.Ros2Rcl_interfacesSrvSetParameters()
        life.Group = New OwnedGroup(1, 1, definition.MaximumRequest, definition.MaximumResponse)
        Dim client = life.Group.OpenClient(definition, a(1), a(2), Nothing)
        Dim value As New R._Ros2Rcl_interfacesMsgParameterValue With {.[type] = CByte(2), .integer_value = number}
        Dim parameter As New R._Ros2Rcl_interfacesMsgParameter With {.name = a(3), .[value] = value}
        Dim request As New R._Ros2Rcl_interfacesSrvSetParameters_Request With {.parameters = New R._Ros2Rcl_interfacesMsgParameter() {parameter}}
        Dim ticket = client.Submit(request)
        Dim completed = False
        For credit = 1 To credits
            Dim delivery = client.Poll()
            If Not delivery.Present Then Continue For
            If delivery.IsControl OrElse delivery.Ticket.Value <> ticket.Value OrElse delivery.Value.results.Length <> 1 OrElse Not delivery.Value.results(0).successful Then Throw New InvalidOperationException("actual SetParameters rejected")
            completed = True
            Exit For
        Next
        If Not completed Then
            client.Abandon(ticket)
            Throw New InvalidOperationException("caller credits exhausted")
        End If
    End Sub
    Public Function Main(a As String()) As Integer
        Dim life As New ApplicationLifetime()
        Try
            AotWitness.CheckGuestRoots()
            If a.Length < 1 Then Throw New ArgumentException("type-query|get|set required")
            Select Case a(0)
                Case "type-query" : TypeQuery(life, a)
                Case "get" : GetParameter(life, a)
                Case "set" : SetParameter(life, a)
                Case Else : Throw New ArgumentException("unsupported caller mode")
            End Select
        Catch error As Exception
            life.Primary = error
        End Try
        Return life.Finish()
    End Function
End Module
