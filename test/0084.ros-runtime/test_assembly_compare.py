#!/usr/bin/env python3
"""Reject false assembly-equivalence evidence while preserving relocation facts."""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[2]/'tools/ros_scene'))
from assembly_compare import instructions, normalize


class AssemblyEvidence(unittest.TestCase):
    def test_empty_success_is_not_evidence(self):
        for text in ('', 'binary: file format elf64-x86-64', 'Disassembly of section .text:'):
            with self.assertRaises(ValueError):
                instructions(text, 0x1000, 16)

    def test_exact_extent_and_order(self):
        for text in ('1001: retq', '1000: nop\n1010: retq', '1000: nop\n1000: retq', '1000: <unknown>', '1000: .byte 0xff'):
            with self.assertRaises(ValueError):
                instructions(text, 0x1000, 16)
        self.assertEqual(len(instructions('1000: nop\n1001: retq', 0x1000, 2)), 2)
        with self.assertRaises(ValueError):
            instructions('1000: retq', 0x1000, 0)

    def test_local_branches_and_resolved_relocations(self):
        before = instructions('1000: jne 0x1008 <f+0x8>\n1002: callq 0x5000 <target>\n1007: movq 0x111(%rip), %rax # 0x6000 <global>\n100f: retq', 0x1000, 16)
        after = instructions('2000: jne 0x2008 <f+0x8>\n2002: callq 0x7000 <target>\n2007: movq -0x222(%rip), %rax # 0x8000 <global>\n200f: retq', 0x2000, 16)
        self.assertEqual(normalize(before, 0x1000, 16), normalize(after, 0x2000, 16))

    def test_constants_and_external_targets_are_not_erased(self):
        def one(text):
            return normalize(instructions('1000: ' + text, 0x1000, 16), 0x1000, 16)
        for left, right in [('movq $0x10, %rax', 'movq $0x20, %rax'),
                            ('movq 0x10(%rdi), %rax', 'movq 0x20(%rdi), %rax'),
                            ('callq 0x5000 <allow>', 'callq 0x5000 <deny>'),
                            ('callq 0x5000 <f::{lambda()#1}>', 'callq 0x5000 <f::{lambda()#2}>'),
                            ('callq 0x5000', 'callq 0x6000'),
                            ('jne 0x1004 <f+0x4>', 'jne 0x1008 <f+0x8>'),
                            ('movq 0x10(%rip), %rax', 'movq 0x20(%rip), %rax'),
                            ('movq 0x10(%rip), %rax # 0x2000 <a>', 'movq 0x10(%rip), %rax # 0x2000 <b>')]:
            self.assertNotEqual(one(left), one(right))

    def test_instruction_offsets_remain_visible(self):
        left = instructions('1000: nop\n1001: retq', 0x1000, 16)
        right = instructions('2000: nop\n2002: retq', 0x2000, 16)
        self.assertNotEqual(normalize(left, 0x1000, 16), normalize(right, 0x2000, 16))


if __name__ == '__main__':
    from run_scene_vm import verify_gate
    verify_gate()
    unittest.main()
