"""Presentation changes cannot turn a projected source row into an exact PC."""
import io
import json
import unittest
from test_dap_native_code_navigation import dap


class EditorPositions(unittest.TestCase):
    def test_exactness_survives_all_display_levels(self):
        output = io.BytesIO()
        adapter = dap.Adapter(output, wasm_source_session="11111111-1111-1111-1111-111111111111")
        for exact in (True, False):
            for level in ('source', 'wasm', 'native'):
                with self.subTest(exact=exact, level=level):
                    adapter.step_level = level
                    frame = dict(id=1, name='update', source=dict(path='/project/controller.cpp'),
                                 line=4, column=7, uwvmSourceExact=exact,
                                 instructionPointerReference='uwvm-native-code:2')
                    adapter.respond(dict(seq=1, command='stackTrace'), dict(stackFrames=[frame]))
                    packet = json.loads(output.getvalue().split(b'\r\n\r\n', 1)[1])
                    row = packet['body']['stackFrames'][0]
                    self.assertEqual(row['uwvmSourceExact'], exact)
                    self.assertEqual(row['uwvmSourceLocation'], dict(path='/project/controller.cpp', line=4, column=7))
                    self.assertEqual(row['source'], frame['source'], 'retain native source selection and VARIABLES')
                    self.assertEqual((row['line'], row['column']), (4, 7), 'CALL STACK and native navigation keep the authentic source coordinates')
                    self.assertEqual('instructionPointerReference' in row, level == 'native')
                    self.assertEqual(frame['column'], 7, 'display shaping must not modify authenticated data')
                    output.seek(0); output.truncate()

    def test_wasm_frame_keeps_native_source_marker_and_wasm_projection(self):
        for exact in (True, False):
            for level in ('source', 'wasm', 'native'):
                for visible in (True, False):
                    with self.subTest(exact=exact, level=level, visible=visible):
                        output = io.BytesIO()
                        adapter = dap.Adapter(output, wasm_source_session="11111111-1111-1111-1111-111111111111")
                        adapter.step_level, adapter.wasm_view_visible = level, visible
                        source = dict(path='/project/controller.cpp', line=4, column=7)
                        wasm = dict(path='uwvm-wasm://session/module-0/function-2', sourceReference=31)
                        frame = dict(id=8, name='update', source=wasm, line=12, column=1,
                                     uwvmSourceExact=exact, uwvmSourceLocation=source,
                                     instructionPointerReference='uwvm-native-code:2')
                        adapter.respond(dict(seq=1, command='stackTrace'), dict(stackFrames=[frame], totalFrames=1))
                        body = json.loads(output.getvalue().split(bytes([13, 10, 13, 10]), 1)[1])['body']
                        row = body['stackFrames'][0]
                        self.assertEqual((body['totalFrames'], len(body['stackFrames']), row['id']), (1, 1, 8))
                        self.assertEqual(row['source']['path'], source['path'])
                        self.assertNotIn('sourceReference', row['source'])
                        self.assertEqual((row['line'], row['column']), (4, 7))
                        self.assertEqual(row['uwvmSourceExact'], exact)
                        self.assertEqual(row['uwvmWasmLocation'], dict(source=wasm, line=12, column=1))
                        self.assertEqual(frame['source'], wasm, 'do not mutate cached authenticated frame')
                        self.assertEqual('instructionPointerReference' in row, level == 'native')

    def test_other_ide_retains_standard_source_location(self):
        output = io.BytesIO()
        adapter = dap.Adapter(output)
        frame = dict(id=1, source=dict(path='/project/controller.cpp'), line=4, column=7, uwvmSourceExact=False)
        adapter.respond(dict(seq=1, command='stackTrace'), dict(stackFrames=[frame]))
        row = json.loads(output.getvalue().split(b'\r\n\r\n', 1)[1])['body']['stackFrames'][0]
        self.assertEqual(row, frame)

if __name__ == '__main__': unittest.main()
