Imports System
Imports System.Runtime.CompilerServices
Module NonRos
    Private Structure Sample
        Public Signed As Long
        Public FullId As ULong
        Public Fraction As Double
    End Structure
    <MethodImpl(MethodImplOptions.NoInlining)>
    Private Function FrameThree(input As Long, sample As Sample) As Long
        Dim typedLocal As Long = input + sample.Signed
        Console.Error.WriteLine("UWVM_AOT_DBG_READY visualbasic")
        Dim control = Console.Read() ' DBG_STOP_VISUALBASIC: caller release G follows inspection.
        If control <> AscW("G"c) Then Throw New InvalidOperationException("explicit debugger release G required")
        Return typedLocal
    End Function
    <MethodImpl(MethodImplOptions.NoInlining)>
    Private Function FrameTwo(input As Long, sample As Sample) As Long
        Dim result = FrameThree(input, sample)
        Console.Error.WriteLine("UWVM_AOT_DBG_RESUME_TWO")
        Return result
    End Function
    <MethodImpl(MethodImplOptions.NoInlining)>
    Private Function FrameOne(input As Long, sample As Sample, depth As Integer) As Long
        Dim result = If(depth = 0, FrameTwo(input, sample), FrameOne(input, sample, depth - 1))
        Console.Error.WriteLine("UWVM_AOT_DBG_RESUME_DEPTH=" & depth)
        Return result
    End Function
    Public Function Main(args As String()) As Integer
        Try
            If args.Length <> 2 Then Throw New ArgumentException("signed-int64 expected-nonASCII-label")
            Dim number = Long.Parse(args(0), Globalization.CultureInfo.InvariantCulture)
            Dim sample As New Sample With {.Signed = -7, .FullId = ULong.MaxValue, .Fraction = 1.25}
            Dim result = FrameOne(number, sample, 2)
            Console.WriteLine("label=" & args(1) & " result=" & result & " full-id=" & sample.FullId & " fraction=" & sample.Fraction)
            Return 0
        Catch error As Exception
            Console.Error.WriteLine(error)
            Return 1
        End Try
    End Function
End Module
