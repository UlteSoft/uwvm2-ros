#!/usr/bin/env python3
"""@file test_backend_layout.py
@brief Prevent source-layout checks from overlooking public example directories.
@details Small temporary SDK trees exercise previously missed example-only
platforms, explicit native ROS backends, and path references. No guest is executed.
"""

from pathlib import Path
import tempfile
import unittest

from check_backend_layout import check


class BackendLayoutTests(unittest.TestCase):
    """@brief Check backend layout independently of an installed SDK or compiler."""

    def setUp(self):
        """@brief Allocate a private temporary SDK tree for each regression."""
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)

    def source(self, relative, text=""):
        """@brief Write one fixture source, creating only its private parents.
        @param relative SDK-relative source name under this fixture's root.
        @param text Complete fixture contents.
        @return Path to the created fixture file.
        """
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)
        return path

    def test_example_only_platform_rejects_mixed_backend(self):
        """@brief Detect a platform that has examples but no include directory."""
        self.source("example/platform/fixture/c/ros2_main/observe/xmake.lua")
        result = check(self.root)
        self.assertFalse(result["passed"])
        self.assertEqual(result["violations"][0]["kind"], "backend-directory")

    def test_separate_profile_keeps_selector_identity(self):
        """@brief Accept canonical paths without renaming immutable profile keys."""
        self.source("example/platform/fixture/c/ros2/main/observe/xmake.lua",
                    'set_values("ros.sdk.profile", "ros2_main")\n')
        self.assertTrue(check(self.root)["passed"])

    def test_standard_ros_example_requires_backend(self):
        """@brief Reject an otherwise valid ROS example with an implicit backend."""
        self.source("example/platform/ros/c/publish_string/xmake.lua")
        result = check(self.root)
        self.assertFalse(result["passed"])
        self.assertEqual(result["violations"][0]["kind"], "missing-example-backend")

    def test_standard_ros_generations_are_separate(self):
        """@brief Allow the same scenario name under distinct native ROS backends."""
        self.source("example/platform/ros/c/ros1/publish_string/xmake.lua")
        self.source("example/platform/ros/c/ros2/publish_string/xmake.lua")
        self.assertTrue(check(self.root)["passed"])

    def test_stale_example_reference_is_reported(self):
        """@brief Detect documentation pointing to a removed flat ROS example."""
        self.source("example/platform/ros/c/ros2/call_trigger/xmake.lua")
        self.source("document/guide.md", "example/platform/ros/c/call_trigger\n")
        self.assertFalse(check(self.root)["passed"])

    def test_shared_source_directory_is_not_a_backend(self):
        """@brief Leave common source helpers outside generation-specific folders."""
        self.source("include/platform/fixture/c/shared/transport.h")
        self.source("example/platform/fixture/c/ros1/noetic/observe/xmake.lua")
        self.assertTrue(check(self.root)["passed"])


if __name__ == "__main__":
    unittest.main()
