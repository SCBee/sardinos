#!/usr/bin/env python

# Copyright (c) 2017 Lockheed Martin Corp.  All rights reserved.

import os
import shutil
import subprocess
from stanagE2A1.generator import Generator

example_root_directory = os.path.dirname(os.path.realpath(__file__))

messages_directory = os.path.join(example_root_directory, "messageDefinitions")

if not os.path.exists(messages_directory):
    print "ERROR: {} does not exit".format(messages_directory)
    exit(-1)

Generator.generate(messages_directory, messages_directory)
